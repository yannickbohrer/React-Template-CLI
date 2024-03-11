cmake_minimum_required(VERSION 3.25)
project(rtc VERSION 1.0 DESCRIPTION "")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

add_library(cxx_setup INTERFACE)
target_compile_features(cxx_setup INTERFACE cxx_std_20)
target_include_directories(cxx_setup INTERFACE ${CMAKE_SOURCE_DIR})
target_compile_definitions(cxx_setup INTERFACE ASSETS_DIR="$ENV{HOME}/.rtc")

add_subdirectory(${PROJECT_NAME})
add_subdirectory(executables)

install(TARGETS rtc DESTINATION bin)

# create templates dir and clone assets/templates
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/templates\")")
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/templates/custom\")")
install(DIRECTORY rtc/assets/templates/ DESTINATION $ENV{HOME}/.rtc/templates)

# create history dir and create history file
install(CODE "file(MAKE_DIRECTORY \"$ENV{HOME}/.rtc/history\")")
install(CODE "file(WRITE \"$ENV{HOME}/.rtc/history/history.txt\")")

# clone help.txt file from assets
install(FILES rtc/assets/help.txt DESTINATION $ENV{HOME}/.rtc)

install(CODE "execute_process(COMMAND sudo chown -R $ENV{USER} $ENV{HOME}/.rtc)")
