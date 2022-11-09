/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CPOP_CIRCLE_HH
#define CPOP_CIRCLE_HH

#include "GeometrySettings.hh"

using namespace Settings::Geometry;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define CPOP Circle. 
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_Circle
{
public:

	/// \brief constructor
	CPOP_Circle(Point_3 pCenter, double r)
	{
		center = pCenter;
		radius = r;
	};

	/// \brief destructor
	~CPOP_Circle()
	{

	};

	/// \brief radisu getter
	double getRadius() const	{return radius;};

	/// \brief center getter
	Point_3 getCenter()	const 	{return center;};

private:
	Point_3 center;	///< \brief the center of the circle
	double radius;	///< \brief the radius of the circle
};

#endif // CPOP_CIRCLE_HH