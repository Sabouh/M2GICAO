# ********************************
# ********* libgratin-src ********
# ********************************

# all header files 
file(GLOB GRATIN_ALL_HEADERS misc/*.h core/*.h view/*.h)

# all source files 
file(GLOB GRATIN_SOURCES misc/*.cpp core/*.cpp view/*.cpp)

# all resource files 
set(GRATIN_RESOURCES src.qrc)

# find all qt object files in the header list
find_qt_object_files("${GRATIN_ALL_HEADERS}" GRATIN_HEADERS)

# use qt moc conversion
qt5_wrap_cpp(GRATIN_HEADERS_MOC ${GRATIN_HEADERS})

# use qt resource conversion
qt5_add_resources(GRATIN_RESOURCES_RCC ${GRATIN_RESOURCES})

# output library directory
set(LIBRARY_OUTPUT_PATH ${GRATIN_LIB_DIR})

# build shared libs
set(BUILD_SHARED_LIBS ON)
add_definitions(-DLIBRARY)

# create the library (using lib-name and all associated source-files)
add_library(${GRATIN_LIB} ${GRATIN_SOURCES} ${GRATIN_HEADERS_MOC} ${GRATIN_RESOURCES_RCC}) 
set(${GRATIN_LIB}_VERSION_MAJOR ${GRATIN_MAJOR_VERSION})
set(${GRATIN_LIB}_VERSION_MINOR ${GRATIN_MINOR_VERSION})
set(${GRATIN_LIB}_VERSION_PATCH ${GRATIN_PATCH_VERSION})
set(${GRATIN_LIB}_VERSION_STRING ${${GRATIN_LIB}_VERSION_MAJOR}.${${GRATIN_LIB}_VERSION_MINOR}.${${GRATIN_LIB}_VERSION_PATCH})
set_target_properties(${GRATIN_LIB} PROPERTIES VERSION ${${GRATIN_LIB}_VERSION_STRING} SOVERSION ${${GRATIN_LIB}_VERSION_MAJOR})

# link with external libraries
link_to_default_libs(${GRATIN_LIB})

# installation rules 
install(TARGETS ${GRATIN_LIB} DESTINATION ${INSTALL_RUNTIME_DIR})

