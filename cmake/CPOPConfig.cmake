##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################
# cReader-config.cmake - Package configuration file

include(CMakeFindDependencyMacro)

# required for InformationSystem, MAS and Modeler
find_dependency(Qt5Core)

# required for MAS and modeler
find_dependency(CGAL)
find_dependency(CLHEP)
find_dependency(XercesC)

#required for Modeler
find_dependency(Geant4)
find_dependency(Qt5Xml)
find_dependency(ROOT)

get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/CPOP.cmake)

set(CPOP_USE_FILE ${CGAL_USE_FILE} ${ROOT_USE_FILE} ${Geant4_USE_FILE})
set(CPOP_LIB
	${Geant4_LIBRARIES}
	${ROOT_LIBRARIES}
	InformationSystem 
	cReader 
	Platform_SMA Modeler
)
