/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "CPOP_Triangle.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param a 			The first vertex of the triangle
/// \param b			The second vertex of the triangle
/// \param c 			The third vertex of the triangle
/// \param pReffinable 	If wa can reffine ( subdive) the facet
////////////////////////////////////////////////////////////////////////////////////////////////////
CPOP_Triangle::CPOP_Triangle(Point_3 a, Point_3 b, Point_3 c, bool pReffinable):
	reffinable(pReffinable)
{
	points.resize(3);
	setA(a);
	setB(b);
	setC(c);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////
CPOP_Triangle::~CPOP_Triangle()
{
	points.clear();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The barycenter of the triangle
////////////////////////////////////////////////////////////////////////////////////////////////////
Point_3 CPOP_Triangle::getBarycenter() const
{
	return Point_3( (getA().x() + getB().x() + getC().x()) / 3., 
					(getA().y() + getB().y() + getC().y()) / 3., 
					(getA().z() + getB().z() + getC().z()) / 3.);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param i The position to set the point
/// \param pt The value f the point to add
////////////////////////////////////////////////////////////////////////////////////////////////////
void CPOP_Triangle::set(unsigned int i, Point_3 pt)
{
	assert(i < 3);
	points[i] = pt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param i  The position to get the point
/// \return The value requested
////////////////////////////////////////////////////////////////////////////////////////////////////
Point_3 CPOP_Triangle::get(unsigned int i) const
{
	assert(i < 3);
	return points[i];
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPOP_Triangle::operator == (const CPOP_Triangle& facet2) const
{

	for(unsigned int iPt = 0; iPt < 3; iPt++)
	{
		std::cout << iPt << " " << facet2.get(iPt) << " vs " << this->get(iPt) << std::endl;
		if(facet2.get(iPt) != this->get(iPt))
		{
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \warning this operator is set to organize triangle on set structure.
////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPOP_Triangle::operator < (const CPOP_Triangle& facet2) const
{
	/// because this function is used to organise facet on "set" want to have 
	/// the more interesting triangle on top.
	/// first : unrefinnable triangle aren't interesting : set then at the bottom
	if(this->canReffine() != facet2.canReffine()) 
	{
		return this->canReffine();
	}

	/// otherwise because wa have reffinable triangle and we are on a sphere,
	/// the more interesting triangle are the one with the higher area.
	/// because the win of point addition will be the higher.
	return (this->get16squareA() >= facet2.get16squareA());
	/// note : the win of information can be calculated by the value : 
	/// square_distance (newPointAdded, projection of the point)
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return the value of 16A², faster to calculate than area
/// \details based on the Heron formula.
/// Used to compare two triangle, quicker than aire.
////////////////////////////////////////////////////////////////////////////////////////////////////
double CPOP_Triangle::get16squareA() const
{
	double squareA = squared_distance(getC(), getB());
	double squareB = squared_distance(getA(), getC());
	double squareC = squared_distance(getA(), getB());

	return (((squareA + squareB + squareC )*(squareA + squareB + squareC )) - 2.*(squareA*squareA + squareB*squareB + squareC*squareC ));
}

