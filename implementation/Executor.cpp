#include <unistd.h>
#include <array>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>

#include "../include/CLI.hpp"
#include "../include/ErrorHandler.hpp"
#include "../include/Executor.hpp"

CLI::Executor::Executor() : m_Activity(std::monostate{}), m_Type(std::monostate{}), m_Path(""), m_Name("") {}

CLI::Executor& CLI::Executor::Get() {
    static Executor m_Instance;
    return m_Instance;
}

std::array<std::string, 2> CLI::Executor::ExtractArgs(const int argc, const char* argv[]) {
    CLI::Executor& cli = CLI::Executor::Get();
    std::array<std::string, 2> res;
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
        } else
            CLI::ErrorHandler(CLI::Error::INVALID_FLAG_SYNTAX);
    }
    return res;
}

void CLI::Executor::Run(const int argc, const char* argv[]) {
    CLI::Executor& cli = CLI::Executor::Get();
    std::array<std::string, 2> args = cli.ExtractArgs(argc, argv);
    cli.MatchActivity(std::string(std::get<0>(args)));
    if (*std::get_if<CLI::Activity>(&cli.m_Activity) != CLI::Activity::HISTORY)
        cli.MatchType(std::string(std::get<1>(args)));
    cli.Execute();
    cli.AddToHistory(argc, argv);
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

std::string CLI::Executor::FilePath() const {
    return CLI::Executor::Get().m_Path;
}

std::string CLI::Executor::FileName() const {
    return CLI::Executor::Get().m_Name;
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
    else if (arg == CLI::Tokens::history)
        m_Activity = CLI::Activity::HISTORY;

    if (std::holds_alternative<std::monostate>(m_Activity))
        CLI::ErrorHandler(CLI::Error::INVALID_ACTIVITY);
}

void CLI::Executor::MatchType(const std::string& arg) {
    if (arg == CLI::Tokens::component)
        m_Type = CLI::Type::COMPONENT;
    else if (arg == CLI::Tokens::fileTemplate)
        m_Type = CLI::Type::TEMPLATE;

    if (std::holds_alternative<std::monostate>(m_Type))
        CLI::ErrorHandler(CLI::Error::INVALID_TYPE);
}

std::tuple<std::string, std::string> CLI::Executor::SplitPathAndName() const {
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
        case CLI::Activity::HISTORY:
            History();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::UNKNOWN);
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

void CLI::Executor::GenerateComponent() {
    bool test = false;
    bool css = false;
    bool ts = false;
    std::string componentFileExtension = ".jsx";
    std::string componentTestExtension = ".js";
    std::string customTemplate;

    for (const std::string& flag : m_Flags) {
        if (flag == CLI::Tokens::css)
            css = true;
        if (flag == CLI::Tokens::test)
            test = true;
        if (flag == CLI::Tokens::typescript) {
            componentFileExtension = ".tsx";
            componentTestExtension = ".ts";
            ts = true;
        }
        if (flag.starts_with(CLI::Tokens::fileTemplate))
            customTemplate =
                flag.substr(flag.find_first_of('=') + 1, flag.length() - CLI::Tokens::fileTemplate.length() - 1);
    }

    std::string requestedTemplate;
    if (!customTemplate.empty()) {
        customTemplate = IsCustomTemplate(customTemplate);
        if (customTemplate.empty())
            CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);
        requestedTemplate = CLI::Config::customTemplatesDir + customTemplate;
        componentFileExtension = customTemplate.substr(customTemplate.find_last_of('.'));
    }

    GenerateRequiredDirectories();

    std::fstream componentFile(m_Path + m_Name + componentFileExtension, std::ios::out);
    if (css) {
        const std::string componentName = ExtractComponentName();
        componentFile << "import './" << componentName << ".css'\n\n";
    }

    std::fstream componentTemplate;
    if (customTemplate.empty())
        componentTemplate.open(CLI::Config::templatesDir + CLI::Tokens::componentTemplate, std::ios::in);
    else
        componentTemplate.open(requestedTemplate, std::ios::in);

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

void CLI::Executor::Add() {
    if (!m_Name.ends_with(".jsx") && !m_Name.ends_with(".tsx") && !m_Name.ends_with(".js") && !m_Name.ends_with(".ts"))
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_REACT_COMPONENT);
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

void CLI::Executor::AddTemplateFile() {
    std::fstream file(m_Path + m_Name);
    if (!file.is_open())
        CLI::ErrorHandler(CLI::Error::INVALID_FILE_PATH);
    std::fstream templateFile(CLI::Config::customTemplatesDir + m_Name, std::ios::out);

    std::cout << "task: add template " << m_Name << "           | ";
    GenerateTemplate(file, templateFile);
    std::cout << "DONE\n";
}

std::string CLI::Executor::ExtractComponentName() const {
    return m_Name.substr(0, m_Name.find_last_of("."));
}

void CLI::Executor::Remove() {
    if (m_Path != "" && m_Path != CLI::Config::customTemplatesDir)
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);
    if (IsCustomTemplate(CLI::Executor::Get().m_Name).empty())
        CLI::ErrorHandler(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);

    const CLI::Type* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            RemoveTemplateFile();
            break;
        default:
            CLI::ErrorHandler(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

std::string CLI::Executor::IsCustomTemplate(const std::string& templateName) const {
    // TODO std::filesystem::directory_entry
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::customTemplatesDir)) {
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        if (name.starts_with(templateName))
            return name;
    }
    return "";
}

void CLI::Executor::RemoveTemplateFile() {
    std::cout << "task: remove template " << m_Name << "           | ";
    if (std::remove((CLI::Config::customTemplatesDir + m_Name).c_str()) != 0)
        CLI::ErrorHandler(CLI::Error::UNKNOWN);
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
    // TODO std::filesystem::directory_entry
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::templatesDir)) {
        if (file.is_directory())
            continue;
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
    std::cout << std::endl;
    if (std::filesystem::is_empty(CLI::Config::customTemplatesDir))
        CLI::ErrorHandler(CLI::Error::NO_CUSTOM_TEMPLATES_FOUND);
    std::cout << "Custom file templates:\n";
    // TODO std::filesystem::directory_entry
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::customTemplatesDir)) {
        const std::string path = std::string(file.path());
        const std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
}

void CLI::Executor::History() const {
    std::fstream history(CLI::Config::historyDir + "history.txt", std::ios::in);
    if (history.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Your history is empty.\nStart using rtc to populate your history!\n";
        return;
    }
    std::string line;
    while (std::getline(history, line)) {
        std::cout << line << "\n";
    }
}
