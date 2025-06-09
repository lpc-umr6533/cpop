#include "MeshTypes.hh"

namespace MeshTypes {

/// \param <pType> {The type we want the name for}
QString getName(MeshType pType) {
	switch(pType) {
		// case Voronoi_tesselation:
		// 	return "Voronoi tesselation";
		default:
			return "Unknow";
	}
}

}
