##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################
cmake_minimum_required(VERSION 3.7)

project(PopulationGeneration)
set(BINARY_NAME generatePopulation)

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

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
