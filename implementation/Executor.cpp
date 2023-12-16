#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>

#include "../include/CLI.hpp"
#include "../include/ErrorHandler.hpp"
#include "../include/Executor.hpp"

CLI::Executor::Executor() : m_Activity(std::monostate{}), m_Type(std::monostate{}) {}

CLI::Executor& CLI::Executor::Get() {
    static Executor m_Instance;
    return m_Instance;
}

void CLI::Executor::Run(const char* argv[]) {
    auto& cli = CLI::Executor::Get();
    cli.MatchActivity(std::string(argv[1]));
    cli.MatchType(std::string(argv[2]));
    cli.m_Path = argv[3];
    cli.Execute();
}

void CLI::Executor::MatchActivity(const std::string& arg) {
    if (arg == CLI::Tokens::generate)
        m_Activity = CLI::Activity::GENERATE;

    if (std::holds_alternative<std::monostate>(m_Activity))
        CLI::ErrorHandler error(CLI::Error::ILLEGAL_ACTIVITY);
}

void CLI::Executor::MatchType(const std::string& arg) {
    if (arg == CLI::Tokens::component)
        m_Type = CLI::Type::COMPONENT;

    if (std::holds_alternative<std::monostate>(m_Type))
        CLI::ErrorHandler error(CLI::Error::ILLEGAL_TYPE);
}

std::string CLI::Executor::ExtractName() const {
    size_t found = m_Path.find_last_of('/');
    if (found != std::string::npos)
        return m_Path.substr(found + 1);
    return m_Path;
}

void CLI::Executor::Execute() {
    m_Name = ExtractName();
    std::fstream componentFile("./" + m_Path + ".jsx", std::ios::out);
    std::fstream componentTestFile("./" + m_Path + ".test.js", std::ios::out);
    std::fstream componentStylesFile("./" + m_Path + ".css", std::ios::out);

    std::fstream componentTemplate;
    componentTemplate.open(CLI::Config::assetsDir + "/component-js.txt", std::ios::in);
    
    std::fstream componentTestTemplate;
    componentTestTemplate.open(CLI::Config::assetsDir + "/component-test-js.txt", std::ios::in);
    
    std::fstream componentStylesTemplate;
    componentStylesTemplate.open(CLI::Config::assetsDir + "/component-styles-css.txt", std::ios::in);


    std::string line;
    while (std::getline(componentTemplate, line)) {
        for (const char c : line) {
            if (c == '%')
                componentFile << m_Name;
            else
                componentFile << c;
        }
        componentFile << '\n';
    }
    componentTemplate.close();
    componentFile.close();

    line = "";
    while (std::getline(componentTestTemplate, line)) {
        for (const char c : line) {
            if (c == '%')
                componentTestFile << m_Name;
            else
                componentTestFile << c;
        }
        componentTestFile << '\n';
    }
    componentTestTemplate.close();
    componentTestFile.close();

    line = "";
    while (std::getline(componentStylesTemplate, line)) {
        for (const char c : line) {
            if (c == '%')
                componentStylesFile << m_Name;
            else
                componentStylesFile << c;
        }
        componentStylesFile << '\n';
    }
    componentStylesTemplate.close();
    componentStylesFile.close();
}











