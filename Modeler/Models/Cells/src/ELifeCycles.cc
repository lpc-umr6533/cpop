/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ELifeCycles.hh"

namespace LifeCycles
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \param <pLifeCycle> [in] {The life sate we want the name for} 
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	QString getName(LifeCycle pLifeCycle)
	{
		switch(pLifeCycle)
		{
			case G1 : return "  G1    ";
			case S  : return "  S     ";
			case G2 : return "  G2    ";
			case M  : return "  M     ";
			case G0 : return "  G0    ";
			case NA : return "  NA    ";
			default : return " Unknow ";
		}
	}
}