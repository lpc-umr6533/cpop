##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################
cmake_minimum_required(VERSION 3.7)

project(HomogeneousRadiation)
set(BINARY_NAME homogeneousRadiation)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir

include(cReader)
include(InformationSystem)
include(MAS)
include(Modeler)

# define CPOP_DIR if needed

find_package(CPOP CONFIG REQUIRED)
foreach(USE_FILE ${CPOP_USE_FILE})
	message(STATUS "Including ${USE_FILE}")
	include(${USE_FILE})
endforeach(USE_FILE)

set(ALL_SOURCE
	src/main.cc
	src/DetectorConstruction.cc
	src/DetectorConstructionMessenger.cc
)

set(ALL_HEADER
	include/DetectorConstruction.hh
	include/DetectorConstructionMessenger.hh
)

add_executable(${BINARY_NAME} ${ALL_SOURCE} ${ALL_HEADER})
target_include_directories(${BINARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_options(${BINARY_NAME} PUBLIC -Wall -pthread)
target_link_libraries(${BINARY_NAME} PUBLIC
	${CPOP_LIB}
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
