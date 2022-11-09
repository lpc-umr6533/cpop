/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DistributionFactory.hh"
#include "RandomCellDistribution.hh"

static DistributionFactory* factory = 0;

/////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////
DistributionFactory::DistributionFactory()
{

}

/////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////
DistributionFactory::~DistributionFactory()
{

}

/////////////////////////////////////////////////////////
/// \return {The singleton of the factory}
/////////////////////////////////////////////////////////
DistributionFactory* DistributionFactory::getInstance()
{
	if(!factory)
	{
		factory = new DistributionFactory();
	}
	return factory;
}
