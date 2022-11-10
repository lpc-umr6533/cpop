##########################################################
# Copyright (C): Axel Delsol, Alexis Pereda              #
# Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA  #
#                                                        #
# This software is distributed under the terms           #
# of the GNU Lesser General  Public Licence (LGPL)       #
# See LICENSE.md for further detais                      #
##########################################################

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

include(ExternalDependencies)

include(InformationSystem)
include(cReader)
include(MAS)
include(Modeler)

# Engine
file(GLOB ENGINE_INCLUDE Engine/include/*.hh)
file(GLOB ENGINE_SRC Engine/src/*.cc)

## Material
file(GLOB MATERIAL_INCLUDE Engine/Material/include/*.hh Engine/Material/Nist/include/*.hh)
file(GLOB MATERIAL_SRC Engine/Material/src/*.cc Engine/Material/Nist/src/*.cc)

# Geometry
## Grid
file(GLOB GRID_INCLUDE Geometry/Grid/include/*.hh)
file(GLOB GRID_SRC Geometry/Grid/src/*.cc)

## Mesh
file(GLOB MESH_INCLUDE Geometry/Mesh/include/*.hh)
file(GLOB MESH_SRC Geometry/Mesh/src/*.cc)

## Mesh subdivision
file(GLOB MESH_SUBDIVISION_INCLUDE Geometry/MeshSubdivision/include/*.hh)
file(GLOB MESH_SUBDIVISION_SRC Geometry/MeshSubdivision/src/*.cc)

### Shapes
file(GLOB SHAPES_INCLUDE Geometry/Shapes/include/*.hh)
file(GLOB SHAPES_SRC Geometry/Shapes/src/*.cc)

### Spatial data structure
file(GLOB DATA_STRUCTURE_INCLUDE Geometry/SpatialDataStructures/include/*.hh)
file(GLOB DATA_STRUCTURE_SRC Geometry/SpatialDataStructures/src/*.cc)

# IO
## GDML
if(WITH_GDML_EXPORT)
	file(GLOB GDML_INCLUDE IO/GDML/include/*.hh)
	file(GLOB GDML_SRC IO/GDML/src/*.cc)
	if(CPOP_IMPORT_INTERNAL_GDML)
		file(GLOB_RECURSE GDML_INCLUDE_INTERNAL Engine/G4_sources/*.hh)
		file(GLOB_RECURSE GDML_SRC_INTERNAL
			${CPOP_MAS_PATH}/*.cc
			Engine/G4_sources/*.cc
			Engine/G4_sources/geometry/management/*.cc
			Engine/G4_sources/persistency/*.cc
		)

		set(GDML_INCLUDE ${GDML_INCLUDE} ${GDML_INCLUDE_INTERNAL})
		set(GDML_SRC ${GDML_SRC} ${GDML_SRC_INTERNAL})
	endif(CPOP_IMPORT_INTERNAL_GDML)
endif(WITH_GDML_EXPORT)

## IO
file(GLOB IO_INCLUDE IO/include/*.hh)
file(GLOB IO_SRC IO/src/*.cc)

# Models
## Cells
file(GLOB CELL_INCLUDE Models/Cells/include/*.hh)
file(GLOB CELL_SRC Models/Cells/src/*.cc)

## Distribution
file(GLOB DISTRIBUTION_INCLUDE Models/Distributions/include/*.hh)
file(GLOB DISTRIBUTION_SRC Models/Distributions/src/*.cc)

## Environments
file(GLOB ENVIRONMENTS_INCLUDE Models/Environments/include/*.hh)
file(GLOB ENVIRONMENTS_SRC Models/Environments/src/*.cc)

## Forces
file(GLOB FORCES_INCLUDE Models/Forces/include/*.hh)
file(GLOB FORCES_SRC Models/Forces/src/*.cc)

### Nucleus
file(GLOB NUCLEUS_INCLUDE Models/Nucleus/include/*.hh)
file(GLOB NUCLEUS_SRC Models/Nucleus/src/*.cc)

# Settings
file(GLOB SETTINGS_INCLUDE Settings/include/*.hh)
file(GLOB SETTINGS_SRC Settings/src/*.cc)

# Stats
file(GLOB STATS_INCLUDE Stats/include/*.hh)
file(GLOB STATS_SRC Stats/src/*.cc)

# Utils
file(GLOB_RECURSE UTILS_INCLUDE Utils/*.hh)
file(GLOB_RECURSE UTILS_SRC 	Utils/*.cc)

### Geant4 files
if(NOT WITH_GEANT_4)
	file(GLOB_RECURSE G4_MATERIAL Engine/Material/materials/*)
	file(GLOB_RECURSE G4_GLOBAL Engine/global/*)
else()
	# Primary Generator Action (PGA)
	file(GLOB_RECURSE PGA_INCLUDE Physics/PrimaryGeneratorAction/*.hh)
	file(GLOB_RECURSE PGA_SRC Physics/PrimaryGeneratorAction/*.cc)

	# UserSpectrum (US)
	file(GLOB_RECURSE US_INCLUDE Physics/UserSpectrum/*.hh)
	file(GLOB_RECURSE US_SRC Physics/UserSpectrum/*.cc)

	# PhysicsList (PL)
	file(GLOB_RECURSE PL_INCLUDE Physics/PhysicsList/*.hh)
	file(GLOB_RECURSE PL_SRC Physics/PhysicsList/*.cc)

	# Process
	file(GLOB_RECURSE PROCESS_INCLUDE Physics/Process/*.hh)
	file(GLOB_RECURSE PROCESS_SRC Physics/Process/*.cc)

	# UserAction (UA)
	file(GLOB_RECURSE UA_INCLUDE Physics/UserAction/*.hh)
	file(GLOB_RECURSE UA_SRC Physics/UserAction/*.cc)

	# Population
	file(GLOB_RECURSE POPULATION_INCLUDE Physics/Population/*.hh)
	file(GLOB_RECURSE POPULATION_SRC Physics/Population/*.cc)
endif(NOT WITH_GEANT_4)

set(LIBRARY_OUTPUT_PATH ${CPOP_LIBRARIES_DIR})

# QObjects
set(QT_OBJECT_MOC
	Geometry/MeshSubdivision/include/RefinementThread.hh
	Geometry/MeshSubdivision/include/DiscoidalCell_MeshSub_Thread.hh
	Geometry/MeshSubdivision/include/SpheroidalCell_MeshSub_Thread.hh
	Geometry/MeshSubdivision/include/Voronoi2DCellMeshSubThread.hh
	Geometry/MeshSubdivision/include/Voronoi3DCellMeshSubThread.hh
)

QT5_WRAP_CPP(QT_MOC_OUT ${QT_OBJECT_MOC})

set(LIBRARY_NAME Modeler)
add_library(${LIBRARY_NAME} ${LIB_TYPE}
	${CELL_SRC}
	${DATA_STRUCTURE_SRC}
	${DISTRIBUTION_SRC}
	${ENGINE_SRC}
	${ENVIRONMENTS_SRC}
	${FORCES_SRC}
	${G4_GLOBAL}
	${GDML_SRC}
	${GRID_SRC}
	${IO_SRC}
	${MATERIAL_SRC}
	${MESH_SRC}
	${MESH_SUBDIVISION_SRC}
	${NUCLEUS_SRC}
	${PGA_SRC}
	${US_SRC}
	${PL_SRC}
	${PROCESS_SRC}
	${UA_SRC}
	${POPULATION_SRC}
	${PHYSICS_UTILS}
	${SETTINGS_SRC}
	${SHAPES_SRC}
	${SOURCES_CC}
	${STATS_SRC}
	${UTILS_SRC}
	${QT_MOC_OUT}
)

set(ALL_HEADERS
	${CELL_INCLUDE}
	${DATA_STRUCTURE_INCLUDE}
	${DISTRIBUTION_INCLUDE}
	${ENGINE_INCLUDE}
	${ENVIRONMENTS_INCLUDE}
	${FORCES_INCLUDE}
	${G4_MATERIAL}
	${GDML_INCLUDE}
	${GRID_INCLUDE}
	${IO_INCLUDE}
	${MATERIAL_INCLUDE}
	${MESH_INCLUDE}
	${MESH_SUBDIVISION_INCLUDE}
	${NUCLEUS_INCLUDE}
	${PGA_INCLUDE}
	${US_INCLUDE}
	${PL_INCLUDE}
	${PROCESS_INCLUDE}
	${UA_INCLUDE}
	${POPULATION_INCLUDE}
	${QT_MOC_OUT}
	${SETTINGS_INCLUDE}
	${SHAPES_INCLUDE}
	${HEADERS_HH}
	${STATS_INCLUDE}
	${UTILS_INCLUDE}
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
  VERSION ${VERSION} # defined in the main CMakeLists.
  FRAMEWORK FALSE
	PUBLIC_HEADER "${ALL_HEADERS}" # specify the public headers
	DEBUG_POSTFIX "_d"
	FOLDER "Source"
)

target_compile_options(${LIBRARY_NAME} PUBLIC -pipe -march=native)
target_compile_features(${LIBRARY_NAME} PUBLIC cxx_std_17)
target_link_libraries(${LIBRARY_NAME}
	CGAL
	Qt5::Core
	Qt5::Xml
	InformationSystem
	Platform_SMA
	${OPENGL_LIBRARIES}
	${XercesC_LIBRARIES}
	${ROOT_LIBRARIES}
	cReader
)

if(WITH_GEANT_4)
	target_link_libraries(${LIBRARY_NAME} ${Geant4_LIBRARIES})
endif(WITH_GEANT_4)

if(WITH_EXTERNAL_CLHEP)
	target_link_libraries(${LIBRARY_NAME} CLHEP::CLHEP)
endif()

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
