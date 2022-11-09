/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ENGINE_SETTINGS_HH
#define ENGINE_SETTINGS_HH

static const unsigned int INITIAL_MAX_THREAD = 12;
static const QThread::Priority SIMU_THREAD_PRIORITY = QThread::LowPriority;

#include "GeometrySettings.hh"
#include "SpatialDataStructure.hh"
#include "WorldLayer.hh"

/// @namespace Settings
namespace Settings
{
	/// \brief Geometries settngs
	namespace SDS
	{
		typedef SpatialDataStructure<double, Point_2, Vector_2> t_SpatialDataStructure_2;	///< \brief 2D spatial data structure
		typedef SpatialDataStructure<double, Point_3, Vector_3> t_SpatialDataStructure_3;	///< \brief 3D spatial data structure
	}

	/// \brief Environement settings
	namespace Env
	{
		typedef WorldLayer<double, Point_2, Vector_2> t_WorldLayer_2;	///< \brief 2D world layer
		typedef WorldLayer<double, Point_3, Vector_3> t_WorldLayer_3;	///< \brief 3D world layer
	}

}


#endif // ENGINE_SETTINGS_HH