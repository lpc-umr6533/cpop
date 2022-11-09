/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_POOL_HH
#define CELL_POOL_HH

#include "Spheroid.hh"
#include "SimpleSpheroidalCell.hh"

namespace CPOP
{
	typedef Spheroid<SimpleSpheroidalCell> t_SimpleSpheroid;

}

#endif // CELL_POOL_HH