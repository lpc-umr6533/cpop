#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
This example shows you how to apply radiation exposure on a cell population

########################################################################
How to build ?

1. source path/to/CPOP-install/CPOP.sh
2. mkdir build
3. cd build
4. cmake .. (if you use ccmake, you can safely ignore CMAKE_INSTALL_PREFIX)
5. make


########################################################################
How to use ?

1. source path/to/geant4/gean4.sh
2. ./homogeneousRadiation -m data/run.mac 
3. If you are using G4MULTITHREADED, merge your root outputs with
   hadd output.root output_t{0..N}.root
   Example : if you used 4 thread -> hadd result.root output_t{0..3}.root
	
########################################################################
Help :

Usage: homogeneousRadiation  [-t <int>] -m=<file>

  Required options:
  -m, --macro=file          macro file(default: input_filename.mac)

  Optional options:
  -t, --thread=int          number of threads(only available with G4 multithread option)
