#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
set(MAS_HEADER_DIR ${CMAKE_SOURCE_DIR}/source/MAS)
include_directories(${MAS_HEADER_DIR}/Agent/include)
include_directories(${MAS_HEADER_DIR}/Agent/src)			# Needed for implementation of templates inside cpp files.
include_directories(${MAS_HEADER_DIR}/ConflictSolver/include)
include_directories(${MAS_HEADER_DIR}/Layers/include)
include_directories(${MAS_HEADER_DIR}/Layers/src)			# Needed for implementation of templates inside cpp files.
include_directories(${MAS_HEADER_DIR}/Settings/include)
include_directories(${MAS_HEADER_DIR}/Shapes/include)
include_directories(${MAS_HEADER_DIR}/Simulation/include)
include_directories(${MAS_HEADER_DIR}/Simulation/src)		# Needed for implementation of templates inside cpp files.
