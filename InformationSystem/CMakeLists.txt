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
# Module : InformationSystem
# Package: CPOP
#
# $Date$ 05/2018
# $Revision$ 0
# $Author$ Delsol Axel
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake) # main (top) cmake dir
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # project specific cmake dir

# Find the QtWidgets library
find_package(Qt5Core)

# include directories
include(InformationSystem)


add_subdirectory(src)
