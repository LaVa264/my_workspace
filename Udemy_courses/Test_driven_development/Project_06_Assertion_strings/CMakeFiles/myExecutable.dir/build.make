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
CMAKE_SOURCE_DIR = /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings

# Include any dependencies generated for this target.
include CMakeFiles/myExecutable.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/myExecutable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/myExecutable.dir/flags.make

CMakeFiles/myExecutable.dir/main.cpp.o: CMakeFiles/myExecutable.dir/flags.make
CMakeFiles/myExecutable.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/myExecutable.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/myExecutable.dir/main.cpp.o -c /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/main.cpp

CMakeFiles/myExecutable.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/myExecutable.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/main.cpp > CMakeFiles/myExecutable.dir/main.cpp.i

CMakeFiles/myExecutable.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/myExecutable.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/main.cpp -o CMakeFiles/myExecutable.dir/main.cpp.s

# Object files for target myExecutable
myExecutable_OBJECTS = \
"CMakeFiles/myExecutable.dir/main.cpp.o"

# External object files for target myExecutable
myExecutable_EXTERNAL_OBJECTS =

myExecutable: CMakeFiles/myExecutable.dir/main.cpp.o
myExecutable: CMakeFiles/myExecutable.dir/build.make
myExecutable: libcommonLibrary.a
myExecutable: CMakeFiles/myExecutable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable myExecutable"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/myExecutable.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/myExecutable.dir/build: myExecutable

.PHONY : CMakeFiles/myExecutable.dir/build

CMakeFiles/myExecutable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/myExecutable.dir/cmake_clean.cmake
.PHONY : CMakeFiles/myExecutable.dir/clean

CMakeFiles/myExecutable.dir/depend:
	cd /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings /home/larva/Documents/My_Workspace/my_workspace/Udemy_courses/Test_driven_development/Project_06_Assertion_strings/CMakeFiles/myExecutable.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/myExecutable.dir/depend

