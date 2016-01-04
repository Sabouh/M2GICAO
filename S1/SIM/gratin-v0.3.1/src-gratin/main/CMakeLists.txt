# ********************************
# ************ gratin ************
# ********************************

# output executable directory
set(EXECUTABLE_OUTPUT_PATH ${GRATIN_EXE_DIR})

# include the src path 
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# include all default paths
add_default_paths()

set(srcs main.cpp)

# Handle Mac OS X specific icon
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE "gratin.icns")
    set(MACOSX_BUNDLE_ICON "${PROJECT_SOURCE_DIR}/resources/${MACOSX_BUNDLE_ICON_FILE}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${GRATIN_MAJOR_VERSION}.${GRATIN_MINOR_VERSION}.${GRATIN_PATCH_VERSION}")
    list(APPEND srcs ${MACOSX_BUNDLE_ICON})
    set_source_files_properties(${MACOSX_BUNDLE_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif()

# create the executable (one single cpp file here)
add_executable(${GRATIN_EXE} MACOSX_BUNDLE ${srcs})
set(${GRATIN_EXE}_VERSION_MAJOR ${GRATIN_MAJOR_VERSION})
set(${GRATIN_EXE}_VERSION_MINOR ${GRATIN_MINOR_VERSION})
set(${GRATIN_EXE}_VERSION_PATCH ${GRATIN_PATCH_VERSION})

set_target_properties(${GRATIN_EXE} PROPERTIES OUTPUT_NAME ${PROJECT_NAME_LOWER})

# link with libraries
link_to_default_libs(${GRATIN_EXE})
link_to_gratin_lib(${GRATIN_EXE})

# post-build (copy needed files)
add_custom_command(TARGET ${GRATIN_EXE}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${GRATIN_BIN_PATH}/nodes/custom
  COMMAND ${CMAKE_COMMAND} -E make_directory ${GRATIN_BIN_PATH}/data
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GRATIN_SRC_PATH}/nodes/custom ${GRATIN_BIN_PATH}/nodes/custom/.
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${GRATIN_SRC_PATH}/data ${GRATIN_BIN_PATH}/data/.
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRATIN_SRC_PATH}/guide.pdf ${GRATIN_BIN_PATH}/.
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GRATIN_SRC_PATH}/LICENCE.MPL2 ${GRATIN_BIN_PATH}/.
  )

# installation rules
install(TARGETS ${GRATIN_EXE} RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} BUNDLE DESTINATION . )
install(DIRECTORY ${GRATIN_BIN_PATH}/data DESTINATION ${INSTALL_RUNTIME_DIR} PATTERN .svn EXCLUDE)
install(DIRECTORY ${GRATIN_BIN_PATH}/nodes DESTINATION ${INSTALL_RUNTIME_DIR} PATTERN .svn EXCLUDE)
install(FILES ${GRATIN_BIN_PATH}/guide.pdf DESTINATION ${INSTALL_RUNTIME_DIR})
install(FILES ${GRATIN_BIN_PATH}/LICENCE.MPL2 DESTINATION ${INSTALL_RUNTIME_DIR})
