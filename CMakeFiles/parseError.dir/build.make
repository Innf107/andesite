# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/prophet/Code/Andesite/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/prophet/Code/Andesite

# Include any dependencies generated for this target.
include CMakeFiles/parseError.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/parseError.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/parseError.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/parseError.dir/flags.make

CMakeFiles/parseError.dir/Parser/ParseError.cpp.o: CMakeFiles/parseError.dir/flags.make
CMakeFiles/parseError.dir/Parser/ParseError.cpp.o: src/Parser/ParseError.cpp
CMakeFiles/parseError.dir/Parser/ParseError.cpp.o: CMakeFiles/parseError.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/prophet/Code/Andesite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/parseError.dir/Parser/ParseError.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/parseError.dir/Parser/ParseError.cpp.o -MF CMakeFiles/parseError.dir/Parser/ParseError.cpp.o.d -o CMakeFiles/parseError.dir/Parser/ParseError.cpp.o -c /home/prophet/Code/Andesite/src/Parser/ParseError.cpp

CMakeFiles/parseError.dir/Parser/ParseError.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/parseError.dir/Parser/ParseError.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/prophet/Code/Andesite/src/Parser/ParseError.cpp > CMakeFiles/parseError.dir/Parser/ParseError.cpp.i

CMakeFiles/parseError.dir/Parser/ParseError.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/parseError.dir/Parser/ParseError.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/prophet/Code/Andesite/src/Parser/ParseError.cpp -o CMakeFiles/parseError.dir/Parser/ParseError.cpp.s

# Object files for target parseError
parseError_OBJECTS = \
"CMakeFiles/parseError.dir/Parser/ParseError.cpp.o"

# External object files for target parseError
parseError_EXTERNAL_OBJECTS =

bin/libparseError.a: CMakeFiles/parseError.dir/Parser/ParseError.cpp.o
bin/libparseError.a: CMakeFiles/parseError.dir/build.make
bin/libparseError.a: CMakeFiles/parseError.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/prophet/Code/Andesite/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library bin/libparseError.a"
	$(CMAKE_COMMAND) -P CMakeFiles/parseError.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/parseError.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/parseError.dir/build: bin/libparseError.a
.PHONY : CMakeFiles/parseError.dir/build

CMakeFiles/parseError.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/parseError.dir/cmake_clean.cmake
.PHONY : CMakeFiles/parseError.dir/clean

CMakeFiles/parseError.dir/depend:
	cd /home/prophet/Code/Andesite && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/prophet/Code/Andesite/src /home/prophet/Code/Andesite/src /home/prophet/Code/Andesite /home/prophet/Code/Andesite /home/prophet/Code/Andesite/CMakeFiles/parseError.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/parseError.dir/depend

