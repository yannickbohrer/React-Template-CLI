# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/cmake-3.27.2-linux-x86_64/bin/cmake

# The command to remove a file.
RM = /opt/cmake-3.27.2-linux-x86_64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/yanboh/dev/private/projects/react-template-cli

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/yanboh/dev/private/projects/react-template-cli/build

# Include any dependencies generated for this target.
include rtc/cli-lib/source/CMakeFiles/cli-lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include rtc/cli-lib/source/CMakeFiles/cli-lib.dir/compiler_depend.make

# Include the progress variables for this target.
include rtc/cli-lib/source/CMakeFiles/cli-lib.dir/progress.make

# Include the compile flags for this target's objects.
include rtc/cli-lib/source/CMakeFiles/cli-lib.dir/flags.make

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/flags.make
rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o: /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/Executor.cpp
rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yanboh/dev/private/projects/react-template-cli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o -MF CMakeFiles/cli-lib.dir/Executor.cpp.o.d -o CMakeFiles/cli-lib.dir/Executor.cpp.o -c /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/Executor.cpp

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cli-lib.dir/Executor.cpp.i"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/Executor.cpp > CMakeFiles/cli-lib.dir/Executor.cpp.i

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cli-lib.dir/Executor.cpp.s"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/Executor.cpp -o CMakeFiles/cli-lib.dir/Executor.cpp.s

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/flags.make
rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o: /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/ErrorHandler.cpp
rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/yanboh/dev/private/projects/react-template-cli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o -MF CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o.d -o CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o -c /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/ErrorHandler.cpp

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/cli-lib.dir/ErrorHandler.cpp.i"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/ErrorHandler.cpp > CMakeFiles/cli-lib.dir/ErrorHandler.cpp.i

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/cli-lib.dir/ErrorHandler.cpp.s"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source/ErrorHandler.cpp -o CMakeFiles/cli-lib.dir/ErrorHandler.cpp.s

# Object files for target cli-lib
cli__lib_OBJECTS = \
"CMakeFiles/cli-lib.dir/Executor.cpp.o" \
"CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o"

# External object files for target cli-lib
cli__lib_EXTERNAL_OBJECTS =

rtc/cli-lib/source/libcli-lib.a: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/Executor.cpp.o
rtc/cli-lib/source/libcli-lib.a: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/ErrorHandler.cpp.o
rtc/cli-lib/source/libcli-lib.a: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/build.make
rtc/cli-lib/source/libcli-lib.a: rtc/cli-lib/source/CMakeFiles/cli-lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/yanboh/dev/private/projects/react-template-cli/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libcli-lib.a"
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && $(CMAKE_COMMAND) -P CMakeFiles/cli-lib.dir/cmake_clean_target.cmake
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/cli-lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
rtc/cli-lib/source/CMakeFiles/cli-lib.dir/build: rtc/cli-lib/source/libcli-lib.a
.PHONY : rtc/cli-lib/source/CMakeFiles/cli-lib.dir/build

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/clean:
	cd /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source && $(CMAKE_COMMAND) -P CMakeFiles/cli-lib.dir/cmake_clean.cmake
.PHONY : rtc/cli-lib/source/CMakeFiles/cli-lib.dir/clean

rtc/cli-lib/source/CMakeFiles/cli-lib.dir/depend:
	cd /home/yanboh/dev/private/projects/react-template-cli/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/yanboh/dev/private/projects/react-template-cli /home/yanboh/dev/private/projects/react-template-cli/rtc/cli-lib/source /home/yanboh/dev/private/projects/react-template-cli/build /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source /home/yanboh/dev/private/projects/react-template-cli/build/rtc/cli-lib/source/CMakeFiles/cli-lib.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : rtc/cli-lib/source/CMakeFiles/cli-lib.dir/depend
