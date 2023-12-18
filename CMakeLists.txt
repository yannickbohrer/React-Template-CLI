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

install(TARGETS react-cli DESTINATION bin) 

#[[ set(ASSETS_DIRECTORY "${CMAKE_INSTALL_PREFIX}/share/react-cli/assets")
set(CUSTOM_DIRECTORY "${ASSETS_DIRECTORY}/custom")
file(MAKE_DIRECTORY ${CUSTOM_DIRECTORY})
install(DIRECTORY assets/ DESTINATION share/react-cli/assets) ]]

install(CODE "file(MAKE_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/share/react-cli/assets/templates/custom\")")
install(DIRECTORY assets/ DESTINATION share/react-cli/assets)
