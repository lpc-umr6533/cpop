#ifndef GRID_UTILS_HH
#define GRID_UTILS_HH

#include "CellProperties.hh"
#include "EnvironmentSettings.hh"
#include "GridSettings.hh"

/// \brief utils relative to the grid
namespace Utils::nGrid {

/// \brief propose grid for the cell and the sub environment
Settings::nGrid::t_Grid_3* getOptimalGrid(const CellProperties*, const Settings::nEnvironment::t_SimulatedSubEnv_3*, double);

}

#endif
