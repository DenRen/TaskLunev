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
CMAKE_SOURCE_DIR = /home/tester/Documents/GitHub/TaskLunev/Proxy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Proxy.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Proxy.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Proxy.dir/flags.make

CMakeFiles/Proxy.dir/main.cpp.o: CMakeFiles/Proxy.dir/flags.make
CMakeFiles/Proxy.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Proxy.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Proxy.dir/main.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Proxy/main.cpp

CMakeFiles/Proxy.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Proxy.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Proxy/main.cpp > CMakeFiles/Proxy.dir/main.cpp.i

CMakeFiles/Proxy.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Proxy.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Proxy/main.cpp -o CMakeFiles/Proxy.dir/main.cpp.s

CMakeFiles/Proxy.dir/AddFunc.cpp.o: CMakeFiles/Proxy.dir/flags.make
CMakeFiles/Proxy.dir/AddFunc.cpp.o: ../AddFunc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Proxy.dir/AddFunc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Proxy.dir/AddFunc.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Proxy/AddFunc.cpp

CMakeFiles/Proxy.dir/AddFunc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Proxy.dir/AddFunc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Proxy/AddFunc.cpp > CMakeFiles/Proxy.dir/AddFunc.cpp.i

CMakeFiles/Proxy.dir/AddFunc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Proxy.dir/AddFunc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Proxy/AddFunc.cpp -o CMakeFiles/Proxy.dir/AddFunc.cpp.s

CMakeFiles/Proxy.dir/DebugFunc.cpp.o: CMakeFiles/Proxy.dir/flags.make
CMakeFiles/Proxy.dir/DebugFunc.cpp.o: ../DebugFunc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Proxy.dir/DebugFunc.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Proxy.dir/DebugFunc.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Proxy/DebugFunc.cpp

CMakeFiles/Proxy.dir/DebugFunc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Proxy.dir/DebugFunc.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Proxy/DebugFunc.cpp > CMakeFiles/Proxy.dir/DebugFunc.cpp.i

CMakeFiles/Proxy.dir/DebugFunc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Proxy.dir/DebugFunc.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Proxy/DebugFunc.cpp -o CMakeFiles/Proxy.dir/DebugFunc.cpp.s

CMakeFiles/Proxy.dir/LocalProxy.cpp.o: CMakeFiles/Proxy.dir/flags.make
CMakeFiles/Proxy.dir/LocalProxy.cpp.o: ../LocalProxy.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Proxy.dir/LocalProxy.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Proxy.dir/LocalProxy.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Proxy/LocalProxy.cpp

CMakeFiles/Proxy.dir/LocalProxy.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Proxy.dir/LocalProxy.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Proxy/LocalProxy.cpp > CMakeFiles/Proxy.dir/LocalProxy.cpp.i

CMakeFiles/Proxy.dir/LocalProxy.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Proxy.dir/LocalProxy.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Proxy/LocalProxy.cpp -o CMakeFiles/Proxy.dir/LocalProxy.cpp.s

CMakeFiles/Proxy.dir/CircBuffer.cpp.o: CMakeFiles/Proxy.dir/flags.make
CMakeFiles/Proxy.dir/CircBuffer.cpp.o: ../CircBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Proxy.dir/CircBuffer.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Proxy.dir/CircBuffer.cpp.o -c /home/tester/Documents/GitHub/TaskLunev/Proxy/CircBuffer.cpp

CMakeFiles/Proxy.dir/CircBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Proxy.dir/CircBuffer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tester/Documents/GitHub/TaskLunev/Proxy/CircBuffer.cpp > CMakeFiles/Proxy.dir/CircBuffer.cpp.i

CMakeFiles/Proxy.dir/CircBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Proxy.dir/CircBuffer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tester/Documents/GitHub/TaskLunev/Proxy/CircBuffer.cpp -o CMakeFiles/Proxy.dir/CircBuffer.cpp.s

# Object files for target Proxy
Proxy_OBJECTS = \
"CMakeFiles/Proxy.dir/main.cpp.o" \
"CMakeFiles/Proxy.dir/AddFunc.cpp.o" \
"CMakeFiles/Proxy.dir/DebugFunc.cpp.o" \
"CMakeFiles/Proxy.dir/LocalProxy.cpp.o" \
"CMakeFiles/Proxy.dir/CircBuffer.cpp.o"

# External object files for target Proxy
Proxy_EXTERNAL_OBJECTS =

Proxy: CMakeFiles/Proxy.dir/main.cpp.o
Proxy: CMakeFiles/Proxy.dir/AddFunc.cpp.o
Proxy: CMakeFiles/Proxy.dir/DebugFunc.cpp.o
Proxy: CMakeFiles/Proxy.dir/LocalProxy.cpp.o
Proxy: CMakeFiles/Proxy.dir/CircBuffer.cpp.o
Proxy: CMakeFiles/Proxy.dir/build.make
Proxy: CMakeFiles/Proxy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable Proxy"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Proxy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Proxy.dir/build: Proxy

.PHONY : CMakeFiles/Proxy.dir/build

CMakeFiles/Proxy.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Proxy.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Proxy.dir/clean

CMakeFiles/Proxy.dir/depend:
	cd /home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tester/Documents/GitHub/TaskLunev/Proxy /home/tester/Documents/GitHub/TaskLunev/Proxy /home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug /home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug /home/tester/Documents/GitHub/TaskLunev/Proxy/cmake-build-debug/CMakeFiles/Proxy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Proxy.dir/depend

