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
            std::cerr << "Not enough arguments! 3 args required\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_ACTIVITY:
            std::cerr << "Illegal activity:\n";
            syntaxErr = true;
            break;
        case CLI::Error::INVALID_TYPE:
            std::cerr << "Illegal type\n";
            syntaxErr = true;
            break;
        case CLI::Error::UNKNOWN:
        default:
            std::cerr << "An unknown error has occured\n";
    }
    if (syntaxErr)
        std::cout << "required format: react-cli [activity] [type] [name]\n";

    exit(EXIT_FAILURE);
}
