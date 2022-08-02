#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
# CMakeLists.txt
#
# Template project example
#
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

project(ComplexRadiation)
set(BINARY_NAME targetedAlphaTherapy)

# define CPOP_DIR if needed

find_package(CPOP CONFIG REQUIRED)
foreach(USE_FILE ${CPOP_USE_FILE})
	message(STATUS "Including ${USE_FILE}")
	include(${USE_FILE})
endforeach(USE_FILE)

### ----------------------------- SOURCE  --------------------------------------###
set(ALL_SOURCE 
	src/main.cc
	src/DetectorConstruction.cc
	src/DetectorConstructionMessenger.cc
)

include_directories(include/)
set(ALL_HEADER
    include/DetectorConstruction.hh
    include/DetectorConstructionMessenger.hh
)

### ------------------ Executable definition
add_executable(${BINARY_NAME}
  ${ALL_SOURCE} ${ALL_HEADER}
)

### ------------------ LINK external libraries
TARGET_LINK_LIBRARIES(${BINARY_NAME}
  ${CPOP_LIB}
)


#Copy data directory for user convenience
file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -Wno-unused-variable")
