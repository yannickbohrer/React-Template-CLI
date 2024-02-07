cmake_minimum_required(VERSION 3.25)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

project(react-template-cli)
set(CMAKE_CXX_STANDARD 20)
add_executable(rtc main.cpp ./implementation/Executor.cpp
                   ./implementation/ErrorHandler.cpp)

target_compile_features(rtc PRIVATE cxx_range_for)
target_compile_definitions(rtc PRIVATE ASSETS_DIR="$ENV{HOME}/.rtc")

install(TARGETS rtc DESTINATION bin)

# create templates dir and clone assets/templates
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/templates\")")
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/templates/custom\")")
install(DIRECTORY assets/templates/ DESTINATION $ENV{HOME}/.rtc/templates)

# create history dir and create history file
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/history\")")
install(CODE "file(WRITE \"$ENV{HOME}/.rtc/history/history.txt\")")

install(CODE "execute_process(COMMAND sudo chown -R $ENV{USER} $ENV{HOME}/.rtc)")
