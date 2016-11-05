cmake_minimum_required(VERSION 3.0)
project(csvtools)

find_package(Boost REQUIRED unit_test_framework system filesystem locale)

add_library(csvtools INTERFACE)
target_include_directories(csvtools INTERFACE "src")
target_link_libraries(csvtools INTERFACE ${Boost_LIBRARIES})
target_include_directories(csvtools INTERFACE ${Boost_INCLUDE_DIR})
