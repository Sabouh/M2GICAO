# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.2

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
CMAKE_SOURCE_DIR = /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison

# Include any dependencies generated for this target.
include src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/depend.make

# Include the progress variables for this target.
include src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/progress.make

# Include the compile flags for this target's objects.
include src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make

src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp: ../src-nodes/pluginNodes/imgCrop/imgCropWidget.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_imgCropWidget.cpp"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp_parameters

src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp: ../src-nodes/pluginNodes/imgCrop/imgCrop.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating moc_imgCrop.cpp"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/lib/x86_64-linux-gnu/qt5/bin/moc @/home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp_parameters

src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp: ../src-nodes/pluginNodes/imgCrop/imgCrop.vert
src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp: ../src-nodes/pluginNodes/imgCrop/imgCrop.frag
src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp: src-nodes/pluginNodes/imgCrop/imgCrop.qrc.depends
src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp: ../src-nodes/pluginNodes/imgCrop/imgCrop.qrc
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating qrc_imgCrop.cpp"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/lib/x86_64-linux-gnu/qt5/bin/rcc -name imgCrop -o /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCrop.qrc

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o: ../src-nodes/pluginNodes/imgCrop/imgCrop.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imgCrop.dir/imgCrop.cpp.o -c /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCrop.cpp

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgCrop.dir/imgCrop.cpp.i"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCrop.cpp > CMakeFiles/imgCrop.dir/imgCrop.cpp.i

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgCrop.dir/imgCrop.cpp.s"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCrop.cpp -o CMakeFiles/imgCrop.dir/imgCrop.cpp.s

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.requires:
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.provides: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.requires
	$(MAKE) -f src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.provides.build
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.provides

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.provides.build: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o: ../src-nodes/pluginNodes/imgCrop/imgCropWidget.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o -c /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCropWidget.cpp

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgCrop.dir/imgCropWidget.cpp.i"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCropWidget.cpp > CMakeFiles/imgCrop.dir/imgCropWidget.cpp.i

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgCrop.dir/imgCropWidget.cpp.s"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop/imgCropWidget.cpp -o CMakeFiles/imgCrop.dir/imgCropWidget.cpp.s

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.requires:
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.provides: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.requires
	$(MAKE) -f src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.provides.build
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.provides

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.provides.build: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o: src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o -c /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.i"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp > CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.i

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.s"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp -o CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.s

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.requires:
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.provides: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.requires
	$(MAKE) -f src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.provides.build
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.provides

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.provides.build: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o: src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o -c /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.i"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp > CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.i

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.s"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp -o CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.s

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.requires:
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.provides: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.requires
	$(MAKE) -f src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.provides.build
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.provides

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.provides.build: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/flags.make
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o: src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o -c /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.i"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp > CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.i

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.s"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp -o CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.s

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.requires:
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.provides: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.requires
	$(MAKE) -f src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.provides.build
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.provides

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.provides.build: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o

# Object files for target imgCrop
imgCrop_OBJECTS = \
"CMakeFiles/imgCrop.dir/imgCrop.cpp.o" \
"CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o" \
"CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o" \
"CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o" \
"CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o"

# External object files for target imgCrop
imgCrop_EXTERNAL_OBJECTS =

nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build.make
nodes/lib/libimgCrop.so: libgratin.so.0.3.1
nodes/lib/libimgCrop.so: /usr/lib/x86_64-linux-gnu/libQt5Widgets.so.5.4.2
nodes/lib/libimgCrop.so: /usr/lib/x86_64-linux-gnu/libQt5Gui.so.5.4.2
nodes/lib/libimgCrop.so: /usr/lib/x86_64-linux-gnu/libQt5Xml.so.5.4.2
nodes/lib/libimgCrop.so: /usr/lib/x86_64-linux-gnu/libQt5Core.so.5.4.2
nodes/lib/libimgCrop.so: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX shared library ../../../nodes/lib/libimgCrop.so"
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/imgCrop.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build: nodes/lib/libimgCrop.so
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/build

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCrop.cpp.o.requires
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/imgCropWidget.cpp.o.requires
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCropWidget.cpp.o.requires
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/moc_imgCrop.cpp.o.requires
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires: src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/qrc_imgCrop.cpp.o.requires
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/requires

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/clean:
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop && $(CMAKE_COMMAND) -P CMakeFiles/imgCrop.dir/cmake_clean.cmake
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/clean

src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/depend: src-nodes/pluginNodes/imgCrop/moc_imgCropWidget.cpp
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/depend: src-nodes/pluginNodes/imgCrop/moc_imgCrop.cpp
src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/depend: src-nodes/pluginNodes/imgCrop/qrc_imgCrop.cpp
	cd /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1 /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes/imgCrop /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src-nodes/pluginNodes/imgCrop/CMakeFiles/imgCrop.dir/depend

