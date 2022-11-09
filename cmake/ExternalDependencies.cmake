#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
if(WITH_GDML_EXPORT)
	if(NOT WITH_GEANT_4)
		### ------- Link Xerces ------
		FIND_PACKAGE(Xerces REQUIRED)
		INCLUDE_DIRECTORIES( ${XERCES_INCLUDE_DIR})
		LINK_DIRECTORIES(${XERCES_LIBRARIES})
	endif(NOT WITH_GEANT_4)
endif(WITH_GDML_EXPORT)

### ------- Link Xerces ------
FIND_PACKAGE(XercesC REQUIRED)
INCLUDE_DIRECTORIES( ${XercesC_INCLUDE_DIRS})
#LINK_DIRECTORIES()

### Link CGAL
find_package(CGAL REQUIRED)
include(${CGAL_USE_FILE})

### ------- Link OpenGL ------
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})

### -------- Link Qt ---------
set(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Xml)

### -------- Link Geant4 ----
if(WITH_GEANT_4)
	FIND_PACKAGE(Geant4 REQUIRED)
	include(${Geant4_USE_FILE})
endif()

### Link external CLHEP ###
if(WITH_EXTERNAL_CLHEP)
	FIND_PACKAGE(CLHEP REQUIRED )
endif()


# find ROOT
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})
