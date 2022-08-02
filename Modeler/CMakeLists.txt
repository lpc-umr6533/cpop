#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : Modeler
# Package: CPOP
#
# $Date$ 05/2018
# $Revision$ 0
# $Author$ Delsol Axel
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

### define BUILD TYPE if none specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPE)
	message(STATUS "Setting build type as 'Release' as none specified")
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()	

include(ExternalDependencies)

set(CMAKE_CXX_STANDARD 11)
if(CMAKE_COMPILER_IS_GNUCXX)
	if (CMAKE_BUILD_TYPE STREQUAL "Release")
		SET(CMAKE_CXX_FLAGS " -Wno-unused-variable -Wno-unused-result -DG4USE_STD11 -pipe -m64 -fsigned-char -fPIC -pthread -std=c++11")
	endif(CMAKE_BUILD_TYPE STREQUAL "Release")
	
	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall  -g -std=c++0x ")	
	endif(CMAKE_BUILD_TYPE STREQUAL "Debug")

endif(CMAKE_COMPILER_IS_GNUCXX)

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

### -------- define sources ----------------------###

### CELL
FILE(GLOB CELL_INCLUDE Models/Cells/include/*.hh)
SOURCE_GROUP("Models\\Cells\\include" 	FILES ${CELL_INCLUDE})
FILE(GLOB CELL_SRC Models/Cells/src/*.cc)
SOURCE_GROUP("Models\\Cells\\src" 		FILES ${CELL_SRC})

### DISTRIBUTION
FILE(GLOB DISTRIBUTION_INCLUDE Models/Distributions/include/*.hh)
SOURCE_GROUP("Models\\Distribution\\include" FILES ${DISTRIBUTION_INCLUDE})
FILE(GLOB DISTRIBUTION_SRC Models/Distributions/src/*.cc)
SOURCE_GROUP("Models\\Distribution\\src" FILES ${DISTRIBUTION_SRC})

### ENGINE
FILE(GLOB ENGINE_INCLUDE Engine/include/*.hh)
SOURCE_GROUP("Engine\\include" FILES ${ENGINE_INCLUDE})
FILE(GLOB ENGINE_SRC Engine/src/*.cc)
SOURCE_GROUP("Engine\\src" FILES ${ENGINE_SRC})

### Environments
FILE(GLOB ENVIRONMENTS_INCLUDE Models/Environments/include/*.hh)
SOURCE_GROUP("Models\\Environment\\include" FILES ${ENVIRONMENTS_INCLUDE})
FILE(GLOB ENVIRONMENTS_SRC Models/Environments/src/*.cc)
SOURCE_GROUP("Models\\Environment\\src" 	FILES ${ENVIRONMENTS_SRC})

### Forces
FILE(GLOB FORCES_INCLUDE Models/Forces/include/*.hh)
SOURCE_GROUP("Models\\Force\\include" FILES ${FORCES_INCLUDE})
FILE(GLOB FORCES_SRC Models/Forces/src/*.cc)
SOURCE_GROUP("Models\\Force\\src" FILES ${FORCES_SRC})

### GDML
if(WITH_GDML_EXPORT)
	FILE(GLOB_RECURSE GDML_INCLUDE IO/GDML/include/*.hh)
	FILE(GLOB_RECURSE GDML_SRC IO/GDML/src/*.cc)
	if(CPOP_IMPORT_INTERNAL_GDML)
		FILE(GLOB_RECURSE GDML_INCLUDE_INTERNAL 
			Engine/G4_sources/*.hh 
			Engine/G4_sources/geometry/management/*.hh
			Engine/G4_sources/persistency/*.hh)
		FILE(GLOB_RECURSE GDML_SRC_INTERNAL 
			${CPOP_MAS_PATH}/*.cc 
			Engine/G4_sources/*.cc 
			Engine/G4_sources/geometry/management/*.cc
			Engine/G4_sources/persistency/*.cc)
		SET(GDML_INCLUDE
			${GDML_INCLUDE}
			${GDML_INCLUDE_INTERNAL}
			)

		SET(GDML_SRC
			${GDML_SRC} 
			${GDML_SRC_INTERNAL})
	else()
	endif(CPOP_IMPORT_INTERNAL_GDML)

	SOURCE_GROUP("IO\\GDML\\include" FILES ${GDML_INCLUDE})
	SOURCE_GROUP("IO\\GDML\\src" FILES ${GDML_SRC})

endif(WITH_GDML_EXPORT)

### GRID
FILE(GLOB GRID_INCLUDE Geometry/Grid/include/*.hh)
SOURCE_GROUP("Grid\\include" FILES ${GRID_INCLUDE})
FILE(GLOB GRID_SRC Geometry/Grid/src/*.cc)
SOURCE_GROUP("Grid\\src" FILES ${GRID_SRC})

### IO
FILE(GLOB IO_INCLUDE IO/include/*.hh)
SOURCE_GROUP("IO\\include" FILES ${IO_INCLUDE})
FILE(GLOB IO_SRC IO/src/*.cc)
SOURCE_GROUP("IO\\src" FILES ${IO_SRC})

### MESH
FILE(GLOB MESH_INCLUDE Geometry/Mesh/include/*.hh)
SOURCE_GROUP("Geometry\\Mesh\\include" FILES ${MESH_INCLUDE})
FILE(GLOB MESH_SRC Geometry/Mesh/src/*.cc)
SOURCE_GROUP("Geometry\\Mesh\\src" FILES ${MESH_SRC})

### MESH SUBDIVISION
FILE(GLOB MESH_SUBDIVISION_INCLUDE Geometry/MeshSubdivision/include/*.hh)
SOURCE_GROUP("Geometry\\MeshSubdivision\\include" FILES ${MESH_SUBDIVISION_INCLUDE})
FILE(GLOB MESH_SUBDIVISION_SRC Geometry/MeshSubdivision/src/*.cc)
SOURCE_GROUP("Geometry\\MeshSubdivision\\src" FILES ${MESH_SUBDIVISION_SRC})

### MATERIAL
FILE(GLOB MATERIAL_INCLUDE Engine/Material/include/*.hh Engine/Material/Nist/include/*.hh)
SOURCE_GROUP("Engine\\Material\\include" FILES ${MATERIAL_INCLUDE})
FILE(GLOB MATERIAL_SRC Engine/Material/src/*.cc Engine/Material/Nist/src/*.cc)
SOURCE_GROUP("Engine\\Material\\src" FILES ${MATERIAL_SRC})

### NUCLEUS
FILE(GLOB NUCLEUS_INCLUDE Models/Nucleus/include/*.hh)
SOURCE_GROUP("Models\\Nucleus\\include" FILES ${NUCLEUS_INCLUDE})
FILE(GLOB NUCLEUS_SRC Models/Nucleus/src/*.cc)
SOURCE_GROUP("Models\\Nucleus\\src" FILES ${NUCLEUS_SRC})

### SETTINGS
FILE(GLOB SETTINGS_INCLUDE Settings/include/*.hh)
SOURCE_GROUP("Settings" FILES ${SETTINGS_INCLUDE})
FILE(GLOB SETTINGS_SRC Settings/src/*.cc)
SOURCE_GROUP("Settings" FILES ${SETTINGS_SRC})

### SHAPES
FILE(GLOB SHAPES_INCLUDE Geometry/Shapes/include/*.hh)
SOURCE_GROUP("Geometry\\Shape\\inclue" FILES ${SHAPES_INCLUDE})
FILE(GLOB SHAPES_SRC Geometry/Shapes/src/*.cc)
SOURCE_GROUP("Geometry\\Shape\\src" FILES ${SHAPES_SRC})

### SPATIAL_DATA_STRUCTURE
FILE(GLOB DATA_STRUCTURE_INCLUDE Geometry/SpatialDataStructures/include/*.hh)
SOURCE_GROUP("Geometry\\SpatialDataStructure\\include" FILES ${DATA_STRUCTURE_INCLUDE})
FILE(GLOB DATA_STRUCTURE_SRC Geometry/SpatialDataStructures/src/*.cc)
SOURCE_GROUP("Geometry\\SpatialDataStructure\\src" FILES ${DATA_STRUCTURE_SRC})

### STATS
FILE(GLOB STATS_INCLUDE Stats/include/*.hh)
SOURCE_GROUP("Stats\\include" FILES ${STATS_INCLUDE})
FILE(GLOB STATS_SRC Stats/src/*.cc)
SOURCE_GROUP("Stats\\src" FILES ${STATS_SRC})

### UTILS
FILE(GLOB_RECURSE UTILS_INCLUDE Utils/*.hh)
SOURCE_GROUP("Utils\\include" FILES ${UTILS_INCLUDE})
FILE(GLOB_RECURSE UTILS_SRC 	Utils/*.cc)
SOURCE_GROUP("Utils\\src" FILES ${UTILS_SRC})

### Geant4 files
if(NOT WITH_GEANT_4)
	# material
	FILE(GLOB_RECURSE G4_MATERIAL Engine/Material/materials/*)
	SOURCE_GROUP("Engine\\Material\\G4_include" FILES ${G4_MATERIAL})
	# global
	FILE(GLOB_RECURSE G4_GLOBAL Engine/global/*)
	SOURCE_GROUP("Engine\\Material\\G4_global" FILES ${G4_GLOBAL})
else()	
	# Primary Generator Action (PGA)
	FILE(GLOB_RECURSE PGA_INCLUDE Physics/PrimaryGeneratorAction/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${PGA_INCLUDE})
	FILE(GLOB_RECURSE PGA_SRC Physics/PrimaryGeneratorAction/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${PGA_SRC})	
	
	#UserSpectrum (US)
	FILE(GLOB_RECURSE US_INCLUDE Physics/UserSpectrum/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${US_INCLUDE})
	FILE(GLOB_RECURSE US_SRC Physics/UserSpectrum/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${US_SRC})
	
	#PhysicsList (PL)
	FILE(GLOB_RECURSE PL_INCLUDE Physics/PhysicsList/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${PL_INCLUDE})
	FILE(GLOB_RECURSE PL_SRC Physics/PhysicsList/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${PL_SRC})
	
	#Process
	FILE(GLOB_RECURSE PROCESS_INCLUDE Physics/Process/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${PROCESS_INCLUDE})
	FILE(GLOB_RECURSE PROCESS_SRC Physics/Process/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${PROCESS_SRC})
	
	#UserAction (UA)
	FILE(GLOB_RECURSE UA_INCLUDE Physics/UserAction/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${UA_INCLUDE})
	FILE(GLOB_RECURSE UA_SRC Physics/UserAction/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${UA_SRC})
	
	#Population
	FILE(GLOB_RECURSE POPULATION_INCLUDE Physics/Population/*.hh)
	SOURCE_GROUP("Physics\\include" FILES ${POPULATION_INCLUDE})
	FILE(GLOB_RECURSE POPULATION_SRC Physics/Population/*.cc)
	SOURCE_GROUP("Physics\\src" FILES ${POPULATION_SRC})
	
endif(NOT WITH_GEANT_4)

#### SET OUTPUT DIR
SET(LIBRARY_OUTPUT_PATH ${CPOP_LIBRARIES_DIR})

### DEAL with QObject
### ------------------ HEADERS ------------------------------
SET(QT_OBJECT_MOC
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

set(ALL_HEADER 
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
  VERSION ${VERSION}  # ${VERSION} was defined in the main CMakeLists.
  FRAMEWORK FALSE
  PUBLIC_HEADER "${ALL_HEADER}" # specify the public headers
)

set_target_properties(Modeler PROPERTIES DEBUG_POSTFIX "_d")


TARGET_LINK_LIBRARIES(${LIBRARY_NAME}
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
	TARGET_LINK_LIBRARIES(Modeler
		${Geant4_LIBRARIES}
	)

endif(WITH_GEANT_4)

### Link external CLHEP ###
if(WITH_EXTERNAL_CLHEP)
	TARGET_LINK_LIBRARIES(Modeler
		CLHEP::CLHEP
	)
endif()

## Set property
SET_PROPERTY(TARGET Modeler PROPERTY FOLDER "Source" )

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
