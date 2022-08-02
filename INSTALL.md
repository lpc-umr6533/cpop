/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA
This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/

# How to install CPOP v2.0

This page will guide you to install CPOP 2.0.
Installation has been tested on the following OS:
1. Ubuntu 16.04
2. Ubuntu 18.04
3. CentOS7.4
4. CentOS7.5

First, you need to install the following dependencies:
1. [Qt5](https://www.qt.io/)
2. [CGAL 4.11](https://www.cgal.org/)
3. [CLHEP 2.4.0.4](http://proj-clhep.web.cern.ch/proj-clhep/)
4. [Root 6.13.02](https://root.cern.ch/)
5. [Geant4 10.04](https://geant4.web.cern.ch/)
6. (optional) GATE : TODO

***

## Notation

We will use generic notation in this tutorial to denote parts of the command the user need to modify.
* ${lib-src} : path to the source directory of lib
* ${lib-build} : path to the build directory of lib
* ${lib-install} : path to the install directory of lib

***

## Qt5

Qt5 may be already provided by your distribution. If it is not, you can install it using the install provided by Qt on their website.

***

## CGAL 

```bash
cd ${CGAL-build}
cmake -DCGAL_DONT_OVERRIDE_CMAKE_FLAGS=OFF -DCMAKE_INSTALL_PREFIX=${CGAL-install} ${CGAL-src}
make -j4
make install
```

Note : If you get the following error when installing CHLEHP (see below)) 
error: ‘(3.1415926535897931e+0 * 3.1415926535897931e+0)’ is not a constant expression
static constexpr double pi2 = pi*pi;

You need to edit the CXX_FLAGS of CGAL and remove -frounding-math

***

## CLHEP

```bash
cd ${CLHEP-build}
cmake -DCMAKE_INSTALL_PREFIX=${CLHEP-install} ${CLHEP-src}
make -j4
make test
make install
```

***

## Root

```bash
cd ${Root-build}
cmake -DCMAKE_INSTALL_PREFIX=${Root-install} ${Root-src}
make -j4
```


***

## Geant4

Note:
* If you want to use CPOP with GATE, you need to disable Geant4 multithreading.
* We build geant4 with -DGEANT4_BUILD_VERBOSE_CODE to OFF to disable some G4cout outputs and run our simulation faster.
* GEANT4 requires CLHEP. If you installed CLHEP in a custom directory, you to tell Geant4 where CLHEP is installed. 

```bash
cd ${Geant4-build}
cmake  -DGEANT4_USE_SYSTEM_CLHEP=ON -DCLHEP_ROOT_DIR=${CLHEP-install} -DGEANT4_INSTALL_DATA=ON -DGEANT4_USE_GDML=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_QT=ON   -DGEANT4_BUILD_MULTITHREADED=ON -DGEANT4_BUILD_VERBOSE_CODE=OFF -DCMAKE_INSTALL_PREFIX=${Geant4-install} ${Geant4-src}
make -j4
```

*** 

## CPOP

We are done with dependencies. If you installed them in a custom directory, you need to tell CPOP where to find them.
This can be done by telling CPOP where the configuration files for each dependencies are.

Most common location for each dependencies :
* CGAL_DIR : `${CGAL-install}/lib/CGAL`
* CLHEP_DIR : `${CLHEP-install}/lib/CLHEP2.4.0.4`
* ROOT_DIR : `${Root-install}/lib/cmake`
* GEANT4_DIR : `${Geant4-install}/lib/Geant4-10.4.1`

```bash
cd ${CPOP-build}
cmake -DCGAL_DIR=CGAL_DIR -DCLHEP_DIR=CLHEP_DIR -DROOT_DIR=ROOT_DIR -DGEANT4_DIR=GEANT4_DIR -DCMAKE_INSTALL_PREFIX=${CPOP-install} ${CPOP-src}
cmake --build . --target test
cmake --build . --target install
```

After installing cpop, ${CPOP-install} should contains the following directories:
* CPOP : generated librairies and a bash script containing all your _DIR variables. You have to source this script before trying to compile any example.
* include : all CPOP header files.
* lib : cmake files to export CPOP in other cmake projects.


