set(NODE_NAME objCurvature)

find_package(Patate REQUIRED)

if(PATATE_FOUND)
    set(GENERIC_FILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../genericNodes/customNodeFiles)

    file(GLOB header_files ${GENERIC_FILE_PATH}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
    file(GLOB source_files ${GENERIC_FILE_PATH}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    file(GLOB resource_files ${GENERIC_FILE_PATH}/*.qrc ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
    file(GLOB ui_files ${GENERIC_FILE_PATH}/*.ui ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

    include_directories(${PATATE_INCLUDE_DIR})
    include_directories(${GENERIC_FILE_PATH})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    create_node_lib_from_filenames(${NODE_NAME} "${header_files}" "${source_files}" "${resource_files}" "${ui_files}"  ${GRATIN_NODE_LIB_DIR})
endif()

