cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
	set(CMAKE_CXX_FLAGS -pthread)
endif (UNIX)

project(substring_parser)

include_directories(
./
utils/
activity/
activity/workers
data/
)

set(SOURCES
main.cpp
activity/activity.cpp
activity/activity_manager.cpp
activity/workers/worker_pool.cpp
activity/workers/task_queue.cpp
data/result_container.cpp
)

add_executable(substring_parser ${SOURCES})