/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GRID_SETTINGS_HH
#define GRID_SETTINGS_HH
#include "AgentSettings.hh"
#include "Grid.hh"

/// @namespace Settings
namespace Settings
{
	/// \brief grid settings
	namespace nGrid
	{
		using namespace Settings::Geometry;
		typedef Grid<double, Point_2, Vector_2> t_Grid_2;				///< \brief 2D Grid
		typedef Grid<double, Point_3, Vector_3> t_Grid_3;				///< \brief 3D Grid
		typedef GridElementTraits<double>		t_GridElementTraits;	///< \brief element traits used on grid
		typedef GridElement<double, Point_2>	t_GridElement_2;		///< \brief 2D Grid Element
		typedef GridElement<double, Point_3>	t_GridElement_3;		///< \brief 3D Grid Element
	}
}

#endif // GRID_SETTINGS_HH