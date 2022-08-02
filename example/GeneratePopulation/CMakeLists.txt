#########################################################
#Copyright (C): Henri Payno, Axel Delsol,         #
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                           #
#This software is distributed under the terms     #
#of the GNU Lesser General  Public Licence (LGPL)   #
#See LICENSE.md for further details           #
#########################################################
cmake_minimum_required(VERSION 2.8)
project(PopulationGeneration)

find_package(CPOP CONFIG REQUIRED)
foreach(USE_FILE ${CPOP_USE_FILE})
	message(STATUS "Including ${USE_FILE}")
	include(${USE_FILE})
endforeach(USE_FILE)

### ----------------------------- TEST --------------------------------------###
set(CONFIGFILE_SOURCE
  src/main.cc
  src/simulationEnvironment.cc
)

include_directories(include/)
set(CONFIGFILE_HEADER
  include/unitSection.hh
  include/cellSection.hh
  include/spheroidSection.hh
  include/meshSection.hh
  include/forceSection.hh
  include/simulationSection.hh
  include/simulationEnvironment.hh
)

### ------------------ Executable definition
set(BINARY_NAME generatePopulation)
add_executable(${BINARY_NAME}
  ${CONFIGFILE_SOURCE} ${CONFIGFILE_HEADER}
)

### ------------------ LINK external libraries
TARGET_LINK_LIBRARIES(${BINARY_NAME}
  ${CPOP_LIB}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -Wno-unused-variable")

#Copy data directory for user convenience
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
