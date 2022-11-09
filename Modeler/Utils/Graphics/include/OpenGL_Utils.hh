/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef OPENGL_UTILS
#define OPENGL_UTILS

#include <vector>

//////////////////////////////////////////////////////////////////////////////
/// @namespace Utils
//////////////////////////////////////////////////////////////////////////////
namespace Utils
{
	//////////////////////////////////////////////////////////////////////////////
	/// \brief OpenGL relative utils
	/// @author Henri Payno
	//////////////////////////////////////////////////////////////////////////////	
	namespace OpenGLUtils
	{
		/// \brief will draw a simple sphere center on the origin
		/// of the world and with a radius of 1.
		void drawSphere(int nbHo, int nbVer);
		/// \brief raw a circle of radius 1
		void drawCircle(int nbPoints);
	}
}
#endif // OPENGL_UTILS