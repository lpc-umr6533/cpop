/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef BOX_SHAPE_HH
#define BOX_SHAPE_HH

#include "Shape.hh"
#include "GeometrySettings.hh"

#include <InformationSystemManager.hh>

using namespace Settings::Geometry;
//////////////////////////////////////////////////////////////////////////////
/// \brief the discoid shape definition
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
/// \todo : doit plutôt hérité de SpatialDelimitation<Kernel, Point, Vector>
template<typename Kernel, typename Point, typename Vector>
class Box_Shape : public Shape
{
public:

	/// \brief constructor
	Box_Shape(Point, Point);
	/// \brief destructor
	~Box_Shape();

	/// \brief called when require to draw the body
	void draw() const;

private:
	Point bottomLeft;	///< \brief bottom left corner
	Point topRight;		///< \brief top right corner
};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// \param pBottomLeft 	The bottom left point
/// \param pTopRight 	The top right point
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Box_Shape<Kernel, Point, Vector>::Box_Shape(Point pBottomLeft, Point pTopRight):
	Shape(ShapeTypes::BOX),
	bottomLeft(pBottomLeft),
	topRight(pTopRight)
{

}

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Box_Shape<Kernel, Point, Vector>::~Box_Shape()
{

}

////////////////////////////////////////////////////////////////////////////////
/// draw
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Box_Shape<Kernel, Point, Vector>::draw() const
{
	QString mess = "unable to draw shape for this king of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Box shape");	
}

/// 2D specialization
template<>
void Box_Shape<double, Point_2, Vector_2>::draw() const;

/// 3D specialization
template<>
void Box_Shape<double, Point_3, Vector_3>::draw() const;

#endif // BOX_SHAPE_HH