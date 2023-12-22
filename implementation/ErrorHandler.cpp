#include <cstdlib>
#include <iostream>

#include "../include/ErrorHandler.hpp"
#include "../include/Executor.hpp"

CLI::ErrorHandler::ErrorHandler(CLI::Error err) {
    const auto& exec = CLI::Executor::Get();
    std::cerr << "--------------- Error ---------------\nMessage: ";
    bool syntaxErr = false;
    switch (err) {
        case CLI::Error::NOT_ENOUGH_ARGUMENTS:
            std::cerr << "Not enough arguments! Minimum: 2 args required\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_ACTIVITY:
            std::cerr << "Invalid activity\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_TYPE:
            std::cerr << "Invalid type\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_TYPE_FOR_ACTIVITY:
            std::cerr << "Activity and type are not compatible\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_FILE_PATH:
            std::cerr << "File not found: " << exec.FilePath() << exec.FileName() << "\n";
            break;
        case CLI::Error::INSUFFICIENT_PERMISSIONS:
            std::cerr << "Insufficient Permissions!\n"
                << "Fix available: Execute react-cli with superuser privileges\n";
            break;
        case CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE:
            std::cerr << "Requested file is not a custom template\n"
                << "Call 'react-cli list template' for information on existing template files\n";
            break;
        case CLI::Error::NO_CUSTOM_TEMPLATES_FOUND:
            std::cerr << "Custom templated directory is empty\n"
                << "Use 'react-cli generate template path/to/MyExistingFile' to generate custom templates\n";
            break;
        case CLI::Error::INVALID_FLAG_SYNTAX:
            std::cerr << "Invalid flag Syntax. Every flag has to begin with '--'\n";
            syntaxErr = true;
            break;
        case CLI::Error::UNKNOWN:
        default:
            std::cerr << "An unknown error has occured\n";
    }
    if (syntaxErr)
        std::cout << "required format: react-cli [activity] [type] [name] [--flag]\n";

    exit(EXIT_FAILURE);
}
