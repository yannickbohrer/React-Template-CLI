add_executable(rtc ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/main.cpp)
target_link_libraries(rtc PUBLIC cxx_setup cli-lib)
