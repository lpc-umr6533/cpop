#include "Geometry_Utils_Circle.hh"
#include "CGAL_Utils.hh"
#include "RandomEngineManager.hh"

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

namespace Utils::Geometry::Circle {

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
using namespace Utils::myCGAL;

/// \return The medium point on the circle
/// \param circleOrigin The center of the circle
/// \param circleRadius The radius of the circle
/// \param a One the point of for which we want the medium point on the circle
/// \param b The second point of for which we want the medium point on the circle
/// \warning point "a" must already be on the disc.
Point_2 getMediumPointOnCircle(Point_2 circleOrigin, double, Point_2 a, Point_2 b) {
	Vector_2 v1 = a - circleOrigin;
	Vector_2 v2 = b - circleOrigin;
	double angle = getSignedAngle(v1, v2) / 2.;
	Vector_2 res = rotateVect2D(v1, angle);
	return {res.x() + circleOrigin.x(), res.y() + circleOrigin.y()};
}

/// \return The medium point on the circle
/// \param circleOrigin The center of the circle
/// \param circleRadius The radius of the circle
/// \param a in One the point of for which we want the medium point on the circle
/// \param b in The second point of for which we want the medium point on the circle
/// \warning point "a" must already be on the disc.
Point_2 getMediumTrigoPointOnCircle(Point_2 circleOrigin, double, Point_2 a, Point_2 b) {
	Vector_2 v1 = a - circleOrigin;
	Vector_2 v2 = b - circleOrigin;
	double angle = getSignedAngle(v1, v2);
	if(angle < 0.f ) {
		angle = 2.*M_PI + angle;
	}
	Vector_2 res = rotateVect2D(v1, angle / 2.);
	return {res.x() + circleOrigin.x(), res.y() + circleOrigin.y()};
}

/// \return The farthest point on disc for the given direction
/// \param circleCenter in The origin of the disc
/// \param circleRadius in The radius of the disc
/// \param direction The direction in which we want to find the point
Point_2 getIntersecPtWithCircle(Point_2 circleCenter, double circleRadius, Vector_2 direction) {
	assert(circleRadius > 0.);
	assert(direction != CGAL::NULL_VECTOR);
	return (circleCenter + ( normalize(direction) * circleRadius) );
}

/// \return The number of intersection
/// \param segStart The origin of the segment
/// \param segEnd The target of the segment
/// \param circleOrigin The center of the circle
/// \param circleRadius The radius of the circle
/// \param intersection1 The first potential intersection point
/// \param intersection2 The second potential intersection point
int lineCircleIntersection(
	Point_2 segStart, Point_2 segEnd,
	Point_2 circleOrigin, double circleRadius, Point_2& intersection1, Point_2& intersection2
) {
	assert(segStart != segEnd);

	Vector_2 lineDir(segEnd.x() - segStart.x(), segEnd.y() - segStart.y());
	double lineDirLength = lineDir.x() * lineDir.x() + lineDir.y() * lineDir.y();
	lineDirLength = sqrt(lineDirLength);
	Vector_2 lineDirNormalized(lineDir.x()/lineDirLength, lineDir.y()/lineDirLength);
	double a = lineDir.x() * lineDir.x() + lineDir.y()*lineDir.y();

	double b = 2. * (
		(lineDir.x() * (segStart.x() - circleOrigin.x()) ) +
		( lineDir.y() * (segStart.y() - circleOrigin.y()))
	);

	double c = circleOrigin.x()*circleOrigin.x() + circleOrigin.y()*circleOrigin.y()
		+ segStart.x()*segStart.x() + segStart.y()*segStart.y()
		-2.* (circleOrigin.x()*segStart.x() + circleOrigin.y()*segStart.y()) - circleRadius*circleRadius;

	double delta = b*b - 4.*a*c;
	if(delta < 0) {
		return 0; // no intersection
	}

	if(delta == 0) {
		double k = -b / (2.*a);
		intersection1 = Point_2(segStart.x() + k* lineDir.x(), segStart.y() + k* lineDir.y());
		return 1;
	}

	// else 2 intersections
	double k1 = (-b - sqrt(delta) ) /(2.*a);
	double k2 = (-b + sqrt(delta) ) /(2.*a);
	intersection1 = Point_2( segStart.x() + k1*lineDir.x(), segStart.y() + k1*lineDir.y());
	intersection2 = Point_2( segStart.x() + k2*lineDir.x(), segStart.y() + k2*lineDir.y());
	return 2;
}

/// \return The number of intersection
/// \param segStart The origin of the segment
/// \param segEnd The target of the segment
/// \param discOrigin The center of the circle
/// \param circleRadius The radius of the circle
/// \param intersection1 The first potential intersection point
/// \param intersection2 The second potential intersection point
int segmentCircleIntersection(
	Point_2 segStart, Point_2 segEnd,
	Point_2 discOrigin, double circleRadius, Point_2& intersection1, Point_2& intersection2
) {
	assert(segStart != segEnd);

	int nbIntersection = lineCircleIntersection(segStart, segEnd, discOrigin, circleRadius, intersection1, intersection2);
	if(nbIntersection == 0) return 0;
	if(nbIntersection == 1) {
		// std::cout << " before calling BB : " << intersection1 << std::endl;
		return (int)isOnBoundingBox(K::Iso_rectangle_2(segStart, segEnd), intersection1);
	}

	if(nbIntersection == 2) {
		// std::cout << " before calling BB : " << intersection1 << std::endl;
		// if the first intersection point not on the segment
		if(!isOnBoundingBox(K::Iso_rectangle_2(segStart, segEnd), intersection1)) {
			intersection1 = intersection2;
			return isOnBoundingBox(K::Iso_rectangle_2(segStart, segEnd), intersection2);
		} else {
			/// check if the second intersection point is onthe segment.
			return 1 + int(isOnBoundingBox(K::Iso_rectangle_2(segStart, segEnd), intersection2));
		}
	}

	return 0;
}

/// \param center The origin of the circle
/// \param radius The radius of the circle
/// \param nbPoint The number of point requested on the circle
/// \return The list of point part of the requested circle}
std::vector<Point_2> getNPointOnCircle(Point_2 center, double radius, unsigned int nbPoint) {
	std::vector<Point_2> pts;
	for(unsigned int iPt = 0; iPt < nbPoint; iPt++) {
		double angle = iPt * 2. * M_PI / nbPoint - M_PI;
		pts.emplace_back(
			center.x() + radius * cos(angle),
			center.y() + radius * sin(angle)
		);
	}

	return pts;
}

/// \param ptToProject The point to project
/// \param circleCenter The origin of the circle
/// \param circleRadius The radius of the circle
/// \return The projection of the point on the circle
Point_2 getProjectionOnCircle(Point_2 ptToProject, Point_2 circleCenter, double circleRadius) {
	return getIntersecPtWithCircle(circleCenter, circleRadius, Vector_2( ptToProject - circleCenter ));
}

/// \return The spot requested inside the sphere
/// \param radius the radius of the disc
/// \param center the origin of the disc
/// \param internalRadius The internal radius of the disc
Point_2 getSpotOnDisc(double radius, Point_2 center, double internalRadius) {
	double nucleusSquareR = radius * radius;
	double internalSqrRadius = internalRadius * internalRadius;

	Point_2 p = Point_2(	RandomEngineManager::getInstance()->randd( -radius, radius),
												RandomEngineManager::getInstance()->randd( -radius, radius) );

	double squaredLength =  p.x()*p.x() + p.y()*p.y();
	while((squaredLength > nucleusSquareR) || (squaredLength < internalSqrRadius)) {
		p = Point_2( 	RandomEngineManager::getInstance()->randd( -radius, radius),
									RandomEngineManager::getInstance()->randd( -radius, radius) );

		squaredLength =  p.x()*p.x() + p.y()*p.y();
	}

	return {p.x()+center.x(), p.y()+center.y()};
}

/// \return The spot requested inside the sphere
/// \param radius the radius of the disc
/// \param center the origin of the disc
/// \param internalRadius The internal radius of the disc
Point_3 getSpotOnDisc(double radius, Point_3 center, double internalRadius) {
	double nucleusSquareR = radius * radius;
	double internalSqrRadius = internalRadius * internalRadius;

	Point_3 p = Point_3(	RandomEngineManager::getInstance()->randd( -radius, radius),
							RandomEngineManager::getInstance()->randd( -radius, radius),
							0. );

	double squaredLength =  p.x()*p.x() + p.y()*p.y();
	while((squaredLength > nucleusSquareR) || (squaredLength < internalSqrRadius)) {
		p = Point_3( 	RandomEngineManager::getInstance()->randd( -radius, radius),
									RandomEngineManager::getInstance()->randd( -radius, radius),
									0. );

		squaredLength =  p.x()*p.x() + p.y()*p.y();
	}
	return {p.x()+center.x(), p.y()+center.y(), p.z()+center.z()};
}

}
