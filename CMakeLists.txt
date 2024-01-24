cmake_minimum_required(VERSION 3.25)

set(CMAKE_INSTALL_PREFIX /usr/local)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

project(react-template-cli)
set(CMAKE_CXX_STANDARD 20)
add_executable(rtc main.cpp ./implementation/Executor.cpp
                         ./implementation/ErrorHandler.cpp)
target_compile_features(rtc PRIVATE cxx_range_for)

install(TARGETS rtc DESTINATION bin) 

# create templates dir and clone assets/templates into it
install(CODE "file(MAKE_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/share/rtc/templates\")")
install(CODE "file(MAKE_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/share/rtc/templates/custom\")")
install(DIRECTORY assets/templates/ DESTINATION share/rtc/templates)

# create history dir and create history file
install(CODE "file(MAKE_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/share/rtc/history\")")
install(CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/share/rtc/history/history.txt\")")
