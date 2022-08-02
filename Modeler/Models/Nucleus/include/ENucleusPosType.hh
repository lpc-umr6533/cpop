/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef E_NUCLEUS_POS_TYPE_HH
#define E_NUCLEUS_POS_TYPE_HH

#include <QString>

	enum eNucleusPosType
	{
		ORIGIN,			///< Use the position of the delaunay vertice
		BARYCENTER,		///< Use the barycenter of the shape vertices
		// RANDOM,		///< Pick a random position on the shape. Not done actually
		NO_STANDARD,	///< Pick a random type from all existing.
						///< \warning : must always stay at position n -1
		UNDEF,
		END_CHOICE		///< use to know the nbumber of position type
	};

	QString getNucleusPosTypeName(eNucleusPosType type);


#endif // E_NUCLEUS_POS_TYPE_HH