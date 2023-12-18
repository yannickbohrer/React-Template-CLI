#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include <string>
#include <tuple>
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
    void ApplyTemplate(std::fstream&, std::fstream&) const;

    void Generate();
    void GenerateComponent();
    std::tuple<std::string, std::string> SplitPathAndName() const;

    Executor();
    Executor(const Executor&) = delete;
    static Executor m_Instance;

    std::variant<std::monostate, CLI::Activity> m_Activity;
    std::variant<std::monostate, CLI::Type> m_Type;
    std::string m_Path;
    std::string m_Name;
};

#endif
