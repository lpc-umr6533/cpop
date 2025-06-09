#ifndef SHAPE_HH
#define SHAPE_HH

#include <Body.hh>
#include <ShapeTypes.hh>

/// \brief the shape definition. Heritate from body
/// Must stay as a virtual class.
/// @author Henri Payno
class Shape : public Body {
public:
	Shape(ShapeTypes::ShapeType shapeType):
		_shapeType(shapeType)
	{}

private:
	/// \brief the shape type ID
	ShapeTypes::ShapeType _shapeType;
};

#endif
