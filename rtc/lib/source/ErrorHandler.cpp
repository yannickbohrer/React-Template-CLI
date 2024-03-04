#include <cstdlib>
#include <iostream>

#include "../include/Executor.hpp"

void CLI::ErrorHandler(CLI::Error err) {
    const CLI::Executor& exec = CLI::Executor::Get();
    std::cerr << "\n\n--------------- Error ---------------\nMessage: ";
    bool syntaxErr = false;
    switch (err) {
        case CLI::Error::NOT_ENOUGH_ARGUMENTS:
            std::cerr << "Not enough arguments! Minimum: 1 args required\n";
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
                      << "Fix available: Execute rtc with superuser privileges\n";
            break;
        case CLI::Error::SELECTED_FILE_IS_NOT_REACT_COMPONENT:
            std::cerr << exec.FilePath() << exec.FileName() << " is not a React component\n"
                      << "Fix suggestion: check file extension\n";
            break;
        case CLI::Error::SELECTED_FILE_IS_NOT_A_CUSTOM_TEMPLATE:
            std::cerr << "Requested file is not a custom template\n"
                      << "Call 'rtc list template' for information on existing template files\n";
            break;
        case CLI::Error::INVALID_FLAG_SYNTAX:
            std::cerr << "Invalid flag Syntax. Every flag has to begin with '--'\n";
            syntaxErr = true;
            break;
        case CLI::Error::COULD_NOT_DELETE_CUSTOM_TEMPLATE:
            std::cerr << "Could not delete template file: " << CLI::Config::customTemplatesDir << exec.FileName() << "\n";
            break;
        case CLI::Error::NEW_FILE_EXTENSION_DOES_NOT_MATCH_OLD_EXTENSION:
            std::cerr << "New file extension has to match the existing template file's file extension.\n";
            break;
        case CLI::Error::UNKNOWN:
        default:
            std::cerr << "An unknown error has occured\n";
    }
    if (syntaxErr)
        std::cout << "\nrequired format: rtc [activity] [type] [name] [--flag]\n";
    std::cout << "\nCalls that result in errors will not be added to history\n";

    exit(EXIT_FAILURE);
}
