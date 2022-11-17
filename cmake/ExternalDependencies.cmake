##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################
if(WITH_GDML_EXPORT)
	if(NOT WITH_GEANT_4)
		### ------- Link Xerces ------
		find_package(Xerces REQUIRED)
		include_directories( ${XERCES_INCLUDE_DIR})
		link_directories(${XERCES_LIBRARIES})
	endif(NOT WITH_GEANT_4)
endif(WITH_GDML_EXPORT)

### ------- Link Xerces ------
find_package(XercesC REQUIRED)
include_directories( ${XercesC_INCLUDE_DIRS})
#LINK_DIRECTORIES()

### Link CGAL
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

### ------- Link OpenGL ------
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

### -------- Link Qt ---------
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core)
find_package(Qt5Xml)

### -------- Link Geant4 ----
if(WITH_GEANT_4)
	find_package(Geant4 REQUIRED)
	include(${Geant4_USE_FILE})
endif()

### Link external CLHEP ###
if(WITH_EXTERNAL_CLHEP)
	find_package(CLHEP REQUIRED )
endif()

# find ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
