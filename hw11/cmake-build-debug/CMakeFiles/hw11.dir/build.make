# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.20

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2021.2.2\bin\cmake\win\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\User\OS\hw11

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\User\OS\hw11\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/hw11.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/hw11.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hw11.dir/flags.make

CMakeFiles/hw11.dir/server.c.obj: CMakeFiles/hw11.dir/flags.make
CMakeFiles/hw11.dir/server.c.obj: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\OS\hw11\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/hw11.dir/server.c.obj"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw11.dir\server.c.obj -c C:\Users\User\OS\hw11\server.c

CMakeFiles/hw11.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw11.dir/server.c.i"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\User\OS\hw11\server.c > CMakeFiles\hw11.dir\server.c.i

CMakeFiles/hw11.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw11.dir/server.c.s"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\User\OS\hw11\server.c -o CMakeFiles\hw11.dir\server.c.s

CMakeFiles/hw11.dir/first.c.obj: CMakeFiles/hw11.dir/flags.make
CMakeFiles/hw11.dir/first.c.obj: ../first.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\OS\hw11\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/hw11.dir/first.c.obj"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw11.dir\first.c.obj -c C:\Users\User\OS\hw11\first.c

CMakeFiles/hw11.dir/first.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw11.dir/first.c.i"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\User\OS\hw11\first.c > CMakeFiles\hw11.dir\first.c.i

CMakeFiles/hw11.dir/first.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw11.dir/first.c.s"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\User\OS\hw11\first.c -o CMakeFiles\hw11.dir\first.c.s

CMakeFiles/hw11.dir/second.c.obj: CMakeFiles/hw11.dir/flags.make
CMakeFiles/hw11.dir/second.c.obj: ../second.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\User\OS\hw11\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/hw11.dir/second.c.obj"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\hw11.dir\second.c.obj -c C:\Users\User\OS\hw11\second.c

CMakeFiles/hw11.dir/second.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hw11.dir/second.c.i"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\User\OS\hw11\second.c > CMakeFiles\hw11.dir\second.c.i

CMakeFiles/hw11.dir/second.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hw11.dir/second.c.s"
	C:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\User\OS\hw11\second.c -o CMakeFiles\hw11.dir\second.c.s

# Object files for target hw11
hw11_OBJECTS = \
"CMakeFiles/hw11.dir/server.c.obj" \
"CMakeFiles/hw11.dir/first.c.obj" \
"CMakeFiles/hw11.dir/second.c.obj"

# External object files for target hw11
hw11_EXTERNAL_OBJECTS =

hw11.exe: CMakeFiles/hw11.dir/server.c.obj
hw11.exe: CMakeFiles/hw11.dir/first.c.obj
hw11.exe: CMakeFiles/hw11.dir/second.c.obj
hw11.exe: CMakeFiles/hw11.dir/build.make
hw11.exe: CMakeFiles/hw11.dir/linklibs.rsp
hw11.exe: CMakeFiles/hw11.dir/objects1.rsp
hw11.exe: CMakeFiles/hw11.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\User\OS\hw11\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable hw11.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\hw11.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hw11.dir/build: hw11.exe
.PHONY : CMakeFiles/hw11.dir/build

CMakeFiles/hw11.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\hw11.dir\cmake_clean.cmake
.PHONY : CMakeFiles/hw11.dir/clean

CMakeFiles/hw11.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\User\OS\hw11 C:\Users\User\OS\hw11 C:\Users\User\OS\hw11\cmake-build-debug C:\Users\User\OS\hw11\cmake-build-debug C:\Users\User\OS\hw11\cmake-build-debug\CMakeFiles\hw11.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/hw11.dir/depend

