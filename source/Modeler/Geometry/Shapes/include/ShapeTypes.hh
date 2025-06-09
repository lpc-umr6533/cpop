#ifndef SHAPE_TYPES
#define SHAPE_TYPES

#include <QString>

/// \brief define all available shape types
/// @author Henri Payno
namespace ShapeTypes {

/// \brief defined shape type on CPOP
enum ShapeType {
	ROUND,		///< \brief define a round shape in 2D or 3D (discoid, spheroid...)
	COMPLEX,	///< \brief define a complex shape wich can not be defined overwise.
	BOX,		///< \brief define a "box" 2D or 3D (rectangle, box...)
	UNKNOW
};

/// \brief return the corresponding name for the given shape type
QString getName(ShapeType pType);

}

#endif
