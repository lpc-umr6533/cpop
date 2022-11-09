/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ROUND_SHAPE_HH
#define ROUND_SHAPE_HH

#include "Shape.hh"
#include "GeometrySettings.hh"

#include <InformationSystemManager.hh>

using namespace Settings::Geometry;

//////////////////////////////////////////////////////////////////////////////
/// \brief The agent class : define a round shape
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class Round_Shape : public Shape
{
public:
	/// \brief constructor
	Round_Shape(Kernel = 0.);
	/// \brief destructor
	virtual ~Round_Shape();
	/// \brief called when require to draw the body
	void draw() const;
	/// \brief radius getter of the sphere.
	Kernel getRadius() const			{return radius;};
	/// \brief radius setter
	void setRadius(Kernel pRadius) 		{radius = pRadius;};	

private:
	Kernel radius;	///< The radius of the sphere	
};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Round_Shape<Kernel, Point, Vector>::Round_Shape(Kernel pRadius):
	Shape(ShapeTypes::ROUND),
	radius(pRadius)
{

}

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Round_Shape<Kernel, Point, Vector>::~Round_Shape()
{
	
}

////////////////////////////////////////////////////////////////////////////////
/// 
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void Round_Shape<Kernel, Point, Vector>::draw() const
{
	QString mess = "unable to draw shape for this king of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Round shape");	
}

/// 2D specialization
template<>
void Round_Shape<float, Point_2, Vector_2>::draw() const;

/// 3D specialization
template<>
void Round_Shape<float, Point_3, Vector_3>::draw() const;

#endif // ROUND_SHAPE_HH