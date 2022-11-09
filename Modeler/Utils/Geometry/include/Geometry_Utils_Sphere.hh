/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GEOMETRY_UTILS__SPHERE_HH
#define GEOMETRY_UTILS__SPHERE_HH

#include "Mesh3DSettings.hh"

using namespace Settings::Geometry;	
using namespace Settings::Geometry::Mesh3D;	

//////////////////////////////////////////////////////////////////////////////
/// @namespace Utils
//////////////////////////////////////////////////////////////////////////////
namespace Utils
{
	//////////////////////////////////////////////////////////////////////////////
	/// @namespace Geometry
	//////////////////////////////////////////////////////////////////////////////
	namespace Geometry
	{
		//////////////////////////////////////////////////////////////////////////////
		/// \brief geometric utils for Sphere
		/// @author Henri Payno
		//////////////////////////////////////////////////////////////////////////////
		namespace Sphere
		{
			using namespace Settings::Geometry;	
			using namespace Settings::Geometry::Mesh3D;	

			/// \brief return n point on the sphere surface, corresponding to n degre on both angle axis axis
			std::vector<Point_3> getNPointOnSphere( Point_3 sphere_center, double sphere_radius, unsigned int sphere_degree);
			/// \brief return the farthest point on sphere according to a direction.
			Point_3 getIntersecPtWithSphere(Point_3 sphereCenter, double sphereRadius, Vector_3 direction);
			/// \brief return the projection point on the given sphere
			Point_3 getProjectionOnSphere(Point_3 pt, double sphereRadius, Point_3 sphereCenter);

			// \brief return the number of intersection between a line and a sphere
			int lineSphereIntersection(Segment_3 seg, Point_3 sphereCenter, double sphereRadius, Point_3& intersection1, Point_3& intersection2);
			// \brief return the number of intersection between a segment and a sphere
			int segmentSphereIntersection(Segment_3 seg, Point_3 sphereCenter, double sphereRadius, Point_3& intersection1, Point_3& intersection2);
			// \brief return the degree of intersection between a line and a sphere. 0 : no intersection, 1 : a point, 2 : a circle
			int planeSphereIntersection(Point_3 sphereCenter, double sphereRadius, Plane_3 plane, Point_3& projectionOfSphereCenter, double& circleRadius);
			/// \brief return a random spot included on the sphere
			Point_3 getSpotOnSphere(double radius, Point_3 origin, double internalRadius = 0.);
		}
	}
}

#endif // GEOMETRY_UTILS_HH