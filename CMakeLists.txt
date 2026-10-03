cmake_minimum_required(VERSION 3.10)
project(DBMS_Project CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(dbms 
    src/main.cpp
    src/parser/query_parser.cpp
    src/storage/file_manager.cpp
)

target_include_directories(dbms PRIVATE src)