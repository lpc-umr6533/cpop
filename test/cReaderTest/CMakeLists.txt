cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir

project(cReaderTest)

### Link Information system
include(InformationSystem)
#include_directories(../InformationSystem/include)

### Link cReader
include(cReader)
#include_directories(../cReader/include)

### -------- Link MAS ----
include(MAS)

### ------  LINK MODELER  --------
include(Modeler)

# Set C++ 11
set(CMAKE_CXX_STANDARD 11)

set(PROJECT_SOURCE
    main.cc
    customobject.cc
    test.cc
    )

set(PROJECT_HEADER
    customsection.hh
    tests.hh
    )

add_executable( cReaderTest ${PROJECT_SOURCE} ${PROJECT_HEADER})
target_link_libraries(cReaderTest cReader pthread)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pthread")

#Copy a file
add_custom_command(TARGET cReaderTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/testcheck.cfg
    $<TARGET_FILE_DIR:cReaderTest>
    )

# TESTING
include(CTest)

add_test(NAME cReaderCTest COMMAND cReaderTest )
set_tests_properties(cReaderCTest PROPERTIES 
    PASS_REGULAR_EXPRESSION "All tests passed"
    )
