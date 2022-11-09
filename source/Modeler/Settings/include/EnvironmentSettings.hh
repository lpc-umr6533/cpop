/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ENVIRONMENT_SETTINGS_HH
#define ENVIRONMENT_SETTINGS_HH

#include "AgentSettings.hh"
#include "DiscsSDelimitation.hh"
#include "Environment.hh"
#include "SimulatedSubEnv.hh"

/// @namespace Settings
namespace Settings
{
	/// \brief environment settings
	namespace nEnvironment
	{
		using namespace Settings::Geometry;

		typedef SubEnvironment<double, Point_2, Vector_2> t_Sub_Env_2;					///< \brief a 2D Sub environment
		typedef SubEnvironment<double, Point_3, Vector_3> t_Sub_Env_3;					///< \brief a 3D Sub environment

		typedef Environment<double, Point_2, Vector_2> t_Environment_2;					///< \brief 2D Environment
		typedef Environment<double, Point_3, Vector_3> t_Environment_3;					///< \brief 3D Environment

		typedef SimulatedSubEnv<double, Point_2, Vector_2> t_SimulatedSubEnv_2;			///< \brief 2D simulated sub environment
		typedef SimulatedSubEnv<double, Point_3, Vector_3> t_SimulatedSubEnv_3;			///< \brief 3D simulated sub environment

		typedef SpatialDelimitation<double, Point_2, Vector_2> t_SpatialDelimitation_2;	///< \brief 2D spatial delimitation
		typedef SpatialDelimitation<double, Point_3, Vector_3> t_SpatialDelimitation_3;	///< \brief 3D spatial delimitation

		typedef DiscsSDelimitation<double, Point_2, Vector_2> t_DiscsSDelimitation_2;	///< \brief 2D Disc spatial delimitation
		typedef DiscsSDelimitation<double, Point_3, Vector_3> t_DiscsSDelimitation_3;	///< \brief 3D Disc spatial delimitation
	}
}

#endif // ENVIRONMENT_SETTINGS_HH