/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ENUM_DISTRIBUTION
#define ENUM_DISTRIBUTION

////////////////////////////////////////////////////////////////////////////
/// \brief relative to distribution system
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
namespace Distribution
{
	/// \brief list the potentiel distribution type such as gaussian, random...
	enum DistributionType
	{
		RANDOM,
		NORMALE
	};
}


#endif