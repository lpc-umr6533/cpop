#ifndef BOX_SHAPE_HH
#define BOX_SHAPE_HH

#include "Shape.hh"
#include "GeometrySettings.hh"

#include <InformationSystemManager.hh>

using namespace Settings::Geometry;
/// \brief the discoid shape definition
/// @author Henri Payno
/// \todo : doit plutôt hérité de SpatialDelimitation<Kernel, Point, Vector>
template<typename Kernel, typename Point, typename Vector>
class Box_Shape : public Shape {
public:
	Box_Shape(Point, Point);

	/// \brief called when require to draw the body
	void draw() const override;

private:
	Point _bottomLeft;  ///< \brief bottom left corner
	Point _topRight;    ///< \brief top right corner
};

/////////////////////////// FUNCTION DEFINITIONS ///////////////////////////

/// \param pBottomLeft 	The bottom left point
/// \param pTopRight 	The top right point
template<typename Kernel, typename Point, typename Vector>
Box_Shape<Kernel, Point, Vector>::Box_Shape(Point pBottomLeft, Point pTopRight):
	Shape(ShapeTypes::BOX),
	_bottomLeft(pBottomLeft),
	_topRight(pTopRight)
{
}

template<typename Kernel, typename Point, typename Vector>
void Box_Shape<Kernel, Point, Vector>::draw() const {
	std::string mess = "unable to draw shape for this king of template parameter";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "Box shape");	
}

/// 2D specialization
template<>
void Box_Shape<double, Point_2, Vector_2>::draw() const;

/// 3D specialization
template<>
void Box_Shape<double, Point_3, Vector_3>::draw() const;

#endif
