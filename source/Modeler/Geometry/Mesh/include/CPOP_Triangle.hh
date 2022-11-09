/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CPOP_Triangle_HH
#define CPOP_Triangle_HH

#include "Mesh3DSettings.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define CPOP facet. Used to know if we can reffine her or no.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_Triangle
{
public:
	/// \brief constructor
	CPOP_Triangle(Point_3 = Point_3(0., 0., 0.), Point_3 = Point_3(0., 0., 0.), Point_3 = Point_3(0., 0., 0.), bool reffinable = false);
	/// \brief destructor
	~CPOP_Triangle();

	/// \brief set the first point
	void setA(Point_3 a) {points[0] = a;};
	/// \brief set the second point of the facet
	void setB(Point_3 b) {points[1] = b;};
	/// \brief set the third point of the facet
	void setC(Point_3 c) {points[2] = c;};
	/// \brief point setter
	void set(unsigned int i, Point_3 pt);
	/// \brief return the first point
	Point_3 getA() const	{return points[0];};
	/// \brief return the second point of the facet
	Point_3 getB() const	{return points[1];};
	/// \brief return the third point of the facet
	Point_3 getC() const 	{return points[2];};
	/// \brief point getter
	Point_3 get(unsigned int) const;
	
	/// \brief is the facet a reffinable one
	bool canReffine() const {return reffinable;};
	/// \brief reffinement settter
	void setReffinement(bool b) {reffinable = b;};

	/// \brief comparator
	bool operator == (const CPOP_Triangle& ) const;
	/// \brief comparator
	bool operator < (const CPOP_Triangle& ) const;

	/// \brief return the barycenter of the facet
	Point_3 getBarycenter() const;
	/// \brief potential win setter
	double get16squareA() const;

private:
	std::vector<Point_3> points;	///< \brief the list of vertex
	bool reffinable;				///< \brief can we reffine this triangle.
};

#endif // CPOP_Triangle_HH