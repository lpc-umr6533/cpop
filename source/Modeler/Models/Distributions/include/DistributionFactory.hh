#ifndef DISTRIBUTION_FACTORY_HH
#define DISTRIBUTION_FACTORY_HH

#include "ADistribution.hh"
#include "DistributionType.hh"
#include "RandomCellDistribution.hh"

/// \brief This distribution factory can create every kind of distribution existing
/// @author Henri Payno
class DistributionFactory {
public:
	/// \brief return an instance of factory
	static DistributionFactory* getInstance();
	/// \brief return the distribution corresponding to the distribution type
	template <typename Kernel, typename Point, typename Vector>
	ADistribution<Kernel, Point, Vector>* getDistribution(const Distribution::DistributionType); /// TODO : add dimensions ?
};

/// \return {return the requested distribution}
template <typename Kernel, typename Point, typename Vector>
ADistribution<Kernel, Point, Vector>* DistributionFactory::getDistribution(const Distribution::DistributionType distributionType) {
	switch(distributionType) {
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

#endif
