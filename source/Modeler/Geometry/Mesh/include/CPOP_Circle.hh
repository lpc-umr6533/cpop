#ifndef CPOP_CIRCLE_HH
#define CPOP_CIRCLE_HH

#include "GeometrySettings.hh"

using namespace Settings::Geometry;

/// \brief Define CPOP Circle. 
/// @author Henri Payno
class CPOP_Circle {
public:
	CPOP_Circle(Point_3 pCenter, double r):
		_center(pCenter),
		_radius(r)
	{}

	/// \brief radisu getter
	[[nodiscard]] double getRadius() const { return _radius; }

	/// \brief center getter
	[[nodiscard]] Point_3 getCenter()	const { return _center; }

private:
	Point_3 _center;  ///< \brief the center of the circle
	double _radius;   ///< \brief the radius of the circle
};

#endif
