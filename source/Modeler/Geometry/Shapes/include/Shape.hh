/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SHAPE_HH
#define SHAPE_HH

#include <Body.hh>
#include <ShapeTypes.hh>

//////////////////////////////////////////////////////////////////////////////
/// \brief the shape definition. Heritate from body
/// Must stay as a virtual class.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Shape : public Body
{
public:
	/// \brief constructor
	Shape(ShapeTypes::ShapeType);
	/// \brief destructor
	virtual ~Shape();

	/// \brief called when require to draw the body
	virtual void draw() const = 0;

private:
	/// \brief the shape type ID
	ShapeTypes::ShapeType shapeType;
};

#endif // SHAPE_HH