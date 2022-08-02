#########################################################
#Copyright (C): Henri Payno, Axel Delsol, 				#
#Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA	#
#														#
#This software is distributed under the terms			#
#of the GNU Lesser General  Public Licence (LGPL)		#
#See LICENSE.md for further details						#
#########################################################
This example shows how to use a configuration file to generate a population of cells.

########################################################################
Table of content:
1) What is a configuration file ?
2) How to build ?
3) How to use ?

########################################################################
1) What is a configuration file ?

A configuration file is a text file containing sections, keys and comments.

A comment is a line starting with "#" .

Sections are written into brackets e.g [mySection]. 
They contain pairs of (key,value) meant to be read by the configuration reader.

Example: example.cfg
#This is a comment
[mySection1]
key1 = value1
key2 = value2

[mySection2]
key1 = value1
key3 = value3

example.cfg is a configuration file containing 2 sections and 4 (key,value) pairs.
The first section is mySection1 and contains two pairs : (key1,value1) and (key2,value2).
The first section is mySection2 and contains two pairs : (key1,value1) and (key3,value3).
Even though key1 appears twice, they are considered as different pairs because they do not belong in the same sections.



WHAT CAN YOU READ FROM THE CONFIGURATION FILE ?

In a (key,value) pair, value can have the following types:
_int
_double 
_bool   : Possibles values are 'true' or 'false' (without quotes)
_vector : A vector is a list of values of type int, double or bool.

Below is an example of a configuration file with all available types:
[Example]
key1 = 1
key2 = 2.0
key3 = true
key4 = 1.2 2.3


########################################################################
2) How to build ?

1. mkdir build
2. cd build
3. source path/to/CPOP-install/CPOP/bin/CPOP.sh
4. cmake ..
5. make


########################################################################
3) How to use ?

source  path/to/G4/geant4.sh

./generatePopulation -f data/exampleConfig.cfg --vis

If you use option --vis, you can visualize your population with :
geomview data/exampleConfig.cfg.off &


########################################################################
Help:
Usage: generatePopulation  -f=<file>

  Required options:
  -f=file                   configuration file(default: input_filename.cfg)

  Optional options:
  --vis                     generate off file to visualize population


