#ifndef AGENT_SETTINGS_HH
#define AGENT_SETTINGS_HH

#include "DynamicAgent.hh"
#include "GeometrySettings.hh"

// #define SIMULATION_VALID_AGENT_NEW_POS ///< \brief. if true at each iteration the simulation manager validate the requiered position of the agent. Else no check is made, Agent can be at the same position

/// \brief settings used by the libraries.
namespace Settings::nAgent {

using namespace Settings::Geometry;

using t_SpatialableAgent_2 = SpatialableAgent<double, Point_2, Vector_2>;	///< \brief a 2D spatialable agent
using t_SpatialableAgent_3 = SpatialableAgent<double, Point_3, Vector_3>;	///< \brief a 3D spatialable agent

using t_DynamicAgent_2 = DynamicAgent<double, Point_2, Vector_2>;       ///< \brief a 2D dynamic agent
using t_DynamicAgent_3 = DynamicAgent<double, Point_3, Vector_3>;       ///< \brief a 3D dynamic agent

}

#endif
