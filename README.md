# CPOP 2.0

The Cell Population modeler is combined to Geant4 simulations in order to tackle how energy depositions are allocated to cells, especially when enhancing radiation outcomes using high-Z nanoparticles.
CPOP is fully interfaced with the Geant4 Monte Carlo toolkit and is able to directly emulate Geant4 simulations after compilation.

CPOP structuration:
```
cpop-2.0/
	cmake/
	config/
	example/
	source/
	test/
	tool/
```

## Dependencies

To build CPOP, you need:
- [Qt5](https://www.qt.io/);
- [CGAL 5.0.1](https://www.cgal.org/);
- [CLHEP 2.4.6.1](http://proj-clhep.web.cern.ch/proj-clhep/);
- [Root 6.26.10](https://root.cern.ch/);
- [Geant4 11.1.0](https://geant4.web.cern.ch/).

Additionally, you will need to have some libraries installed. For Debian-likes:
- libgmp-dev
- libmpfr-dev
- libboost-system1.71-dev

### CGAL

Download:
```bash
git clone https://github.com/CGAL/cgal
cd cgal
git checkout v5.0.1
```

Build and install:
```bash
mkdir cgal_build
cd cgal_build
cmake -DCMAKE_BUILD_TYPE=Release -DCGAL_DONT_OVERRIDE_CMAKE_FLAGS=OFF -DCMAKE_INSTALL_PREFIX="${CGAL_DIR}" ../cgal
make install
```

### CLHEP

Download:
```bash
git clone https://gitlab.cern.ch/CLHEP/CLHEP.git
cd CLHEP
git checkout CLHEP_2_4_6_1
```

Build and install:
```bash
mkdir clhep_build
cd clhep_build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${CLHEP_DIR}" ../CLHEP
make -j4
make install
```

### ROOT

Download:
```bash
git clone https://github.com/root-project/root.git
cd root
git checkout v6-26-10
```

Build and install:
```bash
mkdir root_build
cd root_build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="${ROOT_DIR}" ../root
make -j4
make install
```

### Geant4

Download:
```bash
git clone https://github.com/Geant4/geant4.git
cd geant4
git checkout v11.1.0
```

Note: if you want to use CPOP with GATE9, you need to disable Geant4 multithreading.

Build and install:
```bash
mkdir geant4_build
cd geant4_build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_CXX_FLAGS=-std=c++17 -DCMAKE_INSTALL_PREFIX="${GEANT4_DIR}" -DGEANT4_INSTALL_DATA=ON -DGEANT4_INSTALL_DATADIR="${GEANT4_DIR}/data" -DGEANT4_BUILD_VERBOSE_CODE=OFF -DGEANT4_USE_QT=ON -DGEANT4_USE_OPENGL_X11=ON -DGEANT4_USE_GDML=ON -DGEANT4_BUILD_MULTITHREADED=ON ..
make -j4
make install
```

## Building CPOP

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCGAL_DIR="${CGAL_DIR}/lib/cmake/CGAL" -DCLHEP_DIR="${CLHEP_DIR}/lib/CLHEP-2.4.6.1" -DROOT_DIR="${ROOT_DIR}/lib/cmake" -DGeant4_DIR="${GEANT4_DIR}/lib/cmake/Geant4" -DOpenGL_GL_PREFERENCE=LEGACY ..
make -j4
make install
```

## Colophon

Tested on:
- Ubuntu 22.04
