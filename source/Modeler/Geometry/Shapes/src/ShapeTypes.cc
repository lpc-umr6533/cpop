#include "ShapeTypes.hh"

namespace ShapeTypes {

/// \param <pType> {The type of shape we want to have the name for}
QString getName(ShapeType pType) {
	switch(pType) {
		case ROUND:   return "Round";
		case COMPLEX:	return "Complex";
		case BOX:     return "Box";		
		default:      return "Unknow shape";
	}
}

}
