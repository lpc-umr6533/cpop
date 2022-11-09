#------------------------------------------------------------------------------
# CMakeLists.txt
# Module : 
# Package: CPOP
#
# Intermediate level CMakeLists.txt - just process subdirectories
# $Date$ 05/2018
# $Revision$ 0
# $Author$ Delsol Axel
#------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(InformationSystem)
add_subdirectory(cReader)
add_subdirectory(MAS)
add_subdirectory(Modeler)
