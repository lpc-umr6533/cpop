/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef BOUNDING_BOX_HH
#define BOUNDING_BOX_HH

//////////////////////////////////////////////////////////////////////////////
/// \brief The BoundingBox
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <typename Point>
class BoundingBox
{
public:
	/// \brief constructor
	BoundingBox(Point = Point(), Point = Point());
	/// \brief destructor
	~BoundingBox();
	/// \brief copy constructor
	BoundingBox(const BoundingBox&);

	/// \brief bottomLeft point getter
	Point getBottomLeft() const 	{ return bottomLeft;}
	/// \brief topRight corner getter
	Point getTopRight() const 		{ return topRight;}

private:
	Point bottomLeft;		///< \brief the back bottom left corner
	Point topRight;			///< \brief the front top right corner
};

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
template <typename Point>
BoundingBox<Point>::BoundingBox(Point pbottomLeft , Point pTopRight  ):
	bottomLeft(pbottomLeft),
	topRight(pTopRight)
{

}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
template <typename Point>
BoundingBox<Point>::~BoundingBox()
{

}

//////////////////////////////////////////////////////////////////////////////
/// \param pOther the class to copy
//////////////////////////////////////////////////////////////////////////////
template <typename Point>
BoundingBox<Point>::BoundingBox(const BoundingBox& pOther)
{
	this->bottomLeft = pOther.bottomLeft;
	this->topRight = pOther.topRight;
}

#endif
