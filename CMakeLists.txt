#/*----------------------
#Copyright (C): Henri Payno, Axel Delsol, 
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA
#
#This software is distributed under the terms
#of the GNU Lesser General  Public Licence (LGPL)
#See LICENSE.md for further details
#----------------------*/


cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

# Set C++ 11
set(CMAKE_CXX_STANDARD 11)

# Project name
project("CPOP")
set(CPOP_EXPORT "CPOP")

### define BUILD TYPE if none specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPE)
	message(STATUS "Setting build type as 'Release' as none specified")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Version number (format is MAJOR.MINOR.PATCHLEVEL - e.g. 1.0)
set(VERSION_MAJOR "2")
set(VERSION_MINOR "0")
set(VERSION ${VERSION_MAJOR}.${VERSION_MINOR})

# Compilation options
set(CMAKE_CXX_STANDARD 11)
if(CMAKE_COMPILER_IS_GNUCXX)
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(CMAKE_CXX_FLAGS " -Wno-unused-variable -Wno-unused-result -DG4USE_STD11 -pipe -m64 -fsigned-char -fPIC -pthread -std=c++11")
	endif(CMAKE_BUILD_TYPE STREQUAL "Release")
	
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall  -g -std=c++0x ")	
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

endif(CMAKE_COMPILER_IS_GNUCXX)

# Options
### Geant_4 option
OPTION(WITH_GEANT_4 "Import Geant 4 to be directly complient with G4" ON)
if(WITH_GEANT_4)
	add_definitions(-DWITH_GEANT_4)
	message(STATUS "Import Geant 4 requested")
endif()

### CLHEP option
OPTION(WITH_EXTERNAL_CLHEP "Use an external CLHEP build" ON)
if(WITH_EXTERNAL_CLHEP)
	message(STATUS "Import external CLHEP libraries requested")
endif()

### GDML option
OPTION(WITH_GDML_EXPORT "Unable to be GDML compliant ?" ON)
if(WITH_GDML_EXPORT)
	add_definitions(-DWITH_GDML_EXPORT)
	message(STATUS "GDML export requested")
endif()

### ----------------- Internal option - for CMAKE files Management
OPTION(CPOP_IMPORT_INTERNAL_GDML OFF)
if(WITH_GDML_EXPORT)
	if(NOT WITH_GEANT_4)
		SET(CPOP_IMPORT_INTERNAL_GDML ON)
	endif(NOT WITH_GEANT_4)
endif(WITH_GDML_EXPORT)

### ----------------- set project directories.
add_subdirectory(source)

### TEST option
OPTION(WITH_TEST "Build unitary test" ON)
if(WITH_TEST)
	add_definitions(-DWITH_TEST)
	message(STATUS "Unitary tests requested")
	enable_testing()
	add_subdirectory(test)
endif(WITH_TEST)


# Configuration for user
configure_file (
  "${PROJECT_SOURCE_DIR}/config/CPOP.sh.in"
  "${PROJECT_SOURCE_DIR}/config/CPOP.sh"
)

set(INSTALL_DIRECTORY CPOP/bin)
install(PROGRAMS "config/CPOP.sh"        
         DESTINATION ${INSTALL_DIRECTORY})

#
# CMAKE PACKAGING (for other CMake projects to use this one easily)
# _____________________________________________________________________________

include(PackageConfigurator)
