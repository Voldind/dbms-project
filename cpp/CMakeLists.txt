cmake_minimum_required(VERSION 3.10)
project(DBMS_Project CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_executable(dbms 
    src/main.cpp
    src/parser/query_parser.cpp
    src/storage/database.cpp
    src/storage/file_manager.cpp
)

target_include_directories(dbms PRIVATE src)

set_target_properties(dbms PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin"
)