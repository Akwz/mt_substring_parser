cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)


project(substring_parser)

include_directories(
./
utils/
activity/
activity/workers
)

set(SOURCES
main.cpp
activity/activity.cpp
activity/activity_manager.cpp
activity/workers/worker_pool.cpp
)

add_executable(substring_parser ${SOURCES})