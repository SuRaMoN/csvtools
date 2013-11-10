
## project setup
cmake_minimum_required(VERSION 2.6)
project(csvtoolstests)

## Add boost
find_package(Boost REQUIRED unit_test_framework system filesystem)
add_definitions(-DBOOST_TEST_DYN_LINK)

## include csvtools source headers
include_directories(../../src)

## add all test sources
file(GLOB_RECURSE test_sources *test.cpp)

## create and link main test executable
add_executable(csvtoolstests main.cpp ${test_sources})
target_link_libraries(csvtoolstests ${Boost_LIBRARIES})

## create a exectuable for each test to avoid rebuilding everything for one test
file(GLOB_RECURSE rel_test_sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *test.cpp)
foreach(test_source ${rel_test_sources})
	STRING(REGEX REPLACE "\\.cpp$" "" test_name ${test_source})
	STRING(REGEX REPLACE "[^a-zA-Z0-9_]" "_" test_name ${test_name})
	add_executable(${test_name} EXCLUDE_FROM_ALL main.cpp ${test_source})
	target_link_libraries(${test_name} ${Boost_LIBRARIES})
	add_custom_target(build_and_run_test_${test_source} ${test_name})
endforeach(test_source)

## add optional capability for precompiled header (linux only)
add_custom_target(pch ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -DBOOST_TEST_DYN_LINK -c stdafx.h -o stdafx.h.gch)

## add test command
enable_testing()
ADD_TEST(NAME csvtoolstests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND csvtoolstests)

