#include "OpenGL_Utils.hh"

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

#include <GL/gl.h>
#include <math.h>

#define SUPPRESS_WARNING(a) (void)a

namespace Utils::OpenGLUtils
{

/// \param <nbHo> {The number of horizontal lines}
/// \param <nbVer> {The number of vertical lines}
void drawSphere(int nbHo, int nbVer) {
	glPushMatrix();
	glScalef(2., 2., 2.);
	int i, j;
	for(i = 0; i <= nbHo; i++) {
		double lat0 = M_PI * ((double) (i - 1) / nbHo - 0.5);
		double z0  = sin(lat0);
		double zr0 =  cos(lat0);

		double lat1 = M_PI * ((double) i / nbHo -0.5);
		double z1 = sin(lat1);
		double zr1 = cos(lat1);

		glBegin(GL_QUAD_STRIP);
		for(j = 0; j <= nbVer; j++) {
			double lng = 2. * M_PI * (double) (j - 1) / nbVer;
			double x = cos(lng);
			double y = sin(lng);

			glNormal3f((float)(x * zr0), (float)(y * zr0), (float)z0);
			glVertex3f((float)(x * zr0), (float)(y * zr0), (float)z0);
			glNormal3f((float)(x * zr1), (float)(y * zr1), (float)z1);
			glVertex3f((float)(x * zr1), (float)(y * zr1), (float)z1);
		}
		glEnd();
		glPopMatrix();
	}
}

/// \param <nbPoints> {The number of point the circle is based on}
void drawCircle(int nbPoints) {
	glPushMatrix();
	glBegin(GL_LINE_LOOP);
	for(int i =0; i <= nbPoints; i++) {
		double angle = 2. * M_PI * i / nbPoints;
		double x = cos(angle);
		double y = sin(angle);
		glVertex2d(x,y);
	}
	glEnd(); 
	glPopMatrix();
}

}
