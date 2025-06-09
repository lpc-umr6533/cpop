#ifndef ENVIRONMENT_SETTINGS_HH
#define ENVIRONMENT_SETTINGS_HH

#include "DiscsSDelimitation.hh"
#include "Environment.hh"
#include "SimulatedSubEnv.hh"

/// \brief environment settings
namespace Settings::nEnvironment {

using namespace Settings::Geometry;

using t_Sub_Env_2 = SubEnvironment<double, Point_2, Vector_2>;                  /// < \brief a 2D Sub environment
using t_Sub_Env_3 = SubEnvironment<double, Point_3, Vector_3>;                  /// < \brief a 3D Sub environment

using t_Environment_2 = Environment<double, Point_2, Vector_2>;                 /// < \brief 2D Environment
using t_Environment_3 = Environment<double, Point_3, Vector_3>;                 /// < \brief 3D Environment

using t_SimulatedSubEnv_2 = SimulatedSubEnv<double, Point_2, Vector_2>;         /// < \brief 2D simulated sub environment
using t_SimulatedSubEnv_3 = SimulatedSubEnv<double, Point_3, Vector_3>;         /// < \brief 3D simulated sub environment

using t_SpatialDelimitation_2 = SpatialDelimitation<double, Point_2, Vector_2>; /// < \brief 2D spatial delimitation
using t_SpatialDelimitation_3 = SpatialDelimitation<double, Point_3, Vector_3>; /// < \brief 3D spatial delimitation

using t_DiscsSDelimitation_2 = DiscsSDelimitation<double, Point_2, Vector_2>;   /// < \brief 2D Disc spatial delimitation
using t_DiscsSDelimitation_3 = DiscsSDelimitation<double, Point_3, Vector_3>;   /// < \brief 3D Disc spatial delimitation

}

#endif
