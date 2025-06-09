#ifndef FORCE_SETTINGS_HH
#define FORCE_SETTINGS_HH

#include "Force.hh"
#include "ElasticForce.hh"
#include "RepulsionForcePoint.hh"
#include "AttractionForcePoint.hh"

/// \brief force settings
namespace Settings::nForce {

using namespace Settings::Geometry;

using t_Force_2 = Force<double, Point_2, Vector_2>;					///< \brief 2D Force
using t_Force_3 = Force<double, Point_3, Vector_3>;					///< \brief 3D Force

using t_ElasticForce_2 = ElasticForce<double, Point_2, Vector_2>;	///< \brief a 2D elastic force
using t_ElasticForce_3 = ElasticForce<double, Point_3, Vector_3>;	///< \brief a 3D elastic force

using t_RepulsionForcePoint_2 = RepulsionForcePoint<double, Point_2, Vector_2>;	///< \brief a 2D elastic force
using t_RepulsionForcePoint_3 = RepulsionForcePoint<double, Point_3, Vector_3>;	///< \brief a 3D elastic force

using t_AttractionForcePoint_2 = AttractionForcePoint<double, Point_2, Vector_2>;	///< \brief a 2D elastic force
using t_AttractionForcePoint_3 = AttractionForcePoint<double, Point_3, Vector_3>;	///< \brief a 3D elastic force

}

#endif
