# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/Yra/CS144

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/Yra/CS144/build

# Utility rule file for cppcheck.

# Include the progress variables for this target.
include CMakeFiles/cppcheck.dir/progress.make

CMakeFiles/cppcheck:
	cppcheck --enable=all --project="/home/Yra/CS144/build/compile_commands.json"

cppcheck: CMakeFiles/cppcheck
cppcheck: CMakeFiles/cppcheck.dir/build.make

.PHONY : cppcheck

# Rule to build all files generated by this target.
CMakeFiles/cppcheck.dir/build: cppcheck

.PHONY : CMakeFiles/cppcheck.dir/build

CMakeFiles/cppcheck.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/cppcheck.dir/cmake_clean.cmake
.PHONY : CMakeFiles/cppcheck.dir/clean

CMakeFiles/cppcheck.dir/depend:
	cd /home/Yra/CS144/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Yra/CS144 /home/Yra/CS144 /home/Yra/CS144/build /home/Yra/CS144/build /home/Yra/CS144/build/CMakeFiles/cppcheck.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/cppcheck.dir/depend

