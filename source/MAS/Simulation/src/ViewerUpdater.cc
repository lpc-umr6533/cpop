/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ViewerUpdater.hh"

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
ViewerUpdater::ViewerUpdater()
{

}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
ViewerUpdater::~ViewerUpdater()
{

}

/////////////////////////////////////////////////////////////////////
/// \brief signal listener than an update has been done (Viewers for example)
/////////////////////////////////////////////////////////////////////
void ViewerUpdater::sl_updated()
{
	emit si_updated();
}