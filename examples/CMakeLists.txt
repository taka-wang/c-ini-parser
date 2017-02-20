include_directories (${IniParser_SOURCE_DIR}/src)
link_directories (${IniParser_SOURCE_DIR}/src)
add_executable (mytest test.c)
target_link_libraries (mytest iniparser)
# copy sample ini to build folder
configure_file(${IniParser_SOURCE_DIR}/examples/test.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)