cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)


project(substring_parser)

include_directories(
./
data/
)

set(SOURCES
main.cpp
data/data_unit.cpp
data/shared_storage.cpp
)

add_executable(substring_parser ${SOURCES})