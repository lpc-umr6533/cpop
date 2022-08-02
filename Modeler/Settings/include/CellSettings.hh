/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_SETTINGS_HH
#define CELL_SETTINGS_HH

#include "AgentSettings.hh"
#include "Cell.hh"
#include "CellVariableAttribute.hh"
#include "GeometrySettings.hh"
#include "RoundCell.hh"
#include "RoundNucleus.hh"

/// @namespace Settings
namespace Settings
{
	/// \brief cell settings
	namespace nCell
	{
		using namespace Settings::Geometry;

		typedef Cell<double, Point_2, Vector_2> t_Cell_2;					///< \brief 2D Cell
		typedef Cell<double, Point_3, Vector_3> t_Cell_3;					///< \brief 3D Cell

		typedef RoundCell<double, Point_2, Vector_2> t_RoundCell_2;			///< \brief 2D round cell
		typedef RoundCell<double, Point_3, Vector_3> t_RoundCell_3;			///< \brief 3D round cell

		typedef Nucleus<double, Point_2, Vector_2> t_Nucleus_2;				///< \brief 2D nucleus
		typedef Nucleus<double, Point_3, Vector_3> t_Nucleus_3;				///< \brief 3D nucleus

		typedef RoundNucleus<double, Point_2, Vector_2> t_RoundNucleus_2;	///< \brief 2D Round nucleus
		typedef RoundNucleus<double, Point_3, Vector_3> t_RoundNucleus_3;	///< \brief 3D Round nucleus
	}
}

#endif // CELL_SETTINGS_HH