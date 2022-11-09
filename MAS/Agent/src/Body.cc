/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Body.hh"

/////////////////////////////////////////////////////////////////////
/// \param pCrossable True if the body can be cross
/// \param pDeformable True if the body of the agent can be deformable 
/////////////////////////////////////////////////////////////////////
Body::Body(bool pCrossable, bool pDeformable):
	crossable(pCrossable), deformable(pDeformable)
{

}

/////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////
Body::~Body()
{

}