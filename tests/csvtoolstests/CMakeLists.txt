
## project setup
cmake_minimum_required(VERSION 2.6)
project(csvtoolstests)

## Add boost
find_package(Boost REQUIRED unit_test_framework system filesystem locale)
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

## enable all warnings (linux only, sry)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -W -Wno-long-long -pedantic")
endif()

## add optional capability for precompiled header (linux only, sry)
if(CMAKE_COMPILER_IS_GNUCXX)
	STRING(TOUPPER "CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}" _flags_var_name)
	SET(_compile_flags ${${_flags_var_name}} )
	file(GLOB_RECURSE pch_headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *test_deps.hpp)
	set(pch_objects "")
	foreach(pch_header ${pch_headers})
		add_custom_command(
			OUTPUT ${pch_header}.gch
			MAIN_DEPENDENCY ${pch_header} DEPENDS csvtools_deps.hpp
			COMMAND ${CMAKE_CXX_COMPILER} ${_compile_flags} -DBOOST_TEST_DYN_LINK -c ${pch_header} -o ${pch_header}.gch
		)
		list(APPEND pch_objects ${pch_header}.gch)
	endforeach(pch_header)
	add_custom_target(pch DEPENDS ${pch_objects})
endif(CMAKE_COMPILER_IS_GNUCXX)

## add test command
enable_testing()
ADD_TEST(NAME csvtoolstests WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMAND csvtoolstests --log_level=all)

