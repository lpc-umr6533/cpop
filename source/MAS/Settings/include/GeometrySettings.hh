#ifndef GEOMETRY_SETTINGS_HH
#define GEOMETRY_SETTINGS_HH

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Filtered_kernel.h>

#include "Spatialable.hh"

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

/// @namespace Settings
namespace Settings::Geometry {

using CK = CGAL::Exact_predicates_inexact_constructions_kernel; ///< \brief the basic Kernel definition
using K = CGAL::Filtered_kernel<CK>;                            ///< \brief the Kernel we will used, define filtre used
using Point_2 = CGAL::Point_2<K>;                               ///< \brief 2D Point
using Vector_2 = K::Vector_2;                                   ///< \brief 3D Vector
using Point_3 = K::Point_3;                                     ///< \brief 3D Point
using Vector_3 = K::Vector_3;                                   ///< \brief 3D Vector
using Direction_3 = K::Direction_3;                             ///< \brief 3D Direction

using t_Spatialable_2 = Spatialable<double, Point_2, Vector_2>; ///< \brief 2D spatialable
using t_Spatialable_3 = Spatialable<double, Point_3, Vector_3>; ///< \brief 3D spatialable

/// \brief used to make sure 3D Point comparaison is good
class comparePoint_3 {
public:
	/// \brief operator used to make comparaison between two point
	/// \param p1 first point to compare
	/// \param p2 second point to compare
	bool operator()(const Point_3& p1, const Point_3& p2) const {
		if(p1.x() != p2.x())
			return p1.x() < p2.x();

		if(p1.y() != p2.y())
			return p1.y() < p2.y();

		if(p1.z() != p2.z())
			return p1.z() < p2.z();

		return false;
	}
};

}

#endif
