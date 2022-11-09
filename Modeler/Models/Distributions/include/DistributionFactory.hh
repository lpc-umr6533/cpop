/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DISTRIBUTION_FACTORY_HH
#define DISTRIBUTION_FACTORY_HH

#include "ADistribution.hh"
#include "DistributionType.hh"
#include "RandomCellDistribution.hh"

//////////////////////////////////////////////////////////////////////////////
/// \brief This distribution factory can create every kind of distribution existing
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class DistributionFactory
{
public:
	/// \brief constructor
	DistributionFactory();
	/// \brief destructor
	~DistributionFactory();

	/// \brief return an instance of factory
	static DistributionFactory* getInstance();
	/// \brief return the distribution corresponding to the distribution type
	template <typename Kernel, typename Point, typename Vector>
	ADistribution<Kernel, Point, Vector>* getDistribution(const Distribution::DistributionType); /// TODO : add dimensions ?
};


/////////////////////////////////////////////////////////
/// \return {return the requested distribution}
/////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
ADistribution<Kernel, Point, Vector>* DistributionFactory::getDistribution(const Distribution::DistributionType distributionType)
{
	switch(distributionType)
	{
		case Distribution::RANDOM:
		{
			return new RandomCellDistribution<Kernel, Point, Vector> ();
		}
		case Distribution::NORMALE:
		{
			/// TODO
		}
	}
	return NULL;
}

#endif // DISTRIBUTION_FACTORY_HH