# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /opt/clion-2019.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2019.2/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tester/Documents/GitHub/TaskLunev/Signal

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Signal.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Signal.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Signal.dir/flags.make

CMakeFiles/Signal.dir/main.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Signal.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Signal.dir/main.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Signal/main.cpp

CMakeFiles/Signal.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Signal.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Signal/main.cpp > CMakeFiles/Signal.dir/main.cpp.i

CMakeFiles/Signal.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Signal/main.cpp -o CMakeFiles/Signal.dir/main.cpp.s

CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o: /home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp

CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp > CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.i

CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp -o CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.s

CMakeFiles/Signal.dir/wrrd.cpp.o: CMakeFiles/Signal.dir/flags.make
CMakeFiles/Signal.dir/wrrd.cpp.o: ../wrrd.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Signal.dir/wrrd.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Signal.dir/wrrd.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Signal/wrrd.cpp

CMakeFiles/Signal.dir/wrrd.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Signal.dir/wrrd.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Signal/wrrd.cpp > CMakeFiles/Signal.dir/wrrd.cpp.i

CMakeFiles/Signal.dir/wrrd.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Signal.dir/wrrd.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Signal/wrrd.cpp -o CMakeFiles/Signal.dir/wrrd.cpp.s

# Object files for target Signal
Signal_OBJECTS = \
"CMakeFiles/Signal.dir/main.cpp.o" \
"CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o" \
"CMakeFiles/Signal.dir/wrrd.cpp.o"

# External object files for target Signal
Signal_EXTERNAL_OBJECTS =

Signal: CMakeFiles/Signal.dir/main.cpp.o
Signal: CMakeFiles/Signal.dir/home/tester/Documents/GitHub/TaskLunev/AddLib/AddFunc.cpp.o
Signal: CMakeFiles/Signal.dir/wrrd.cpp.o
Signal: CMakeFiles/Signal.dir/build.make
Signal: CMakeFiles/Signal.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable Signal"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Signal.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Signal.dir/build: Signal

.PHONY : CMakeFiles/Signal.dir/build

CMakeFiles/Signal.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Signal.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Signal.dir/clean

CMakeFiles/Signal.dir/depend:
	cd /home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tester/Documents/GitHub/TaskLunev/Signal /home/tester/Documents/GitHub/TaskLunev/Signal /home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug /home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug /home/tester/Documents/GitHub/TaskLunev/Signal/cmake-build-debug/CMakeFiles/Signal.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Signal.dir/depend

