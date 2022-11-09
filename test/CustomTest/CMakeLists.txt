cmake_minimum_required(VERSION 3.1)

project(FirstCatchTest)

# Set C++ 11
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_SOURCE
	main.cc
	testclass.cc
    test.cc
)

set(PROJECT_HEADER
	testclass.hh
)

add_executable( firstTest ${PROJECT_SOURCE} ${PROJECT_HEADER})
target_include_directories(firstTest PUBLIC ${CATCH_INCLUDE_DIR})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

#Copy a file
add_custom_command(TARGET firstTest POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	   ${CMAKE_CURRENT_SOURCE_DIR}/totoro
	   $<TARGET_FILE_DIR:firstTest>
)

# TESTING
include(CTest)

add_test(NAME FirstCTEST COMMAND firstTest )
set_tests_properties(FirstCTEST PROPERTIES 
	PASS_REGULAR_EXPRESSION "All tests passed"
)
