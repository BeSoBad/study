# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = "/home/bsb/github/study/numeric methods/1"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/bsb/github/study/numeric methods/1/build"

# Include any dependencies generated for this target.
include 1.1/CMakeFiles/main.dir/depend.make

# Include the progress variables for this target.
include 1.1/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include 1.1/CMakeFiles/main.dir/flags.make

1.1/CMakeFiles/main.dir/main.cpp.o: 1.1/CMakeFiles/main.dir/flags.make
1.1/CMakeFiles/main.dir/main.cpp.o: ../1.1/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/bsb/github/study/numeric methods/1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object 1.1/CMakeFiles/main.dir/main.cpp.o"
	cd "/home/bsb/github/study/numeric methods/1/build/1.1" && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/main.dir/main.cpp.o -c "/home/bsb/github/study/numeric methods/1/1.1/main.cpp"

1.1/CMakeFiles/main.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cpp.i"
	cd "/home/bsb/github/study/numeric methods/1/build/1.1" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/bsb/github/study/numeric methods/1/1.1/main.cpp" > CMakeFiles/main.dir/main.cpp.i

1.1/CMakeFiles/main.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cpp.s"
	cd "/home/bsb/github/study/numeric methods/1/build/1.1" && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/bsb/github/study/numeric methods/1/1.1/main.cpp" -o CMakeFiles/main.dir/main.cpp.s

1.1/CMakeFiles/main.dir/main.cpp.o.requires:

.PHONY : 1.1/CMakeFiles/main.dir/main.cpp.o.requires

1.1/CMakeFiles/main.dir/main.cpp.o.provides: 1.1/CMakeFiles/main.dir/main.cpp.o.requires
	$(MAKE) -f 1.1/CMakeFiles/main.dir/build.make 1.1/CMakeFiles/main.dir/main.cpp.o.provides.build
.PHONY : 1.1/CMakeFiles/main.dir/main.cpp.o.provides

1.1/CMakeFiles/main.dir/main.cpp.o.provides.build: 1.1/CMakeFiles/main.dir/main.cpp.o


# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cpp.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

1.1/main: 1.1/CMakeFiles/main.dir/main.cpp.o
1.1/main: 1.1/CMakeFiles/main.dir/build.make
1.1/main: libmatrix.a
1.1/main: libvector.a
1.1/main: liboperations.a
1.1/main: 1.1/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/bsb/github/study/numeric methods/1/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable main"
	cd "/home/bsb/github/study/numeric methods/1/build/1.1" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
1.1/CMakeFiles/main.dir/build: 1.1/main

.PHONY : 1.1/CMakeFiles/main.dir/build

1.1/CMakeFiles/main.dir/requires: 1.1/CMakeFiles/main.dir/main.cpp.o.requires

.PHONY : 1.1/CMakeFiles/main.dir/requires

1.1/CMakeFiles/main.dir/clean:
	cd "/home/bsb/github/study/numeric methods/1/build/1.1" && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : 1.1/CMakeFiles/main.dir/clean

1.1/CMakeFiles/main.dir/depend:
	cd "/home/bsb/github/study/numeric methods/1/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/bsb/github/study/numeric methods/1" "/home/bsb/github/study/numeric methods/1/1.1" "/home/bsb/github/study/numeric methods/1/build" "/home/bsb/github/study/numeric methods/1/build/1.1" "/home/bsb/github/study/numeric methods/1/build/1.1/CMakeFiles/main.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : 1.1/CMakeFiles/main.dir/depend
