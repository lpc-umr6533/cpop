/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MeshOutFormats.hh"

namespace MeshOutFormats
{
	///////////////////////////////////////////////////////////////////
	///
	///////////////////////////////////////////////////////////////////
	QString getFormatName(outputFormat pFormat)
	{
		switch(pFormat)
		{
			case OFF:
				return "OFF";
			case GDML:
				return "GDML";
			case GEANT_4:
				return "GEANT_4";			
			case GATE:
				return "GATE";
			default:
				return "Unknow";
		}
	}
}