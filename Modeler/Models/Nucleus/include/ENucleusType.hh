/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ENUCLEUS_TYPE_HH
#define ENUCLEUS_TYPE_HH

#include <QString>

	enum eNucleusType
	{
		ROUND
		// ellipsoidal ...
	};

	QString getNucleusTypeName(eNucleusType);

#endif 