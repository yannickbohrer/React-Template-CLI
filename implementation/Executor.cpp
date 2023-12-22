#include <unistd.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <variant>

#include "../include/CLI.hpp"
#include "../include/ErrorHandler.hpp"
#include "../include/Executor.hpp"

CLI::Executor::Executor()
    : m_Activity(std::monostate{}), m_Type(std::monostate{}), m_Path(""), m_Name("") {}

CLI::Executor& CLI::Executor::Get() {
    static Executor m_Instance;
    return m_Instance;
}

void CLI::Executor::Run(const int argc, const char* argv[]) {
    auto& cli = CLI::Executor::Get();
    cli.MatchActivity(std::string(argv[1]));
    cli.MatchType(std::string(argv[2]));
    if (argc > 3)
        cli.m_Path = argv[3];
    if (argc > 4) {
        int it = 4;
        while (it < argc) {
            std::string flag = std::string(argv[it]);
            if (!flag.starts_with(CLI::Config::flagPrefix))
                CLI::ErrorHandler error(CLI::Error::INVALID_FLAG_SYNTAX);
            cli.m_Flags.emplace_back(flag.substr(2, flag.length() - 2));
            it++;
        }
    }
    cli.Execute();
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

    if (std::holds_alternative<std::monostate>(m_Activity))
        CLI::ErrorHandler error(CLI::Error::INVALID_ACTIVITY);
}

void CLI::Executor::MatchType(const std::string& arg) {
    if (arg == CLI::Tokens::component)
        m_Type = CLI::Type::COMPONENT;
    else if (arg == CLI::Tokens::fileTemplate)
        m_Type = CLI::Type::TEMPLATE;

    if (std::holds_alternative<std::monostate>(m_Type))
        CLI::ErrorHandler error(CLI::Error::INVALID_TYPE);
}

std::tuple<std::string, std::string> CLI::Executor::SplitPathAndName() const {
    size_t pos = m_Path.find_last_of('/');
    if (pos != std::string::npos)
        return std::tuple(m_Path.substr(0, pos + 1), m_Path.substr(pos + 1));
    return std::tuple("", m_Path);
}

void CLI::Executor::Execute() {
    const auto pathAndName = SplitPathAndName();
    m_Path = std::get<0>(pathAndName);
    m_Name = std::get<1>(pathAndName);

    const auto* activity = std::get_if<CLI::Activity>(&m_Activity);

    if (!activity)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);

    switch (*activity) {
        case CLI::Activity::GENERATE:
            Generate();
            break;
        case CLI::Activity::ADD:
            if (geteuid() != 0)
                CLI::ErrorHandler error(CLI::Error::INSUFFICIENT_PERMISSIONS);
            Add();
            break;
        case CLI::Activity::REMOVE:
            if (geteuid() != 0)
                CLI::ErrorHandler error(CLI::Error::INSUFFICIENT_PERMISSIONS);
            Remove();
            break;
        case CLI::Activity::LIST:
            List();
            break;
        default:
            CLI::ErrorHandler error(CLI::Error::UNKNOWN);
    }
}

void CLI::Executor::ApplyTemplate(std::fstream& from, std::fstream& to) const {
    std::string line;
    while (std::getline(from, line)) {
        for (const char c : line) {
            if (c == '%')
                to << m_Name;
            else
                to << c;
        }
        to << '\n';
    }
    from.close();
    to.close();
}

void CLI::Executor::GenerateTemplate(std::fstream& from, std::fstream& to) const {
    const std::string componentName = ExtractComponentName();
    std::string line;
    while (std::getline(from, line)) {
        const std::size_t pos = line.find(componentName);
        if (pos != std::string::npos)
            line.replace(pos, componentName.length(), "%");
        to << line << '\n';
    }
    from.close();
    to.close();
}

void CLI::Executor::Generate() {
    const auto* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::COMPONENT:
            GenerateComponent();
            break;
        default:
            CLI::ErrorHandler error(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::GenerateComponent() {
    bool css = false;
    for (const std::string& flag : m_Flags) {
        if (flag == CLI::Tokens::css)
            css = true;
    }
    const std::string& fileName = ExtractComponentName();
    GenerateRequiredDirectories();
    std::filesystem::create_directory(m_Path + "tests/");

    std::fstream componentFile(m_Path + m_Name + ".jsx", std::ios::out);
    std::fstream componentTestFile(m_Path + "tests/" + m_Name + ".test.js", std::ios::out);

    std::fstream componentTemplate(CLI::Config::assetsDir + "component-js.txt", std::ios::in);

    std::fstream componentTestTemplate(CLI::Config::assetsDir + "component-test-js.txt",
                                       std::ios::in);
    
    std::cout << "task: generating " << fileName << ".jsx           | ";
    ApplyTemplate(componentTemplate, componentFile);
    std::cout << "DONE\n";
    std::cout << "task: generating tests/" << fileName << ".test.js | ";
    ApplyTemplate(componentTestTemplate, componentTestFile);
    std::cout << "DONE\n";

    if (css) {
        std::fstream componentStylesFile(m_Path + m_Name + ".css", std::ios::out);
        std::fstream componentStylesTemplate;
        componentStylesTemplate.open(CLI::Config::assetsDir + "component-styles-css.txt",
                                     std::ios::in);
        std::cout << "task: generating " << fileName << ".css           | ";
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
    const auto* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            AddTemplateFile();
            break;
        default:
            CLI::ErrorHandler error(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::AddTemplateFile() {
    std::fstream file(m_Path + m_Name);

    if (!file.is_open())
        CLI::ErrorHandler error(CLI::Error::INVALID_FILE_PATH);

    std::fstream templateFile(CLI::Config::customAssetsDir + m_Name, std::ios::out);
    GenerateTemplate(file, templateFile);
}

std::string CLI::Executor::ExtractComponentName() const {
    return m_Name.substr(0, m_Name.find_last_of("."));
}

void CLI::Executor::Remove() {
    if (m_Path != "" && m_Path != CLI::Config::customAssetsDir)
        CLI::ErrorHandler error(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);
    if (!IsCustomTemplate())
        CLI::ErrorHandler error(CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE);

    const auto* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            RemoveTemplateFile();
            break;
        default:
            CLI::ErrorHandler error(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

bool CLI::Executor::IsCustomTemplate() const {
    bool result = false;
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::customAssetsDir)) {
        std::string path = std::string(file.path());
        std::string name = path.substr(path.find_last_of("/") + 1);
        if (name == m_Name)
            result = true;
    }
    return result;
}

void CLI::Executor::RemoveTemplateFile() {
    if (std::remove((CLI::Config::customAssetsDir + m_Name).c_str()) != 0)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);
}

void CLI::Executor::List() const {
    const auto* type = std::get_if<CLI::Type>(&m_Type);
    if (!type)
        CLI::ErrorHandler error(CLI::Error::UNKNOWN);

    switch (*type) {
        case CLI::Type::TEMPLATE:
            ListCustomTemplateFiles();
            break;
        default:
            CLI::ErrorHandler error(CLI::Error::INVALID_TYPE_FOR_ACTIVITY);
    }
}

void CLI::Executor::ListCustomTemplateFiles() const {
    std::cout << "react-cli provided templates:\n";
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::assetsDir)) {
        if (file.is_directory())
            continue;
        std::string path = std::string(file.path());
        std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
    std::cout << std::endl;
    if (std::filesystem::is_empty(CLI::Config::customAssetsDir))
        CLI::ErrorHandler error(CLI::Error::NO_CUSTOM_TEMPLATES_FOUND);
    std::cout << "Custom file templates:\n";
    for (const auto& file : std::filesystem::directory_iterator(CLI::Config::customAssetsDir)) {
        std::string path = std::string(file.path());
        std::string name = path.substr(path.find_last_of("/") + 1);
        std::cout << "\t- " << name << "\n";
    }
}
