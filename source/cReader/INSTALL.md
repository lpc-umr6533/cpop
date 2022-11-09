Steps to install cReader library on your system:

1. mkdir build
2. cd build
3. cmake -DCMAKE_INSTALL_PREFIX=path/to/install ..
   Note -DCMAKE_INSTALL_PREFIX=path/to/install is only required if you don't want to install the library in /usr/local
4. make
5. make install or sudo make install
