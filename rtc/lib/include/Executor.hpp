#pragma once

#include <string>
#include <tuple>
#include <variant>
#include <vector>
#include "../../include/CLI.hpp"

class CLI::Executor {
public:
    static Executor& Get();
    static void Run(const int, const char*[]);

    const std::string FilePath() const;
    const std::string FileName() const;

private:
    const std::array<std::string, 2> ParseArgs(const int, const char*[]);
    void MatchActivity(const std::string&);
    void MatchType(const std::string&);
    const std::tuple<std::string, std::string> SplitPathAndName() const;
    void Execute();
    void ApplyTemplate(std::fstream&, std::fstream&) const;
    void GenerateTemplate(std::fstream&, std::fstream&) const;

    void Generate();
    void GenerateComponent();
    void GenerateRequiredDirectories() const;

    void Add();
    void AddTemplateFile();
    const std::string ExtractComponentName() const;

    void Remove();
    void RemoveTemplateFile(const std::string&);
    const std::string IsCustomTemplate(const std::string&) const;

    void List() const;
    void ListCustomTemplateFiles() const;

    void Print() const;
    void PrintHistory() const;

    void Clear() const;
    void ClearHistory() const;

    void Rename();
    void RenameTemplate();

    void AddToHistory(int, const char*[]) const;

    void Help() const;

    Executor();
    Executor(const Executor&) = delete;
    static Executor m_Instance;

    std::variant<std::monostate, CLI::Activity> m_Activity;
    std::variant<std::monostate, CLI::Type> m_Type;
    std::string m_Path;
    std::string m_Name;
    std::string m_NewNameForRename;
    bool m_TypeRequired;
    std::vector<std::string> m_Flags;
};
