#include <cstdlib>

#include "include/CLI.hpp"
#include "include/ErrorHandler.hpp"
#include "include/Executor.hpp"

int main(const int argc, const char* argv[]) {
    if (argc < 3) {
        CLI::ErrorHandler(CLI::Error::NOT_ENOUGH_ARGUMENTS);
    }

    CLI::Executor::Run(argc, argv);

    return EXIT_SUCCESS;
}
