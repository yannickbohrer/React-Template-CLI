#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include "../../include/CLI.hpp"
#include "../include/ErrorHandler.hpp"
#include "../include/Executor.hpp"

CLI::Executor::Executor()
    : m_Activity(std::monostate{}),
      m_Type(std::monostate{}),
      m_Path(""),
      m_Name(""),
      m_NewNameForRename(""),
      m_TypeRequired(true) {}

CLI::Executor& CLI::Executor::Get() {
    static Executor m_Instance;
    return m_Instance;
}

const std::string CLI::Executor::FilePath() const {
    return CLI::Executor::Get().m_Path;
}

const std::string CLI::Executor::FileName() const {
    return CLI::Executor::Get().m_Name;
}

void CLI::Executor::AddToHistory(int argc, const char* argv[]) const {
    std::fstream history(CLI::Config::historyDir + "history.txt", std::ios::app);
    std::string cmd;

    const std::time_t time = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%d.%m.%Y, %H:%M:%S");
    cmd += "[" + oss.str() + "]: ";

    int it = 0;
    while (it < argc) {
        cmd += std::string(argv[it]) + " ";
        it++;
    }
    history << cmd << "\n";
    history.close();
}

const std::array<std::string, 2> CLI::Executor::ParseArgs(const int argc, const char* argv[]) {
    CLI::Executor& cli = CLI::Executor::Get();
    std::array<std::string, 2> res = {""};
    int it = 1, arrIdx = 0;
    while (it < argc) {
        const std::string arg = std::string(argv[it]);
        it++;
        if (arg.starts_with(CLI::Config::flagPrefix))
            cli.m_Flags.emplace_back(arg.substr(2, arg.length() - 2));
        else if (arrIdx < 2) {
            res.at(arrIdx) = arg;
            arrIdx++;
        } else if (arrIdx == 2) {
            cli.m_Path = arg;
            arrIdx++;
        } else if (arrIdx == 3) {
            cli.m_NewNameForRename = arg;
            arrIdx++;
        } else
            CLI::ErrorHandler(CLI::Error::INVALID_FLAG_SYNTAX);
    }
    return res;
}

void CLI::Executor::Run(const int argc, const char* argv[]) {
    CLI::Executor& cli = CLI::Executor::Get();
    std::array<std::string, 2> args = cli.ParseArgs(argc, argv);
    cli.MatchActivity(std::string(std::get<0>(args)));
    if (cli.m_TypeRequired)
        cli.MatchType(std::string(std::get<1>(args)));
    cli.Execute();
    if (*std::get_if<CLI::Activity>(&cli.m_Activity) == CLI::Activity::CLEAR &&
        *std::get_if<CLI::Type>(&cli.m_Type) == CLI::Type::HISTORY)
        return;
    cli.AddToHistory(argc, argv);
}

void CLI::Executor::MatchActivity(const std::string& arg) {
    if (arg == CLI::Tokens::generate)
        m_Activity = CLI::Activity::GENERATE;
    else if (arg == CLI::Tokens::add)
        m_Activity = CLI::Activity::ADD;
    else if (arg == CLI::Tokens::remove)
        m_Activity = CLI::Activity::REMOVE;
    else if (arg == CLI::Tokens::list)
        m_Activity = CLI::Activity::LIST;
    else if (arg == CLI::Tokens::rename)
        m_Activity = CLI::Activity::RENAME;
    else if (arg == CLI::Tokens::print)
        m_Activity = CLI::Activity::PRINT;
    else if (arg == CLI::Tokens::clear)
        m_Activity = CLI::Activity::CLEAR;
    else if (arg == CLI::Tokens::help) {
        m_Activity = CLI::Activity::HELP;
        m_TypeRequired = false;
    }

    if (std::holds_alternative<std::monostate>(m_Activity))
        CLI::ErrorHandler(CLI::Error::INVALID_ACTIVITY);
}

void CLI::Executor::MatchType(const std::string& arg) {
    if (arg == CLI::Tokens::component)
        m_Type = CLI::Type::COMPONENT;
    else if (arg == CLI::Tokens::fileTemplate)
        m_Type = CLI::Type::TEMPLATE;
    else if (arg == CLI::Tokens::history)
        m_Type = CLI::Type::HISTORY;

    if (std::holds_alternative<std::monostate>(m_Type))
        CLI::ErrorHandler(CLI::Error::INVALID_TYPE);
}

const std::tuple<std::string, std::string> CLI::Executor::SplitPathAndName() const {
    size_t pos = m_Path.find_last_of('/');
    if (pos != std::string::npos)
        return std::tuple(m_Path.substr(0, pos + 1), m_Path.substr(pos + 1));

    return std::tuple("", m_Path);
}

void CLI::Executor::Execute() {
    const std::tuple<std::string, std::string> pathAndName = SplitPathAndName();
    m_Path = std::get<0>(pathAndName);
    m_Name = std::get<1>(pathAndName);

    const CLI::Activity* activity = std::get_if<CLI::Activity>(&m_Activity);

    if (!activity)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*activity) {
        case CLI::Activity::GENERATE:
            Generate();
            break;
        case CLI::Activity::ADD:
            Add();
            break;
        case CLI::Activity::REMOVE:
            Remove();
            break;
        case CLI::Activity::LIST:
            List();
            break;
        case CLI::Activity::RENAME:
            Rename();
            break;
        case CLI::Activity::PRINT:
            Print();
            break;
        case CLI::Activity::CLEAR:
            Clear();
            break;
        case CLI::Activity::HELP:
            Help();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::UNKNOWN);
    }
}

void CLI::Executor::Generate() {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::COMPONENT:
            GenerateComponent();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::ApplyTemplate(std::fstream& from, std::fstream& to) const {
    std::string line;
    while (std::getline(from, line)) {
        std::size_t pos = line.find(CLI::Config::templatePlaceholder);
        while (pos != std::string::npos) {
            line.replace(pos, CLI::Config::templatePlaceholder.length(), m_Name);
            pos = line.find(CLI::Config::templatePlaceholder);
        }
        to << line << '\n';
    }
    from.close();
    to.close();
}

void CLI::Executor::GenerateRequiredDirectories() const {
    if (m_Path.empty() || m_Path == "./" || m_Path == "/")
        return;
    int itL = 0, itR = 0;
    std::string lastDir = "";
    while (itR < m_Path.length()) {
        if (m_Path.at(itR) == '/') {
            std::string nextDir = m_Path.substr(itL, itR - itL);
            std::filesystem::create_directory(lastDir + nextDir);
            lastDir += nextDir + '/';
            itL = itR + 1;
        }
        itR++;
    }
}

const std::string CLI::Executor::IsCustomTemplate(const std::string& templateName) const {
    for (const std::filesystem::directory_entry& file :
         std::filesystem::directory_iterator(CLI::Config::customTemplatesDir)) {
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        if (name.starts_with(templateName))
            return name;
    }
    return "";
}

void CLI::Executor::GenerateComponent() {
    bool ts = false;
    bool test = false;
    bool css = false;
    bool customStyles = false;
    std::string componentFileExtension = ".jsx";
    std::string componentTestExtension = ".js";
    std::string customComponentTemplate;
    std::string customStylesTemplate;
    std::string stylesFileExtension;

    for (const std::string& flag : m_Flags) {
        if (flag == CLI::Tokens::typescript) {
            componentFileExtension = ".tsx";
            componentTestExtension = ".ts";
            ts = true;
        }
        if (flag == CLI::Tokens::test)
            test = true;
        if (flag == CLI::Tokens::css)
            css = true;

        if (flag.starts_with(CLI::Tokens::fileTemplate))
            customComponentTemplate =
                flag.substr(flag.find_first_of('=') + 1, flag.length() - CLI::Tokens::fileTemplate.length() - 1);
        if (flag.starts_with(CLI::Tokens::styles)) {
            customStyles = true;
            customStylesTemplate =
                flag.substr(flag.find_first_of('=') + 1, flag.length() - CLI::Tokens::styles.length() - 1);
        }

        if (css && customStyles)
            CLI::ErrorHandler(CLI::Error::TOO_MANY_STYLES_FILES_REQUESTED);
    }

    std::string requestedComponentTemplate;
    if (!customComponentTemplate.empty()) {
        customComponentTemplate = IsCustomTemplate(customComponentTemplate);
        if (customComponentTemplate.empty())
            CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);
        requestedComponentTemplate = CLI::Config::customTemplatesDir + customComponentTemplate;
        componentFileExtension = customComponentTemplate.substr(customComponentTemplate.find_last_of('.'));
    }

    std::string requestedStylesTemplate;
    if (!customStylesTemplate.empty()) {
        customStylesTemplate = IsCustomTemplate(customStylesTemplate);
        if (customStylesTemplate.empty())
            CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);
        requestedStylesTemplate = CLI::Config::customTemplatesDir + customStylesTemplate;
        stylesFileExtension = customStylesTemplate.substr(customStylesTemplate.find_last_of('.'));
    }

    GenerateRequiredDirectories();

    std::fstream componentFile(m_Path + m_Name + componentFileExtension, std::ios::out);
    if (css) {
        const std::string componentName = ExtractComponentName();
        componentFile << "import './" << componentName << ".css'\n\n";
    }

    std::fstream componentTemplate;
    if (customComponentTemplate.empty())
        componentTemplate.open(CLI::Config::templatesDir + CLI::Tokens::componentTemplate, std::ios::in);
    else
        componentTemplate.open(requestedComponentTemplate, std::ios::in);

    std::cout << "task: generating " << m_Name << componentFileExtension << "           | ";
    ApplyTemplate(componentTemplate, componentFile);
    std::cout << "DONE\n";

    if (test) {
        std::filesystem::create_directory(m_Path + "tests/");
        std::string relevantTemplate = CLI::Tokens::jsTestTemplate;
        if (ts)
            relevantTemplate = CLI::Tokens::tsTestTemplate;
        std::fstream componentTestTemplate(CLI::Config::templatesDir + relevantTemplate, std::ios::in);
        std::fstream componentTestFile(m_Path + "tests/" + m_Name + ".test" + componentTestExtension, std::ios::out);
        std::cout << "task: generating tests/" << m_Name << ".test" << componentTestExtension << " | ";
        ApplyTemplate(componentTestTemplate, componentTestFile);
        std::cout << "DONE\n";
    }

    if (css) {
        std::fstream componentStylesFile(m_Path + m_Name + ".css", std::ios::out);
        std::fstream componentStylesTemplate;
        componentStylesTemplate.open(CLI::Config::templatesDir + CLI::Tokens::cssTemplate, std::ios::in);
        std::cout << "task: generating " << m_Name << ".css           | ";
        ApplyTemplate(componentStylesTemplate, componentStylesFile);
        std::cout << "DONE\n";
    }

    if (customStyles) {
        std::fstream stylesTemplate;
        stylesTemplate.open(requestedStylesTemplate, std::ios::in);
        std::fstream componentStylesFile(m_Path + m_Name + stylesFileExtension, std::ios::out);
        std::cout << "task: generating " << m_Name << stylesFileExtension << "           | ";
        ApplyTemplate(stylesTemplate, componentStylesFile);

        componentFile.open(m_Path + m_Name + componentFileExtension, std::ios::in);
        std::vector<std::string> componentFileContents;
        std::string lineBuffer;

        const std::string adjustedStylesFilePath =
            requestedStylesTemplate.substr(requestedStylesTemplate.find_last_of("/") + 1);
        componentFileContents.emplace_back(std::string("import './" + adjustedStylesFilePath + "'"));
        while (std::getline(componentFile, lineBuffer)) {
            componentFileContents.emplace_back(lineBuffer);
        }
        componentFile.close();
        componentFile.open(m_Path + m_Name + componentFileExtension, std::ios::out);
        for (const std::string& line : componentFileContents) {
            componentFile << line << '\n';
        }
        componentFile.close();
        std::cout << "DONE\n";
    }
}

void CLI::Executor::Add() {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            AddTemplateFile();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::GenerateTemplate(std::fstream& from, std::fstream& to) const {
    const std::string componentName = ExtractComponentName();
    std::string line;
    while (std::getline(from, line)) {
        std::size_t pos = line.find(componentName);
        while (pos != std::string::npos) {
            line.replace(pos, componentName.length(), CLI::Config::templatePlaceholder);
            pos = line.find(componentName);
        }
        to << line << '\n';
    }
    from.close();
    to.close();
}

void CLI::Executor::AddTemplateFile() {
    const bool isReactComponent =
        m_Name.ends_with(".jsx") || m_Name.ends_with(".tsx") || m_Name.ends_with(".js") || m_Name.ends_with(".ts");
    const bool isStylesFile = m_Name.ends_with(".css") || m_Name.ends_with(".scss");
    if (!isReactComponent && !isStylesFile)
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_SUITABLE_AS_TEMPLATE);

    std::fstream file(m_Path + m_Name);
    if (!file.is_open())
        CLI::ErrorHandler(CLI::Error::INVALID_FILE_PATH);
    std::fstream templateFile(CLI::Config::customTemplatesDir + m_Name, std::ios::out);

    std::cout << "task: add template " << m_Name << "           | ";
    GenerateTemplate(file, templateFile);
    std::cout << "DONE\n";
}

const std::string CLI::Executor::ExtractComponentName() const {
    return m_Name.substr(0, m_Name.find_last_of("."));
}

void CLI::Executor::Remove() {
    if (m_Path != "" && m_Path != CLI::Config::customTemplatesDir)
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);

    const std::string fileName = IsCustomTemplate(CLI::Executor::Get().m_Name);
    if (fileName.empty())
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);

    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            RemoveTemplateFile(fileName);
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::RemoveTemplateFile(const std::string& fileName) {
    std::cout << "task: remove template " << m_Name << "           | ";
    if (std::remove((CLI::Config::customTemplatesDir + fileName).c_str()) != 0)
        CLI::ErrorHandler(CLI::Error::COULD_NOT_DELETE_CUSTOM_TEMPLATE);
    std::cout << "DONE\n";
}

void CLI::Executor::List() const {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            ListCustomTemplateFiles();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::ListCustomTemplateFiles() const {
    std::cout << "rtc provided templates:\n";
    for (const std::filesystem::directory_entry& file :
         std::filesystem::directory_iterator(CLI::Config::templatesDir)) {
        if (file.is_directory())
            continue;
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
    std::cout << "\nCustom file templates:\n";
    if (std::filesystem::is_empty(CLI::Config::customTemplatesDir)) {
        std::cout << "\tCustom templates directory is empty!\n"
                  << "\nUse 'rtc add template path/to/MyExistingFile' to add custom templates\n";
        return;
    }
    for (const std::filesystem::directory_entry& file :
         std::filesystem::directory_iterator(CLI::Config::customTemplatesDir)) {
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
}

void CLI::Executor::Rename() {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            RenameTemplate();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::RenameTemplate() {
    std::cout << "task: rename template " << m_Name << " to " << m_NewNameForRename << "           | ";
    const std::string fileName = IsCustomTemplate(m_Name);
    if (fileName.empty())
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);

    const std::string oldFileExtension = fileName.substr(fileName.find_last_of('.'));
    if (m_NewNameForRename.find('.') == std::string::npos)
        m_NewNameForRename += oldFileExtension;
    const std::string newFileExtension = m_NewNameForRename.substr(m_NewNameForRename.find_last_of('.'));
    if (oldFileExtension != newFileExtension)
        CLI::ErrorHandler(CLI::Error::NEW_FILE_EXTENSION_DOES_NOT_MATCH_OLD_EXTENSION);

    const std::filesystem::path oldFilePath = std::filesystem::path(CLI::Config::customTemplatesDir + fileName);
    const std::filesystem::path newFilePath =
        std::filesystem::path(CLI::Config::customTemplatesDir + m_NewNameForRename);
    std::filesystem::rename(oldFilePath, newFilePath);
    std::cout << "DONE\n";
}

void CLI::Executor::Print() const {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::HISTORY:
            PrintHistory();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::PrintHistory() const {
    std::fstream history(CLI::Config::historyDir + "history.txt", std::ios::in);
    if (history.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Your history is empty.\nStart using rtc to populate your history!\n";
        history.close();
        return;
    }
    std::string line;
    while (std::getline(history, line)) {
        std::cout << line << "\n";
    }
    history.close();
}

void CLI::Executor::Clear() const {
    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::HISTORY:
            ClearHistory();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::ClearHistory() const {
    std::cout << "task: clear history" << "           | ";
    std::fstream history(CLI::Config::historyDir + "history.txt", std::ios::out);
    history.close();
    std::cout << "DONE\n";
}

void CLI::Executor::Help() const {
    std::fstream help(CLI::Config::assetsDir + "/help.txt", std::ios::in);
    std::string line;
    while (std::getline(help, line)) {
        std::cout << line << "\n";
    }
    help.close();
}
