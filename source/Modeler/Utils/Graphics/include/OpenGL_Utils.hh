#ifndef OPENGL_UTILS
#define OPENGL_UTILS

#include <vector>

/// \brief OpenGL relative utils
/// @author Henri Payno
namespace Utils::OpenGLUtils {

/// \brief will draw a simple sphere center on the origin
/// of the world and with a radius of 1.
void drawSphere(int nbHo, int nbVer);
/// \brief raw a circle of radius 1
void drawCircle(int nbPoints);

}

#endif
