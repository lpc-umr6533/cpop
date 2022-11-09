/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ShapeTypes.hh"

namespace ShapeTypes
{
	/////////////////////////////////////////////////////////////////////////////
	/// \param <pType> {The type of shape we want to have the name for}
	/////////////////////////////////////////////////////////////////////////////
	QString getName(ShapeType pType)
	{
		switch(pType)
		{
			case ROUND :
				return "Round";
			case COMPLEX :	
				return "Complex";
			case BOX :
				return "Box";		
			default:
				return "Unknow shape";
		}
	}
}