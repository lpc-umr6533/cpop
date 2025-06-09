#ifndef ADISTRIBUTION_HH
#define ADISTRIBUTION_HH

#include "Cell.hh"
#include "DistributionType.hh"
#include "SimulatedSubEnv.hh"

template<typename Kernel, typename Point, typename Vector>
class SimulatedSubEnv;

/// \brief Define a distribution for a given cell properties and a given worldLayer
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class ADistribution {
public:
	ADistribution(Distribution::DistributionType);
	virtual ~ADistribution() = default;

	///\brief function wich will distribute the cellPopProperties to the world layer
	virtual void distribute(SimulatedSubEnv<Kernel, Point, Vector>* pSubEnv, const CellProperties*, unsigned int, std::map<LifeCycles::LifeCycle, double>) = 0;
	///\brief distribute a simple cell on the environment and set is parameters ( position, life cycle...)
	virtual void distribute(SimulatedSubEnv<Kernel, Point, Vector>* pSubEnv, Cell<Kernel, Point, Vector>*, std::map<LifeCycles::LifeCycle, double>) = 0;

private:
	Distribution::DistributionType type;	///< \brief the type of distribution .
};

/// \param <pType> {} The distribution type }
template<typename Kernel, typename Point, typename Vector>
ADistribution<Kernel, Point, Vector>::ADistribution(Distribution::DistributionType pType):
	type(pType)
{
}

#endif
