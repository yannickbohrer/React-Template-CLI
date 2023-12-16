cmake_minimum_required(VERSION 3.25)


set(CMAKE_INSTALL_PREFIX /usr/local)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(react-cli)
set(CMAKE_CXX_STANDARD 20)
add_executable(react-cli main.cpp ./implementation/Executor.cpp
                         ./implementation/ErrorHandler.cpp)
target_compile_features(react-cli PRIVATE cxx_range_for)

# Add the installation commands
install(TARGETS react-cli DESTINATION bin) 
install(DIRECTORY assets/ DESTINATION share/react-cli/assets)
