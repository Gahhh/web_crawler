# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/web_crawler.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/web_crawler.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/web_crawler.dir/flags.make

CMakeFiles/web_crawler.dir/main.c.o: CMakeFiles/web_crawler.dir/flags.make
CMakeFiles/web_crawler.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/web_crawler.dir/main.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/web_crawler.dir/main.c.o -c /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/main.c

CMakeFiles/web_crawler.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/web_crawler.dir/main.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/main.c > CMakeFiles/web_crawler.dir/main.c.i

CMakeFiles/web_crawler.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/web_crawler.dir/main.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/main.c -o CMakeFiles/web_crawler.dir/main.c.s

CMakeFiles/web_crawler.dir/list.c.o: CMakeFiles/web_crawler.dir/flags.make
CMakeFiles/web_crawler.dir/list.c.o: ../list.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/web_crawler.dir/list.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/web_crawler.dir/list.c.o -c /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/list.c

CMakeFiles/web_crawler.dir/list.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/web_crawler.dir/list.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/list.c > CMakeFiles/web_crawler.dir/list.c.i

CMakeFiles/web_crawler.dir/list.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/web_crawler.dir/list.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/list.c -o CMakeFiles/web_crawler.dir/list.c.s

# Object files for target web_crawler
web_crawler_OBJECTS = \
"CMakeFiles/web_crawler.dir/main.c.o" \
"CMakeFiles/web_crawler.dir/list.c.o"

# External object files for target web_crawler
web_crawler_EXTERNAL_OBJECTS =

web_crawler: CMakeFiles/web_crawler.dir/main.c.o
web_crawler: CMakeFiles/web_crawler.dir/list.c.o
web_crawler: CMakeFiles/web_crawler.dir/build.make
web_crawler: CMakeFiles/web_crawler.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable web_crawler"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/web_crawler.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/web_crawler.dir/build: web_crawler

.PHONY : CMakeFiles/web_crawler.dir/build

CMakeFiles/web_crawler.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/web_crawler.dir/cmake_clean.cmake
.PHONY : CMakeFiles/web_crawler.dir/clean

CMakeFiles/web_crawler.dir/depend:
	cd /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug /Users/yuliang/Desktop/IT/COMP9024/assignment/web_crawler/cmake-build-debug/CMakeFiles/web_crawler.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/web_crawler.dir/depend

