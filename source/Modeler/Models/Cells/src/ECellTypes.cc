/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ECellTypes.hh"

QString getCellTypeName(CellType type)
{
	switch(type)
	{
		case SIMPLE_ROUND: return "SimpleRound";
		default : return "unknow";
	}
}
