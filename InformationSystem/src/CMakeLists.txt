#########################################################
#Copyright (C): Henri Payno, Axel Delsol,         #
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                           #
#This software is distributed under the terms     #
#of the GNU Lesser General  Public Licence (LGPL)   #
#See LICENSE.md for further details           #
#########################################################
# Find the QtWidgets library
find_package(Qt5Core)

#### ----------------- HEADERS ---------------------###
set(InformationSystem_HEADERS
	../include/InformationSystemManager.hh
)

#### ----------------- SOURCES ---------------------###
set(InformationSystem_SOURCES
	InformationSystemManager.cc
)

### DEFINE LIBRARY
set(LIBRARY_NAME InformationSystem)
add_library(${LIBRARY_NAME} ${LIB_TYPE}
	${InformationSystem_SOURCES}
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${InformationSystem_HEADERS}" # specify the public headers
)

set_target_properties(InformationSystem PROPERTIES DEBUG_POSTFIX "_d")

TARGET_LINK_LIBRARIES(InformationSystem
	Qt5::Core
)

## Set property
#SET_PROPERTY(TARGET InformationSystem PROPERTY FOLDER "Source" )

# Says how and where to install software
# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
  EXPORT ${CPOP_EXPORT}            # for downstream dependencies
  ARCHIVE DESTINATION CPOP/lib COMPONENT libs   # static lib
  RUNTIME DESTINATION CPOP/bin COMPONENT libs   # binaries
  LIBRARY DESTINATION CPOP/lib COMPONENT libs   # shared lib
  FRAMEWORK DESTINATION CPOP/bin COMPONENT libs # for mac
  PUBLIC_HEADER DESTINATION include/CPOP COMPONENT devel   # headers for mac (note the different component -> different package)
  INCLUDES DESTINATION include/CPOP            # headers
)
