/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GEOMETRY_UTILS_CIRCLE_HH
#define GEOMETRY_UTILS_CIRCLE_HH

#include "GeometrySettings.hh"
#include "Mesh2DSettings.hh"

//////////////////////////////////////////////////////////////////////////////
/// @namespace Utils
//////////////////////////////////////////////////////////////////////////////	
namespace Utils
{
	//////////////////////////////////////////////////////////////////////////////
	/// \brief geometric utils
	/// @author Henri Payno
	//////////////////////////////////////////////////////////////////////////////
	namespace Geometry
	{
		//////////////////////////////////////////////////////////////////////////////
		/// \brief geometric utils for circle
		/// @author Henri Payno
		//////////////////////////////////////////////////////////////////////////////
		namespace Circle
		{
			using namespace Settings::Geometry;	
			using namespace Settings::Geometry::Mesh2D;

			/// \brief return the medium point on the disc between two point.
			Point_2 getMediumPointOnCircle(Point_2 sphereCenter, double sphereRadius, Point_2 a, Point_2 b);
			/// \brief return the medium point on the trigonometrique clock
			Point_2 getMediumTrigoPointOnCircle(Point_2 sphereCenter, double sphereRadius, Point_2 a, Point_2 b);
			/// \brief return the farthest point on a disc according to a direction.
			Point_2 getIntersecPtWithCircle(Point_2 circleCenter, double circleRadius, Vector_2 direction);
			/// \brief return the number of intersection between a segment and a circle
			int segmentCircleIntersection(Point_2 segStart, Point_2 segEnd, Point_2 circleOrigin, double circleRadius, Point_2& intersect1, Point_2& intersect2);
			/// \brief return the number of intersection between a line and a circle.
			int lineCircleIntersection(Point_2 segStart, Point_2 segEnd, Point_2 circleOrigin, double circleRadius, Point_2& intersect1, Point_2& intersect2);
			/// \brief return N regular point equaly distributed on a circle
			std::vector<Point_2> getNPointOnCircle(Point_2 center, double radius, unsigned int n);
			/// \brief project the point on the circle
			Point_2 getProjectionOnCircle(Point_2 ptToProject, Point_2 circleCneter, double circleRadius);
			/// \brief return a random spot on the disc.
			Point_2 getSpotOnDisc(double, Point_2 center, double internalRadius = 0.);
			/// \brief return a random spot on the disc, on the XY axis.
			Point_3 getSpotOnDisc(double radius, Point_3 center, double internalRadius = 0.);
		} // circle namespace
	} // geometry namespace
} // utils namespace

#endif // GEOMETRY_UTILS_CIRCLE_HH