# Install script for directory: /home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/levrague/Documents/apps/cpop-2.0/install_cpop-2.0")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "0")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xlibsx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CPOP/lib" TYPE STATIC_LIBRARY FILES "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/build/source/MAS/libPlatform_SMA.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xdevelx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/CPOP" TYPE FILE FILES
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/Agent.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/AgentStates.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/Body.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/DynamicAgent.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/Movable.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/Spatialable.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Agent/include/SpatialableAgent.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/ConflictSolver/include/ConflictSolver.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/ConflictSolver/include/SpatialConflictSolver.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Layers/include/Dimensioned_Layer.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Layers/include/Layer.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Layers/include/World.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Layers/include/WorldLayer.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Shapes/include/BoundingBox.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/Action.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/IDManager.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/MASPlatform.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/RandomEngineManager.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/Scheduler.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/SimulationManager.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/SpatialDataStructure.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/SpatialDataStructureManager.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/ThreadAgentGroup.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Simulation/include/ViewerUpdater.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Settings/include/AgentSettings.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Settings/include/EngineSettings.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/source/MAS/Settings/include/GeometrySettings.hh"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/build/source/MAS/Simulation/include/moc_MASPlatform.cpp"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/build/source/MAS/Simulation/include/moc_SimulationManager.cpp"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/build/source/MAS/Simulation/include/moc_ThreadAgentGroup.cpp"
    "/home/levrague/Documents/apps/cpop-2.0/cpop-2.0/build/source/MAS/Simulation/include/moc_ViewerUpdater.cpp"
    )
endif()

