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
CMAKE_BINARY_DIR = /home/Yra/CS144

# Include any dependencies generated for this target.
include tests/CMakeFiles/wrapping_integers_unwrap.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/wrapping_integers_unwrap.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/wrapping_integers_unwrap.dir/flags.make

tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o: tests/CMakeFiles/wrapping_integers_unwrap.dir/flags.make
tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o: tests/wrapping_integers_unwrap.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/Yra/CS144/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o"
	cd /home/Yra/CS144/tests && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o -c /home/Yra/CS144/tests/wrapping_integers_unwrap.cc

tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.i"
	cd /home/Yra/CS144/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/Yra/CS144/tests/wrapping_integers_unwrap.cc > CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.i

tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.s"
	cd /home/Yra/CS144/tests && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/Yra/CS144/tests/wrapping_integers_unwrap.cc -o CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.s

# Object files for target wrapping_integers_unwrap
wrapping_integers_unwrap_OBJECTS = \
"CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o"

# External object files for target wrapping_integers_unwrap
wrapping_integers_unwrap_EXTERNAL_OBJECTS =

tests/wrapping_integers_unwrap: tests/CMakeFiles/wrapping_integers_unwrap.dir/wrapping_integers_unwrap.cc.o
tests/wrapping_integers_unwrap: tests/CMakeFiles/wrapping_integers_unwrap.dir/build.make
tests/wrapping_integers_unwrap: tests/libspongechecks.a
tests/wrapping_integers_unwrap: libsponge/libsponge.a
tests/wrapping_integers_unwrap: tests/CMakeFiles/wrapping_integers_unwrap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/Yra/CS144/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable wrapping_integers_unwrap"
	cd /home/Yra/CS144/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wrapping_integers_unwrap.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/wrapping_integers_unwrap.dir/build: tests/wrapping_integers_unwrap

.PHONY : tests/CMakeFiles/wrapping_integers_unwrap.dir/build

tests/CMakeFiles/wrapping_integers_unwrap.dir/clean:
	cd /home/Yra/CS144/tests && $(CMAKE_COMMAND) -P CMakeFiles/wrapping_integers_unwrap.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/wrapping_integers_unwrap.dir/clean

tests/CMakeFiles/wrapping_integers_unwrap.dir/depend:
	cd /home/Yra/CS144 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/Yra/CS144 /home/Yra/CS144/tests /home/Yra/CS144 /home/Yra/CS144/tests /home/Yra/CS144/tests/CMakeFiles/wrapping_integers_unwrap.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/wrapping_integers_unwrap.dir/depend

