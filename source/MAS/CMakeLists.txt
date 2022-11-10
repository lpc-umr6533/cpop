##########################################################
# Copyright (C): Henri Payno, Axel Delsol, Alexis Pereda #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

## if with Geant4 : should be with external CLHEP : same link to avoid conflict
if(WITH_GEANT_4)
	if(NOT WITH_EXTERNAL_CLHEP)
		message(FATAL_ERROR "you can't use the internal CLHEP if your are using Geant4. You must use the same external CLHEP as used from your Geant4. Please set the WITH_EXTERNAL_CLHEP option and link the CLHEP used with Geant4")
	endif(NOT WITH_EXTERNAL_CLHEP)
endif(WITH_GEANT_4)

# Qt auto MOC
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)
find_package(CGAL REQUIRED)
find_package(OpenGL REQUIRED)

# Link information system
include(InformationSystem)

# Link external CLHEP
if(WITH_EXTERNAL_CLHEP)
	find_package(CLHEP REQUIRED)
else(WITH_EXTERNAL_CLHEP)
	include_directories(Internal_CLHEP)
	include_directories(Internal_CLHEP/CLHEP/Random/include)
	file(GLOB_RECURSE CLHEP_HEADERS Internal_CLHEP/CLHEP/*.hh)
	file(GLOB_RECURSE CLHEP_SOURCES Internal_CLHEP/CLHEP/*.cc)

	source_group("CLHEP\\include" FILES ${CLHEP_HEADERS})
	source_group("CLHEP\\src" 		FILES ${CLHEP_SOURCES})
endif(WITH_EXTERNAL_CLHEP)

include(MAS)

set(HEADERS
	Agent/include/Agent.hh
	Agent/include/AgentStates.hh
	Agent/include/Body.hh
	Agent/include/DynamicAgent.hh
	Agent/include/Movable.hh
	Agent/include/Spatialable.hh
	Agent/include/SpatialableAgent.hh

	ConflictSolver/include/ConflictSolver.hh
	ConflictSolver/include/SpatialConflictSolver.hh

	Layers/include/Dimensioned_Layer.hh
	Layers/include/Layer.hh
	Layers/include/World.hh
	Layers/include/WorldLayer.hh

	Shapes/include/BoundingBox.hh

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

	Settings/include/AgentSettings.hh
	Settings/include/EngineSettings.hh
	Settings/include/GeometrySettings.hh
)

set(SOURCES
	Agent/src/Agent.cc
	Agent/src/Body.cc
	Agent/src/DynamicAgent.cc
	Agent/src/SpatialableAgent.cc

	ConflictSolver/src/ConflictSolver.cc

	Layers/src/Dimensioned_Layer.cc
	Layers/src/Layer.cc
	Layers/src/World.cc
	Layers/src/WorldLayer.cc

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

set(QOBJECTS
	Simulation/include/MASPlatform.hh
	Simulation/include/SimulationManager.hh
	Simulation/include/ThreadAgentGroup.hh
	Simulation/include/ViewerUpdater.hh
)

QT5_WRAP_CPP(QT_MOC_OUT ${QOBJECTS})

### ----------------------------- MAS executable ----------------------------###
if(CMAKE_COMPILER_IS_GNUCXX)
	set(LIBRARY_OUTPUT_PATH "${CPOP_LIBRARIES_DIR}")
endif(CMAKE_COMPILER_IS_GNUCXX)

set(LIBRARY_NAME Platform_SMA)
add_library(${LIBRARY_NAME} ${LIB_TYPE} ${SOURCES} ${CLHEP_SOURCES} ${QT_MOC_OUT})

set(ALL_HEADERS ${HEADERS} ${CLHEP_HEADERS} ${QT_MOC_OUT})

set_target_properties(${LIBRARY_NAME} PROPERTIES
	VERSION ${VERSION} # defined in the main CMakeLists.txt
	FRAMEWORK FALSE
	PUBLIC_HEADER "${ALL_HEADERS}" # specify the public headers
	DEBUG_POSTFIX "_d"
	FOLDER "Source"
)

target_compile_options(${LIBRARY_NAME} PUBLIC -pipe -march=native)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)
target_link_libraries(${LIBRARY_NAME} Qt5::Core CGAL InformationSystem CLHEP::CLHEP ${OPENGL_gl_LIBRARY})

# Targets:
#   * <prefix>/lib/<libraries>
#   * header location after install: <prefix>/include/<project>/*.h
#   * headers can be included by C++ code `#<project>/Bar.hpp>`
install(TARGETS ${LIBRARY_NAME}
	EXPORT ${CPOP_EXPORT}                         # for downstream dependencies
	ARCHIVE DESTINATION CPOP/lib COMPONENT libs   # static lib
	RUNTIME DESTINATION CPOP/bin COMPONENT libs   # binaries
	LIBRARY DESTINATION CPOP/lib COMPONENT libs   # shared lib
	FRAMEWORK DESTINATION CPOP/bin COMPONENT libs # for mac
	PUBLIC_HEADER DESTINATION include/CPOP COMPONENT devel   # headers for mac (note the different component -> different package)
	INCLUDES DESTINATION include/CPOP            # headers
	)
