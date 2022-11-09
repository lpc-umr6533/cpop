#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
This example shows you how to irradiate 

########################################################################
How to build ?

1. source path/to/CPOP-install/CPOP.sh
2. mkdir build
3. cd build
4. cmake .. (if you use ccmake, you can safely ignore CMAKE_INSTALL_PREFIX)
5. make

#######################################################################
Options

- 3 geometries are available : a 95 µm radius spheroid with 3 degrees of compaction (25, 50 and 75%)

- Number of particles is initially set at 1 particle per cell, can be changed

- Emission site of particles in cells can be changed (membrane, cytoplasm, homogeneous or nucleus only)

- % of cells labeled with particles

- Diffusion of radionuclide's daughter after fixation (only for At-211 for now)



Simulation initialization can be a bit faster if .gdml writing is deactivated in SpheroidalCellMesh.cc

########################################################################
How to use ?

1. source path/to/geant4/gean4.sh
2. ./complexRadiation -m data/run.mac 
3. If you are using G4MULTITHREADED, merge your root outputs with
   hadd output.root output_t{0..N}.root
   Example : if you used 4 thread -> hadd result.root output_t{0..3}.root
	
########################################################################
Help :

Usage: complexRadiation  [-t <int>] -m=<file>

  Required options:
  -m, --macro=file          macro file(default: input_filename.mac)

  Optional options:
  -t, --thread=int          number of threads(only available with G4 multithread option)
  
########################################################################


