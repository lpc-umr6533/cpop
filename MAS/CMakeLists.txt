#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : MAS
# Package: CPOP
#
# $Date$ 05/2018
# $Revision$ 0
# $Author$ Delsol Axel
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

## if with Geant4 : should be with external CLHEP : same link to avoid conflict
if(WITH_GEANT_4)
	if(NOT WITH_EXTERNAL_CLHEP)
	Message(FATAL_ERROR "you can't use the internal CLHEP if your are using Geant4. You must use the same external CLHEP as used from your Geant4. Please set the WITH_EXTERNAL_CLHEP option and link the CLHEP used with Geant4")
	endif(NOT WITH_EXTERNAL_CLHEP)
endif(WITH_GEANT_4)

### Link Qt
set(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core)

### Link CGAL
find_package(CGAL REQUIRED)

### Link OpenGL
find_package(OpenGL)

### Link Information system
include(InformationSystem)

### Link external CLHEP ###
if(WITH_EXTERNAL_CLHEP)
	FIND_PACKAGE(CLHEP REQUIRED )
else(WITH_EXTERNAL_CLHEP)
	include_directories(Internal_CLHEP)
	include_directories(Internal_CLHEP/CLHEP/Random/include)
	FILE(GLOB_RECURSE CLHEP_HEADERS Internal_CLHEP/CLHEP/*.hh)
	FILE(GLOB_RECURSE CLHEP_SOURCES Internal_CLHEP/CLHEP/*.cc)

	source_group("CLHEP\\include" 			FILES ${CLHEP_HEADERS})
	source_group("CLHEP\\src" 				FILES ${CLHEP_SOURCES})

endif(WITH_EXTERNAL_CLHEP)

### Link cc & hh
include(MAS)


### ----------------------------- HEADERS -----------------------------------###
set(PLATFORM_MAS_AGENT_HEADERS
	Agent/include/Agent.hh
	Agent/include/AgentStates.hh
	Agent/include/Body.hh
	Agent/include/DynamicAgent.hh
	Agent/include/Movable.hh
	Agent/include/Spatialable.hh
	Agent/include/SpatialableAgent.hh	
)

set(PLATFORM_MAS_CONFLICT_SOLVER_HEADERS
	ConflictSolver/include/ConflictSolver.hh
	ConflictSolver/include/SpatialConflictSolver.hh
)

set(PLATFORM_MAS_LAYERS_HEADERS
	Layers/include/Dimensioned_Layer.hh
	Layers/include/Layer.hh
	Layers/include/World.hh
	Layers/include/WorldLayer.hh
)

set(PLATFORM_MAS_SHAPES_HEADERS
	Shapes/include/BoundingBox.hh
)

set(PLATFORM_MAS_SIMULATION_HEADERS
	Simulation/include/Action.hh
	Simulation/include/IDManager.hh
	Simulation/include/MASPlatform.hh
	Simulation/include/RandomEngineManager.hh
	Simulation/include/Scheduler.hh
	Simulation/include/SimulationManager.hh
	Simulation/include/SpatialDataStructure.hh
	Simulation/include/SpatialDataStructureManager.hh
	Simulation/include/ThreadAgentGroup.hh
	Simulation/include/ViewerUpdater.hh
)

set(PLATFORM_SETTINGS_HEADERS
	Settings/include/AgentSettings.hh
	Settings/include/EngineSettings.hh
	Settings/include/GeometrySettings.hh
	
)

### ----------------------------- HEADERS -----------------------------------###

set(PLATFORM_MAS_AGENT_SOURCES
	Agent/src/Agent.cc
	Agent/src/Body.cc
	Agent/src/DynamicAgent.cc
	Agent/src/SpatialableAgent.cc	
)

set(PLATFORM_MAS_CONFLICT_SOLVER_SOURCES
	ConflictSolver/src/ConflictSolver.cc
)

set(PLATFORM_MAS_LAYERS_SOURCES
	Layers/src/Dimensioned_Layer.cc
	Layers/src/Layer.cc
	Layers/src/World.cc
	Layers/src/WorldLayer.cc
)

set(PLATFORM_MAS_SIMULATION_SOURCES
	Simulation/src/Action.cc
	Simulation/src/IDManager.cc
	Simulation/src/MASPlatform.cc
	Simulation/src/RandomEngineManager.cc
	Simulation/src/Scheduler.cc
	Simulation/src/SimulationManager.cc
	Simulation/src/SpatialDataStructure.cc
	Simulation/src/SpatialDataStructureManager.cc
	Simulation/src/ThreadAgentGroup.cc
	Simulation/src/ViewerUpdater.cc
)

### ---- MOC QOBJECT

set(PLATFORM_QOBJECTS
	Simulation/include/MASPlatform.hh
	Simulation/include/SimulationManager.hh
	Simulation/include/ThreadAgentGroup.hh
	Simulation/include/ViewerUpdater.hh
)

QT5_WRAP_CPP(QT_MOC_OUT 
	${PLATFORM_QOBJECTS})

## group sources ( for VS )
source_group("Agent\\include" 		FILES ${PLATFORM_MAS_AGENT_HEADERS}  )
source_group("Agent\\src" 			FILES ${PLATFORM_MAS_AGENT_SOURCES} )

source_group("Solver\\include" 		FILES ${PLATFORM_MAS_CONFLICT_SOLVER_HEADERS}  )
source_group("Solver\\src" 			FILES ${PLATFORM_MAS_CONFLICT_SOLVER_SOURCES} )

source_group("Layers\\include" 		FILES ${PLATFORM_MAS_LAYERS_HEADERS}  )
source_group("Layers\\src" 			FILES ${PLATFORM_MAS_LAYERS_SOURCES} )

source_group("Simulation\\include" 	FILES ${PLATFORM_MAS_SIMULATION_HEADERS}  )
source_group("Simulation\\src" 		FILES ${PLATFORM_MAS_SIMULATION_SOURCES} )

source_group("Shapes\\include" 		FILES ${PLATFORM_MAS_SHAPES_HEADERS} )

source_group("Settings"				FILES ${PLATFORM_SETTINGS_HEADERS} )

### ----------------------------- SMA executable ----------------------------###
#### SET OUTPUT DIR
if(CMAKE_COMPILER_IS_GNUCXX)
	SET(LIBRARY_OUTPUT_PATH "${CPOP_LIBRARIES_DIR}")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(LIBRARY_NAME Platform_SMA)
add_library(${LIBRARY_NAME} ${LIB_TYPE}
	${PLATFORM_MAS_AGENT_SOURCES}
	${PLATFORM_MAS_CONFLICT_SOLVER_SOURCES}
	${PLATFORM_MAS_LAYERS_SOURCES}
	${PLATFORM_MAS_SIMULATION_SOURCES}
	${CLHEP_SOURCES}
	${QT_MOC_OUT}
)

set(ALL_HEADER 
	${PLATFORM_MAS_AGENT_HEADERS}
	${PLATFORM_MAS_CONFLICT_SOLVER_HEADERS}
	${PLATFORM_MAS_LAYERS_HEADERS}
	${PLATFORM_MAS_SHAPES_HEADERS}
	${PLATFORM_MAS_SIMULATION_HEADERS}
	${PLATFORM_SETTINGS_HEADERS}
	${CLHEP_HEADERS}
	${QT_MOC_OUT}
)
	

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${ALL_HEADER}" # specify the public headers
)


set_target_properties(Platform_SMA PROPERTIES DEBUG_POSTFIX "_d")

TARGET_LINK_LIBRARIES(Platform_SMA
	Qt5::Core
	CGAL
	InformationSystem
	CLHEP::CLHEP
	${OPENGL_gl_LIBRARY}
 )

if(CMAKE_COMPILER_IS_GNUCXX)
	LINK_LIBRARIES(Platform_SMA
		${INFORMATION_SYSTEM_LIBRARY}
	)
endif(CMAKE_COMPILER_IS_GNUCXX)

## Set property
SET_PROPERTY(TARGET Platform_SMA PROPERTY FOLDER "Source" )


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
