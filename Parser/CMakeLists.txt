cmake_minimum_required (VERSION 3.0)

project (Parser)

include_directories (include)

aux_source_directory(src SRC_FILES)

add_executable (Parser ${SRC_FILES} main.cpp)
