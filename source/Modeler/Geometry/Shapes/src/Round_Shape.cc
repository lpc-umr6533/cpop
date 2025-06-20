#include "Round_Shape.hh"

#include "InformationSystemManager.hh"
#include "DisplaySettings.hh"

#include <OpenGL_Utils.hh>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif
#include <GL/gl.h>

#include <QString>

#ifndef NDEBUG
	#define DEBUG_ROUND_SHAPE 0
#else
	#define DEBUG_ROUND_SHAPE 0        // must always stay to 0
#endif

/// 2D
template<>
void Round_Shape<double, Point_2, Vector_2>::draw() const {
	// glScalef(radius, radius, 1.);
	// Utils::OpenGLUtils::drawCircle(NB_POINT);
}

/// 3D
template<>
void Round_Shape<double, Point_3, Vector_3>::draw() const {
	glScalef(_radius, _radius, _radius);
	Utils::OpenGLUtils::drawSphere(NB_LINES_PER_AXE_SPHERE, NB_LINES_PER_AXE_SPHERE);
	if(DEBUG_ROUND_SHAPE) {
		std::string message = "Drawing spheroid of radius : " + std::to_string(_radius);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, message, "Spheroid");
	}
}
