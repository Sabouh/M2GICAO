# Install script for directory: /home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/genericNodes/pingPongCustom

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "CMAKE_CXX_FLAGS_RELEASE")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so"
         RPATH "/usr/local/Gratin")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib" TYPE SHARED_LIBRARY FILES "/home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison/nodes/lib/libpingPongCustom.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so"
         OLD_RPATH "/home/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build-maison:"
         NEW_RPATH "/usr/local/Gratin")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/Gratin/nodes/lib/libpingPongCustom.so")
    endif()
  endif()
endif()

