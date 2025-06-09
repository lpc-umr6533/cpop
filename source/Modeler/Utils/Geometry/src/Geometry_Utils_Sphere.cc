#include "Geometry_Utils_Sphere.hh"
#include "CGAL_Utils.hh"
#include "RandomEngineManager.hh"

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

namespace Utils::Geometry::Sphere {

using namespace Utils::myCGAL;

/// \param sphere_center The center of the sphere we want to generate point for
/// \param sphere_radius The radius of the sphere we want to generate point for
/// \param sphere_degree The number of step we want to have for the sphere <=> number of point on the equator.
/// \return the N requested point on the sphere
std::vector<Point_3> getNPointOnSphere( Point_3 sphere_center, double sphere_radius, unsigned int sphere_degree) {
	assert(sphere_degree > 2); // else not enought point to generate a "true sphere"
	assert(sphere_radius >= 0. );
	std::vector<Point_3> result;
	double mPi_x2 = M_PI * 2.;
	for(unsigned int lat = 0; lat < sphere_degree; lat++) {
		for(unsigned int hor = 0; hor < sphere_degree; hor++) {
			double gamma = (double) (lat * mPi_x2) / (double) sphere_degree; 		/// gamma part of [0, 2.pi]
			double theta = (double) (hor * M_PI) / (double) sphere_degree; 		/// theta part of [0, pi].
			result.emplace_back( 	sphere_center.x() +	sphere_radius*std::cos( gamma ) * std::sin(theta),
														sphere_center.y() +	sphere_radius*std::sin( gamma ) * std::sin(theta),
														sphere_center.z() + sphere_radius*std::cos( theta ) );
		}
	}

	return result;
}

/// \return The farthest point on disc for the given direction
/// \param sphereCenter The origin of the sphere
/// \param sphereRadius The radius of the sphere
/// \param direction  The direction in which we want to find the point
Point_3 getIntersecPtWithSphere(Point_3 sphereCenter, double sphereRadius, Vector_3 direction) {
	assert(sphereRadius > 0.);
	assert(direction != CGAL::NULL_VECTOR);
	return (sphereCenter + ( normalize(direction) * sphereRadius) );
}

/// \param pt The point to project one the sphere
/// \param sphereRadius The radius of the sphere to project
/// \param sphereCenter The center of the sphere to project
/// \return the projected given point (pt) on the given sphere
Point_3 getProjectionOnSphere(Point_3 pt, double sphereRadius, Point_3 sphereCenter) {
	return getIntersecPtWithSphere(sphereCenter, sphereRadius, Vector_3(pt - sphereCenter));
}

/// \return The number of intersection
/// \param seg a segment of the line
/// \param sphereCenter  The center of the circle
/// \param sphereRadius  The radius of the circle
/// \param intersection1 The first potential intersection point
/// \param intersection2 The second potential intersection point
int lineSphereIntersection(Segment_3 seg, Point_3 sphereCenter, double sphereRadius, Point_3& intersection1, Point_3& intersection2) {
	assert( seg.source() != seg.target());

	Vector_3 lineDir = seg.target() - seg.source();
	double lineDirLength = lineDir.x()*lineDir.x() + lineDir.y()*lineDir.y() + lineDir.z()*lineDir.z();
	lineDirLength = sqrt(lineDirLength);
	Vector_2 lineDirNormalized(lineDir.x()/lineDirLength, lineDir.y()/lineDirLength, lineDir.z()/lineDirLength);
	double a = lineDir.x()*lineDir.x() + lineDir.y()*lineDir.y() + lineDir.z()*lineDir.z();

	double b = 2. * (
		( lineDir.x() * (seg.source().x() - sphereCenter.x())) +
		( lineDir.y() * (seg.source().y() - sphereCenter.y())) +
		( lineDir.z() * (seg.source().z() - sphereCenter.z()))
	);

	double c = sphereCenter.x()*sphereCenter.x() + sphereCenter.y()*sphereCenter.y()  + sphereCenter.z()*sphereCenter.z()
		+ seg.source().x()*seg.source().x() + seg.source().y()*seg.source().y() + seg.source().z()*seg.source().z()
		-2.* (sphereCenter.x()*seg.source().x() + sphereCenter.y()*seg.source().y() + sphereCenter.z()*seg.source().z()) - sphereRadius*sphereRadius;

	double delta = b*b - 4.*a*c;
	if(delta < 0.) {
		return 0; // no intersection
	}

	if(delta == 0.) {
		double k = -b / (2.*a);
		intersection1 = Point_3(seg.source().x() + k* lineDir.x(), seg.source().y() + k* lineDir.y(), seg.source().z() + k* lineDir.z());
		return 1;
	}

	// else 2 intersections
	double k1 = (-b - sqrt(delta) ) /(2.*a);
	double k2 = (-b + sqrt(delta) ) /(2.*a);
	intersection1 = Point_3( seg.source().x() + k1*lineDir.x(), seg.source().y() + k1*lineDir.y(), seg.source().z() + k1*lineDir.z());
	intersection2 = Point_3( seg.source().x() + k2*lineDir.x(), seg.source().y() + k2*lineDir.y(), seg.source().z() + k2*lineDir.z());

	return 2;
}

/// \return The number of intersection
/// \param seg The segment to check
/// \param sphereCenter The center of the sphere
/// \param sphereRadius The radius of the sphere
/// \param intersection1 The first potential intersection point
/// \param intersection2 The second potential intersection point
int segmentSphereIntersection(Segment_3 seg, Point_3 sphereCenter, double sphereRadius, Point_3& intersection1, Point_3& intersection2) {
	assert( seg.source() != seg.target());
	int nbIntersection = lineSphereIntersection(seg, sphereCenter, sphereRadius, intersection1, intersection2);
	K::Iso_cuboid_3 bb(seg.source(), seg.target());

	if(nbIntersection == 0) return 0;
	if(nbIntersection == 1) {
		return (bb.bounded_side(intersection1) != CGAL::ON_UNBOUNDED_SIDE);
	}

	if(nbIntersection == 2) {
		// if the first intersection point not on the segment
		if(bb.bounded_side(intersection1) == CGAL::ON_UNBOUNDED_SIDE) {
			intersection1 = intersection2;
			return (bb.bounded_side(intersection2) != CGAL::ON_UNBOUNDED_SIDE);
		} else {
			/// check if the second intersection point is onthe segment.
			return 1 + ((int)(bb.bounded_side(intersection2) != CGAL::ON_UNBOUNDED_SIDE));
		}
	}

	return 0;
}

/// \param sphereCenter The origin of the sphere
/// \param sphereRadius The radius of the sphere
/// \param plane The plane to check
/// \param projectionOfSphereCenter The projection of the sphere center on the plane
/// \param circleRadius the radius of the intersection between sphere and plane
/// \return  0 : no intersection, 1 : the intersection is a point, 2 : the intersection is a sphere.
/// \warning the three points need to be distincts
int planeSphereIntersection(Point_3 sphereCenter, double sphereRadius, Plane_3 plane, Point_3& projectionOfSphereCenter, double& circleRadius) {
	assert(sphereRadius > 0.);
	Point_3 projOriginSphere = plane.projection(sphereCenter);
	double squarDistProjToOrigin = CGAL::squared_distance(sphereCenter, projOriginSphere);
	double squarSphereRadius = sphereRadius * sphereRadius;
	if(squarDistProjToOrigin > squarSphereRadius) {
		return 0;
	}

	if(squarDistProjToOrigin == squarSphereRadius) {
		projectionOfSphereCenter = projOriginSphere;
		return 1;
	}

	// else get the 3 intersections
	projectionOfSphereCenter = projOriginSphere;
	circleRadius = sqrt(squarSphereRadius - squarDistProjToOrigin);

	return 2;
}

/// \return The spot requested inside the sphere
/// \param radius The (external) radius of the sphere
/// \param center The center of the sphere
/// \param internalRadius The internal radius of the sphere
Point_3 getSpotOnSphere(double radius, Point_3 center, double internalRadius) {
	Point_3 p =	 Point_3(	RandomEngineManager::getInstance()->randd( -radius, radius),
												RandomEngineManager::getInstance()->randd( -radius, radius),
												RandomEngineManager::getInstance()->randd( -radius, radius) );

	double nucleusSquareR = radius * radius;
	double internalSqrRadius = internalRadius * internalRadius;
	double squaredLength =  p.x()*p.x() + p.y()*p.y() + p.z()*p.z();

	while((squaredLength > nucleusSquareR) || (squaredLength < internalSqrRadius)) {
		p = Point_3( 	RandomEngineManager::getInstance()->randd( -radius, radius),
									RandomEngineManager::getInstance()->randd( -radius, radius),
									RandomEngineManager::getInstance()->randd( -radius, radius) );

		squaredLength =  p.x()*p.x() + p.y()*p.y() + p.z()*p.z();
	}

	return {p.x()+center.x(), p.y()+center.y(), p.z()+center.z()};
}

}
