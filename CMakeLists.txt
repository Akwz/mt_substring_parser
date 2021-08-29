cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_STANDARD 14)

if (UNIX)
	set(CMAKE_CXX_FLAGS -pthread)
endif (UNIX)

project(substring_parser VERSION 1.0)

set(SOURCES
data/shared_storage.cpp
data/result_container.cpp
activity/activity.cpp
activity/activity_manager.cpp
activity/workers/worker_pool.cpp
activity/workers/task_queue.cpp
text_processing/text_storage.cpp
text_processing/text_provider.cpp
text_processing/text_data_view.cpp
text_processing/parsing/text_parser.cpp
text_processing/parsing/single_prefix_mask.cpp
)

set(HEADERS
"./"
"utils/"
"activity/"
"activity/workers"
"data/"
"text_processing/"
"text_processing/parsing"
)

add_executable(substring_parser main.cpp ${SOURCES})

target_include_directories(substring_parser PUBLIC ${HEADERS})

project(parser_test)

set(TEST_SOURCES
test/googletest/src/gtest_main.cc
text_processing/test/text_processor_test.cpp
text_processing/test/test_utils.cpp
)

set(TEST_HEADERS
"test/googletest/include/"
"text_processing/test/"
)

add_executable(parser_test ${SOURCES} ${TEST_SOURCES})

target_include_directories(parser_test PUBLIC ${HEADERS} ${TEST_HEADERS})

add_subdirectory(test test/build)

target_link_libraries(parser_test gtest)