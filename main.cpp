#include <cstdlib>

#include "include/CLI.hpp"
#include "include/ErrorHandler.hpp"
#include "include/Executor.hpp"

int main(const int argc, const char* argv[]) {
    if (argc != 4) {
        CLI::ErrorHandler error(CLI::Error::NOT_ENOUGH_ARGUMENTS);
    }

    CLI::Executor::Run(argv);

    return EXIT_SUCCESS;
}
