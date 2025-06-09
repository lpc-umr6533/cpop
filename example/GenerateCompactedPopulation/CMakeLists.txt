cmake_minimum_required(VERSION 3.7)

project(CompactedPopulationGeneration)
set(BINARY_NAME generateCompactedPopulation)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir

include(cReader)
include(InformationSystem)
include(MAS)
include(Modeler)

set(CONFIGFILE_SOURCE
	src/main.cc
	src/simulationEnvironment.cc
)

set(CONFIGFILE_HEADER
	include/unitSection.hh
	include/cellSection.hh
	include/spheroidSection.hh
	include/meshSection.hh
	include/forceSection.hh
	include/simulationSection.hh
	include/simulationEnvironment.hh
)

add_executable(${BINARY_NAME} ${CONFIGFILE_SOURCE} ${CONFIGFILE_HEADER})
target_include_directories(${BINARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(${BINARY_NAME} PUBLIC -Wall -pthread)
target_link_libraries(${BINARY_NAME} PUBLIC Platform_SMA Modeler)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/macros DESTINATION ${CMAKE_BINARY_DIR}/example/GenerateCompactedPopulation)
