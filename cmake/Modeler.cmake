#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
set(MODELER_HEADER_DIR ${CMAKE_SOURCE_DIR}/source/Modeler)
include_directories(${MODELER_HEADER_DIR}/Engine/include)
include_directories(${MODELER_HEADER_DIR}/Engine/Material/include)
include_directories(${MODELER_HEADER_DIR}/Geometry/Grid/include)
include_directories(${MODELER_HEADER_DIR}/Geometry/Mesh/include)
include_directories(${MODELER_HEADER_DIR}/Geometry/MeshSubdivision/include)
include_directories(${MODELER_HEADER_DIR}/Geometry/Shapes/include)
include_directories(${MODELER_HEADER_DIR}/Geometry/SpatialDataStructures/include)
include_directories(${MODELER_HEADER_DIR}/IO/include)
include_directories(${MODELER_HEADER_DIR}/Models/Cells/include)
include_directories(${MODELER_HEADER_DIR}/Models/Distributions/include)
include_directories(${MODELER_HEADER_DIR}/Models/Environments/include)
include_directories(${MODELER_HEADER_DIR}/Models/Forces/include)
include_directories(${MODELER_HEADER_DIR}/Models/Nucleus/include)
include_directories(${MODELER_HEADER_DIR}/Settings/include)
include_directories(${MODELER_HEADER_DIR}/Stats/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Cell/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Cutter/include)
include_directories(${MODELER_HEADER_DIR}/Utils/File/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Geometry/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Graphics/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Grid/include)
include_directories(${MODELER_HEADER_DIR}/Utils/Statistics/include)

### include GDLM exporter
if(WITH_GDML_EXPORT)
	include_directories(${MODELER_HEADER_DIR}/IO/GDML/include)
endif(WITH_GDML_EXPORT)

if(CPOP_IMPORT_INTERNAL_GDML)
	include_directories(${MODELER_HEADER_DIR}/Engine/G4_sources)
	include_directories(${MODELER_HEADER_DIR}/Engine/G4_sources/graphics_reps)
	include_directories(${MODELER_HEADER_DIR}/Engine/G4_sources/geometry/management)
	include_directories(${MODELER_HEADER_DIR}/Engine/G4_sources/geometry/solid)
	include_directories(${MODELER_HEADER_DIR}/Engine/G4_sources/geometry/volume)
	#include_directories(${CPOP_MAS_PATH})
endif(CPOP_IMPORT_INTERNAL_GDML)

if(NOT WITH_GEANT_4)
	include_directories(${MODELER_HEADER_DIR}/Engine/global)
	include_directories(${MODELER_HEADER_DIR}/Engine/Material/materials)
	include_directories(${MODELER_HEADER_DIR}/Engine/Material/materials/Nist/include)
else()
	include_directories(${MODELER_HEADER_DIR}/Physics/PrimaryGeneratorAction/include)
	include_directories(${MODELER_HEADER_DIR}/Physics/UserSpectrum/include)
	include_directories(${MODELER_HEADER_DIR}/Physics/PhysicsList/include)
	include_directories(${MODELER_HEADER_DIR}/Physics/Process/include)
	include_directories(${MODELER_HEADER_DIR}/Physics/UserAction/include)
	include_directories(${MODELER_HEADER_DIR}/Physics/Population/include)
endif(NOT WITH_GEANT_4)
