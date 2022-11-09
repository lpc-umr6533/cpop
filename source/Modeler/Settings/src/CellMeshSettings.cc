/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "CellMeshSettings.hh"
#include <iostream>

// extern unsigned int NUCLEUS_SPHERE_NB_POINT =;

unsigned int NUCLEUS_SPHERE_NB_POINT = MIN_SPHERE_NB_POINT;

namespace Settings
{
	/// \param pNbPts the number of degree to pick on each axis. So the total number pf point will be pNbPts*pNbPts
	void setNucleusNbDegreePerAxis(unsigned int pNbPts)
	{
		NUCLEUS_SPHERE_NB_POINT = pNbPts;
	}
}