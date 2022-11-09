/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GRID_UTILS_HH
#define GRID_UTILS_HH

#include "CellProperties.hh"
#include "EnvironmentSettings.hh"
#include "GridSettings.hh"

//////////////////////////////////////////////////////////////////////////////
/// @namespace Utils
//////////////////////////////////////////////////////////////////////////////	
namespace Utils
{
	//////////////////////////////////////////////////////////////////////////////
	/// \brief utils relative to the grid
	//////////////////////////////////////////////////////////////////////////////	
	namespace nGrid
	{
		/// \brief propose grid for the cell and the sub environment
		Settings::nGrid::t_Grid_3* getOptimalGrid(const CellProperties*, const Settings::nEnvironment::t_SimulatedSubEnv_3*, double);
		
	}
}

#endif // GRID_UTILS_HH