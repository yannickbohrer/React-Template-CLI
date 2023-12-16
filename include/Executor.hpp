#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <fstream>
#include <string>
#include <variant>
#include "CLI.hpp"

class CLI::Executor {
public:
    static Executor& Get();
    static void Run(const char*[]);

private:
    void MatchActivity(const std::string&);
    void MatchType(const std::string&);
    void Execute();
    std::string ExtractName() const;

    Executor();
    Executor(const Executor&) = delete;
    static Executor m_Instance;

    std::variant<std::monostate, CLI::Activity> m_Activity;
    std::variant<std::monostate, CLI::Type> m_Type;
    std::string m_Path;
    std::string m_Name;
    std::fstream m_RequiredTemplate;
};

#endif
