cmake_minimum_required (VERSION 3.0)

project (Lexer)

include_directories (include)

aux_source_directory(src SRC_FILES)

add_executable (Lexer ${SRC_FILES} main.cpp)
