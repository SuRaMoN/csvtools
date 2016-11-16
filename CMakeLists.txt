## project setup
cmake_minimum_required(VERSION 3.0)
project(csvtools)

## Add required libraries
find_package(Boost REQUIRED unit_test_framework system filesystem locale)

## Search for sources
file(GLOB_RECURSE test_sources tests/*.cpp)

## Create library
add_library(csvtools INTERFACE)

## Create executable
add_executable(csvtoolstests tests/csvtoolstests/main.cpp ${test_sources})

# Link libraries and include sources
target_include_directories(csvtools INTERFACE src ${Boost_INCLUDE_DIR})
target_link_libraries(csvtools INTERFACE ${Boost_LIBRARIES})
target_link_libraries(csvtoolstests ${Boost_LIBRARIES} csvtools)

# Configure tests
target_compile_definitions(csvtoolstests PRIVATE BOOST_TEST_DYN_LINK=1)

## add test command
enable_testing()
ADD_TEST(NAME csvtoolstests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests/csvtoolstests COMMAND csvtoolstests --log_level=all)
