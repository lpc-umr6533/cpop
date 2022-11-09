/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FORCE_SETTINGS_HH
#define FORCE_SETTINGS_HH

#include "AgentSettings.hh"
#include "Force.hh"
#include "ElasticForce.hh"
#include "RepulsionForcePoint.hh"
#include "AttractionForcePoint.hh"

/// @namespace settings
namespace Settings
{
	/// \brief force settings
	namespace nForce
	{
		using namespace Settings::Geometry;
		typedef Force<double, Point_2, Vector_2> t_Force_2;					///< \brief 2D Force
		typedef Force<double, Point_3, Vector_3> t_Force_3;					///< \brief 3D Force

		typedef ElasticForce<double, Point_2, Vector_2>	t_ElasticForce_2;	///< \brief a 2D elastic force
		typedef ElasticForce<double, Point_3, Vector_3>	t_ElasticForce_3;	///< \brief a 3D elastic force

		typedef RepulsionForcePoint<double, Point_2, Vector_2>		t_RepulsionForcePoint_2;	///< \brief a 2D elastic force
		typedef RepulsionForcePoint<double, Point_3, Vector_3>		t_RepulsionForcePoint_3;	///< \brief a 3D elastic force

		typedef AttractionForcePoint<double, Point_2, Vector_2>		t_AttractionForcePoint_2;	///< \brief a 2D elastic force
		typedef AttractionForcePoint<double, Point_3, Vector_3>		t_AttractionForcePoint_3;	///< \brief a 3D elastic force

	}
}

#endif // FORCE_SETTINGS_HH