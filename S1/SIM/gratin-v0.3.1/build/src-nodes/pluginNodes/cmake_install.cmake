# Install script for directory: /home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/src-nodes/pluginNodes

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "CMAKE_CXX_FLAGS_RELEASE")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/fft2D/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/gaussBlur/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/imgResize/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/imgCrop/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/GLSLoader/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/imgLoader/cmake_install.cmake")
  INCLUDE("/home/b/bouhdids/Documents/Git/M2GICAO/S1/SIM/gratin-v0.3.1/build/src-nodes/pluginNodes/poissonGrey/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

