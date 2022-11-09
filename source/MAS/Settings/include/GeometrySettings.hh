/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GEOMETRY_SETTINGS_HH
#define GEOMETRY_SETTINGS_HH

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Filtered_kernel.h> 

#include "Spatialable.hh"

#define AXIS_X 0
#define AXIS_Y 1
#define AXIS_Z 2

/// @namespace Settings
namespace Settings
{
	/// \brief Geometries settngs
	namespace Geometry
	{
		typedef CGAL::Exact_predicates_inexact_constructions_kernel CK;				///< \brief the basic Kernel definition
		typedef CGAL::Filtered_kernel<CK> 							K; 				///< \brief the Kernel we will used, define filtre used
		typedef CGAL::Point_2<K>										Point_2;		///< \brief 2D Point
		typedef K::Vector_2											Vector_2;		///< \brief 3D Vector
		typedef K::Point_3											Point_3;		///< \brief 3D Point
		typedef K::Vector_3											Vector_3;		///< \brief 3D Vector
		typedef K::Direction_3										Direction_3;	///< \brief 3D Direction

		typedef Spatialable<double, Point_2, Vector_2> t_Spatialable_2;				///< \brief 2D spatialable
		typedef Spatialable<double, Point_3, Vector_3> t_Spatialable_3;				///< \brief 3D spatialable

		/// \brief used to make sure 3D Point comparaison is good
		class comparePoint_3
		{
		public:
			/// \brief operator used to make comparaison between two point
			/// \param p1 first point to compare
			/// \param p2 second point to compare
			bool operator()(const Point_3& p1, const Point_3& p2) const
			{
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
}

#endif // GEOMETRY_SETTINGS_HHmake
