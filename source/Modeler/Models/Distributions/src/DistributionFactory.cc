#include "DistributionFactory.hh"
#include "RandomCellDistribution.hh"

static DistributionFactory* factory = nullptr;

/// \return {The singleton of the factory}
DistributionFactory* DistributionFactory::getInstance() {
	if(!factory)
		factory = new DistributionFactory();
	return factory;
}
