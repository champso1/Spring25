# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build

# Include any dependencies generated for this target.
include Source/Core/CMakeFiles/ColSim_Core.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Source/Core/CMakeFiles/ColSim_Core.dir/compiler_depend.make

# Include the progress variables for this target.
include Source/Core/CMakeFiles/ColSim_Core.dir/progress.make

# Include the compile flags for this target's objects.
include Source/Core/CMakeFiles/ColSim_Core.dir/flags.make

Source/Core/CMakeFiles/ColSim_Core.dir/codegen:
.PHONY : Source/Core/CMakeFiles/ColSim_Core.dir/codegen

Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/flags.make
Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o: /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Logger.cpp
Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o -MF CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o.d -o CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o -c /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Logger.cpp

Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ColSim_Core.dir/src/Logger.cpp.i"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Logger.cpp > CMakeFiles/ColSim_Core.dir/src/Logger.cpp.i

Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ColSim_Core.dir/src/Logger.cpp.s"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Logger.cpp -o CMakeFiles/ColSim_Core.dir/src/Logger.cpp.s

Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/flags.make
Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o: /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Settings.cpp
Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o -MF CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o.d -o CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o -c /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Settings.cpp

Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ColSim_Core.dir/src/Settings.cpp.i"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Settings.cpp > CMakeFiles/ColSim_Core.dir/src/Settings.cpp.i

Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ColSim_Core.dir/src/Settings.cpp.s"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Settings.cpp -o CMakeFiles/ColSim_Core.dir/src/Settings.cpp.s

Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/flags.make
Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o: /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Utils.cpp
Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o: Source/Core/CMakeFiles/ColSim_Core.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o -MF CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o.d -o CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o -c /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Utils.cpp

Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ColSim_Core.dir/src/Utils.cpp.i"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Utils.cpp > CMakeFiles/ColSim_Core.dir/src/Utils.cpp.i

Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ColSim_Core.dir/src/Utils.cpp.s"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core/src/Utils.cpp -o CMakeFiles/ColSim_Core.dir/src/Utils.cpp.s

# Object files for target ColSim_Core
ColSim_Core_OBJECTS = \
"CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o" \
"CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o" \
"CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o"

# External object files for target ColSim_Core
ColSim_Core_EXTERNAL_OBJECTS =

Source/Core/libColSim_Core.a: Source/Core/CMakeFiles/ColSim_Core.dir/src/Logger.cpp.o
Source/Core/libColSim_Core.a: Source/Core/CMakeFiles/ColSim_Core.dir/src/Settings.cpp.o
Source/Core/libColSim_Core.a: Source/Core/CMakeFiles/ColSim_Core.dir/src/Utils.cpp.o
Source/Core/libColSim_Core.a: Source/Core/CMakeFiles/ColSim_Core.dir/build.make
Source/Core/libColSim_Core.a: Source/Core/CMakeFiles/ColSim_Core.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library libColSim_Core.a"
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && $(CMAKE_COMMAND) -P CMakeFiles/ColSim_Core.dir/cmake_clean_target.cmake
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ColSim_Core.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Source/Core/CMakeFiles/ColSim_Core.dir/build: Source/Core/libColSim_Core.a
.PHONY : Source/Core/CMakeFiles/ColSim_Core.dir/build

Source/Core/CMakeFiles/ColSim_Core.dir/clean:
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core && $(CMAKE_COMMAND) -P CMakeFiles/ColSim_Core.dir/cmake_clean.cmake
.PHONY : Source/Core/CMakeFiles/ColSim_Core.dir/clean

Source/Core/CMakeFiles/ColSim_Core.dir/depend:
	cd /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/Source/Core /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core /home/champson/Dropbox/School/Spring25/CS4632_ModSim/Programs/ColSim/build/Source/Core/CMakeFiles/ColSim_Core.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : Source/Core/CMakeFiles/ColSim_Core.dir/depend

