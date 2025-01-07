# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/marco/workspace/libretro_shader_viewer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/marco/workspace/libretro_shader_viewer/src

# Include any dependencies generated for this target.
include CMakeFiles/pixel_shader_viewer.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pixel_shader_viewer.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pixel_shader_viewer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pixel_shader_viewer.dir/flags.make

CMakeFiles/pixel_shader_viewer.dir/main.cpp.o: CMakeFiles/pixel_shader_viewer.dir/flags.make
CMakeFiles/pixel_shader_viewer.dir/main.cpp.o: main.cpp
CMakeFiles/pixel_shader_viewer.dir/main.cpp.o: CMakeFiles/pixel_shader_viewer.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/marco/workspace/libretro_shader_viewer/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/pixel_shader_viewer.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/pixel_shader_viewer.dir/main.cpp.o -MF CMakeFiles/pixel_shader_viewer.dir/main.cpp.o.d -o CMakeFiles/pixel_shader_viewer.dir/main.cpp.o -c /home/marco/workspace/libretro_shader_viewer/src/main.cpp

CMakeFiles/pixel_shader_viewer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/pixel_shader_viewer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/marco/workspace/libretro_shader_viewer/src/main.cpp > CMakeFiles/pixel_shader_viewer.dir/main.cpp.i

CMakeFiles/pixel_shader_viewer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/pixel_shader_viewer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/marco/workspace/libretro_shader_viewer/src/main.cpp -o CMakeFiles/pixel_shader_viewer.dir/main.cpp.s

# Object files for target pixel_shader_viewer
pixel_shader_viewer_OBJECTS = \
"CMakeFiles/pixel_shader_viewer.dir/main.cpp.o"

# External object files for target pixel_shader_viewer
pixel_shader_viewer_EXTERNAL_OBJECTS =

pixel_shader_viewer: CMakeFiles/pixel_shader_viewer.dir/main.cpp.o
pixel_shader_viewer: CMakeFiles/pixel_shader_viewer.dir/build.make
pixel_shader_viewer: external/glad/libglad.a
pixel_shader_viewer: external/imgui/libimgui.a
pixel_shader_viewer: external/nfd/libnfd.a
pixel_shader_viewer: /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
pixel_shader_viewer: /usr/lib/x86_64-linux-gnu/libGL.so
pixel_shader_viewer: CMakeFiles/pixel_shader_viewer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/marco/workspace/libretro_shader_viewer/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable pixel_shader_viewer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pixel_shader_viewer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pixel_shader_viewer.dir/build: pixel_shader_viewer
.PHONY : CMakeFiles/pixel_shader_viewer.dir/build

CMakeFiles/pixel_shader_viewer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pixel_shader_viewer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pixel_shader_viewer.dir/clean

CMakeFiles/pixel_shader_viewer.dir/depend:
	cd /home/marco/workspace/libretro_shader_viewer/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/marco/workspace/libretro_shader_viewer /home/marco/workspace/libretro_shader_viewer /home/marco/workspace/libretro_shader_viewer/src /home/marco/workspace/libretro_shader_viewer/src /home/marco/workspace/libretro_shader_viewer/src/CMakeFiles/pixel_shader_viewer.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/pixel_shader_viewer.dir/depend

