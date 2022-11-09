/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MeshTypes.hh"

namespace MeshTypes
{
	/////////////////////////////////////////////////////////////////////////////////
	/// \param <pType> {The type we want the name for}
	/////////////////////////////////////////////////////////////////////////////////
	QString getName(MeshType pType)
	{
		switch(pType)
		{
			// case Voronoi_tesselation:
			// 	return "Voronoi tesselation";
			default:
				return "Unknow";
		}
	}
}