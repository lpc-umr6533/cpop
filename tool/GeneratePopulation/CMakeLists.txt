#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
cmake_minimum_required(VERSION 2.8)
project(PopulationGeneration)

set(BINARY_NAME generateCellPop)

include(ExternalDependencies)

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

# Compilation options
set(CMAKE_CXX_STANDARD 17)
if(CMAKE_COMPILER_IS_GNUCXX)
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(CMAKE_CXX_FLAGS " -Wno-unused-variable -Wno-unused-result -DG4USE_STD11 -pipe -m64 -fsigned-char -fPIC -pthread")
	endif(CMAKE_BUILD_TYPE STREQUAL "Release")
	
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")	
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

endif(CMAKE_COMPILER_IS_GNUCXX)

set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/CPOP/lib)

### ----------------------------- TEST --------------------------------------###
set(CONFIGFILE_SOURCE
  main.cc
  simulationEnvironment.cc
)

set(CONFIGFILE_HEADER
  unitSection.hh
  cellSection.hh
  spheroidSection.hh
  meshSection.hh
  forceSection.hh
  simulationSection.hh
  simulationEnvironment.hh
)

### ------------------ Executable definition
add_executable(${BINARY_NAME}
  ${CONFIGFILE_SOURCE} ${CONFIGFILE_HEADER}
)

### ------------------ LINK external libraries
TARGET_LINK_LIBRARIES(${BINARY_NAME}
	CGAL
	Qt5::Core
	Qt5::Xml
	InformationSystem
	Platform_SMA
	Modeler
	${OPENGL_LIBRARIES}
	${XercesC_LIBRARIES}
	${ROOT_LIBRARIES}
	cReader
)

if(WITH_GEANT_4)
	TARGET_LINK_LIBRARIES(${BINARY_NAME}
		${Geant4_LIBRARIES}
	)

endif(WITH_GEANT_4)

### Link external CLHEP ###
if(WITH_EXTERNAL_CLHEP)
	TARGET_LINK_LIBRARIES(${BINARY_NAME}
		CLHEP::CLHEP
	)
endif()

## Compiler flags
install(TARGETS ${BINARY_NAME}        
         DESTINATION CPOP/bin)
