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
CMAKE_SOURCE_DIR = /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development

# Include any dependencies generated for this target.
include CMakeFiles/Sum_library.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Sum_library.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Sum_library.dir/flags.make

CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o: CMakeFiles/Sum_library.dir/flags.make
CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o: Libraries/sum.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o -c /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Libraries/sum.cpp

CMakeFiles/Sum_library.dir/Libraries/sum.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Sum_library.dir/Libraries/sum.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Libraries/sum.cpp > CMakeFiles/Sum_library.dir/Libraries/sum.cpp.i

CMakeFiles/Sum_library.dir/Libraries/sum.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Sum_library.dir/Libraries/sum.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Libraries/sum.cpp -o CMakeFiles/Sum_library.dir/Libraries/sum.cpp.s

# Object files for target Sum_library
Sum_library_OBJECTS = \
"CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o"

# External object files for target Sum_library
Sum_library_EXTERNAL_OBJECTS =

libSum_library.a: CMakeFiles/Sum_library.dir/Libraries/sum.cpp.o
libSum_library.a: CMakeFiles/Sum_library.dir/build.make
libSum_library.a: CMakeFiles/Sum_library.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libSum_library.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Sum_library.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Sum_library.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Sum_library.dir/build: libSum_library.a

.PHONY : CMakeFiles/Sum_library.dir/build

CMakeFiles/Sum_library.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Sum_library.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Sum_library.dir/clean

CMakeFiles/Sum_library.dir/depend:
	cd /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/CMakeFiles/Sum_library.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Sum_library.dir/depend

