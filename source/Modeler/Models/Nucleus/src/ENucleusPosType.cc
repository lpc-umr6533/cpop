/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ENucleusPosType.hh"

////////////////////////////////////////////////////////////////////
/// \brief return the alpha nuemrical value of the nucleus position type
////////////////////////////////////////////////////////////////////
QString getNucleusPosTypeName(eNucleusPosType type)
{
	switch(type)
	{
		case ORIGIN :
		{
			return "ORIGIN";
		}

		case BARYCENTER :
		{
			return "BARYCENTER";
		}
		default : 
		{
			return "Unknow";
		}
	}
}
