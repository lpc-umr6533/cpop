cmake_minimum_required(VERSION 3.7)

project(cReaderTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir

include(InformationSystem)
include(cReader)
include(MAS)
include(Modeler)

set(PROJECT_SOURCE
	main.cc
	customobject.cc
	test.cc
)

set(PROJECT_HEADER
	customsection.hh
	tests.hh
)

set(test_name cReaderTest)

add_executable(${test_name} ${PROJECT_SOURCE} ${PROJECT_HEADER})

target_compile_options(${test_name} PUBLIC -Wall -pthread)
target_compile_features(${test_name} PUBLIC cxx_std_17)
target_link_libraries(${test_name} cReader pthread)

add_custom_command(TARGET ${test_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_CURRENT_SOURCE_DIR}/testcheck.cfg
	$<TARGET_FILE_DIR:cReaderTest>
)

include(CTest)

add_test(NAME cReaderCTest COMMAND ${test_name})
set_tests_properties(cReaderCTest PROPERTIES PASS_REGULAR_EXPRESSION "All tests passed")
