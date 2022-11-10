cmake_minimum_required(VERSION 3.7)

project(FirstCatchTest)

set(PROJECT_SOURCE
	main.cc
	testclass.cc
	test.cc
)

set(PROJECT_HEADER
	testclass.hh
)

set(test_name firstTest)

add_executable(${test_name} ${PROJECT_SOURCE} ${PROJECT_HEADER})

target_compile_options(${test_name} PUBLIC -Wall)
target_compile_features(${test_name} PUBLIC cxx_std_17)

add_custom_command(TARGET ${test_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	 ${CMAKE_CURRENT_SOURCE_DIR}/totoro
	 $<TARGET_FILE_DIR:firstTest>
)

include(CTest)
add_test(NAME FirstCTEST COMMAND ${test_name})
set_tests_properties(FirstCTEST PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed")
