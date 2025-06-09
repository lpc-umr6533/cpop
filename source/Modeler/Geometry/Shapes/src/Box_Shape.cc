#include "Box_Shape.hh"

#include <OpenGL_Utils.hh>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif
#include <GL/gl.h>

#include <QString>
/// Specialization of Dynamic agent templates

/// 2D
template<>
void Box_Shape<double, Point_2, Vector_2>::draw() const {
	/// \todo
}

/// 3D
template<>
void Box_Shape<double, Point_3, Vector_3>::draw() const {
	/// \todo
}
