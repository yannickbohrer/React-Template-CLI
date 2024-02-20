#include <cstdlib>
#include "include/CLI.hpp"
#include "lib/include/ErrorHandler.hpp"
#include "lib/include/Executor.hpp"

int main(const int argc, const char* argv[]) {
    if (argc < 2)
        CLI::ErrorHandler(CLI::Error::NOT_ENOUGH_ARGUMENTS);

    CLI::Executor::Run(argc, argv);
    return EXIT_SUCCESS;
}
