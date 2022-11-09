/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef AGENT_SETTINGS_HH
#define AGENT_SETTINGS_HH

#include "DynamicAgent.hh"
#include "GeometrySettings.hh"

// #define SIMULATION_VALID_AGENT_NEW_POS ///< \brief. if true at each iteration the simulation manager validate the requiered position of the agent. Else no check is made, Agent can be at the same position

/// \brief settings used by the libraries. 
namespace Settings
{
	/// @namespace Agent
	namespace nAgent
	{
		using namespace Settings::Geometry;
		typedef SpatialableAgent<double, Point_2, Vector_2> t_SpatialableAgent_2;	///< \brief a 2D spatialable agent
		typedef SpatialableAgent<double, Point_3, Vector_3> t_SpatialableAgent_3;	///< \brief a 3D spatialable agent

		typedef DynamicAgent<double, Point_2, Vector_2> t_DynamicAgent_2;			///< \brief a 2D dynamic agent
		typedef DynamicAgent<double, Point_3, Vector_3> t_DynamicAgent_3;			///< \brief a 3D dynamic agent
	}
}

#endif // AGENT_SETTINGS_HH