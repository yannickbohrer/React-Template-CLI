add_library(cli-lib Executor.cpp ErrorHandler.cpp)
target_link_libraries(cli-lib PUBLIC cxx_setup)
