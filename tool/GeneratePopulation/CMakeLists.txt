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

set(BINARY_NAME generateCellPop)

include(ExternalDependencies)
include(InformationSystem)
include(cReader)
include(MAS)
include(Modeler)

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

add_executable(${BINARY_NAME} ${CONFIGFILE_SOURCE} ${CONFIGFILE_HEADER})

target_link_libraries(${BINARY_NAME} PUBLIC
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
	target_link_libraries(${BINARY_NAME} ${Geant4_LIBRARIES})
endif(WITH_GEANT_4)

if(WITH_EXTERNAL_CLHEP)
	target_link_libraries(${BINARY_NAME} CLHEP::CLHEP)
endif()

install(TARGETS ${BINARY_NAME} DESTINATION CPOP/bin)
