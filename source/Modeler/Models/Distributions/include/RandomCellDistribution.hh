#ifndef RANDOM_CELL_DISTRIBUTION_HH
#define RANDOM_CELL_DISTRIBUTION_HH

#include "ADistribution.hh"
#include "RandomEngineManager.hh"
#include "SimulatedSubEnv.hh"
#include "RoundCell.hh"
#include "Round_Shape.hh"
#include "RoundNucleus.hh"

/// \brief Define a random cell distribution inside a world.
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class RandomCellDistribution : public ADistribution<Kernel, Point, Vector> {
public:
	RandomCellDistribution();

	/// \brief function to distribute agent from a cell population propoerties inside a world layer
	void distribute(SimulatedSubEnv<Kernel, Point, Vector>*, const CellProperties*, unsigned int, std::map<LifeCycles::LifeCycle, double>);
	///\brief distribute a simple cell on the environment and set is parameters ( position, life cycle...)
	void distribute(SimulatedSubEnv<Kernel, Point, Vector>*, Cell<Kernel, Point, Vector>*, std::map<LifeCycles::LifeCycle, double>);
};

///////////////////////////////// FUNCTION DEFINITIONS ////////////////////////////////////////
#define DEBUG_RANDOM_CELL_DISTRIB 0
#include <cassert>

#include "CellFactory.hh"
#include "InformationSystemManager.hh"
#include "DistributionType.hh"

template <typename Kernel, typename Point, typename Vector>
RandomCellDistribution<Kernel, Point, Vector>::RandomCellDistribution():
	ADistribution<Kernel, Point, Vector>(Distribution::RANDOM)
{
}

/// \param pSubEnv The world layer to insert the cells
/// \param pCellProperties the properties to set to cells
/// \param nbCell The number of cell we want to add to the subEnvironment
/// \param pLifeCycles the rates of each life cycle to add to the sub environment
template <typename Kernel, typename Point, typename Vector>
void RandomCellDistribution<Kernel, Point, Vector>::distribute(
	SimulatedSubEnv<Kernel, Point, Vector>* pSubEnv,
	const CellProperties* pCellProperties,
	unsigned int nbCell,
	std::map<LifeCycles::LifeCycle, double > pLifeCycles)
{
	assert(pSubEnv);

	if(DEBUG_RANDOM_CELL_DISTRIB) {
		QString mess = "ask for distribution with template parameters : " +
		QString(typeid(Kernel).name()) + ", " + QString(typeid(Point).name()) + ", " + QString(typeid(Vector).name()) + ", ";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "RandomCellDistribution");
	}

	int nbCellGenerated = 0;

	auto lastState = pLifeCycles.end();
	lastState --;
	for(auto itLSR = pLifeCycles.begin(); itLSR != pLifeCycles.end(); ++itLSR) {
		int nbPopCellToStateN;
		/// make sure we have the right number and no conversion approximation
		if(itLSR == lastState) {
			nbPopCellToStateN = nbCell - nbCellGenerated;
		} else {
			nbPopCellToStateN = itLSR->second * nbCell;
		}

		int lLocalGenerated = 0;
		while(lLocalGenerated < nbPopCellToStateN) {
			auto* newCell = CellFactory::getInstance()->produce<Kernel, Point, Vector>(pCellProperties, itLSR->first);

			/// define the spot / position
			Point spot = pSubEnv->getSpatialDelimitation()->getSpot(Distribution::RANDOM);
			/// set position and orientation randomly
			newCell->setPosition(spot);
			/// \todo : set LifeCycle according to a specifique distribution...
			newCell->setLifeCycle(itLSR->first);
			pSubEnv->addAgent(newCell);

			lLocalGenerated++;
		}

		nbCellGenerated +=lLocalGenerated;
	}

	if(DEBUG_RANDOM_CELL_DISTRIB) {
		QString message = "nb cell created : " + QString::number(nbCellGenerated);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, message.toStdString(), "Random distribution" );
	}
}

/// \param pSubEnv The sub environment ( world layer ) where to insert the cells
/// \param pCell The cell to set
/// \param pLifeCycleRates Rates of each life cycles
template <typename Kernel, typename Point, typename Vector>
void RandomCellDistribution<Kernel, Point, Vector>::distribute(
	SimulatedSubEnv<Kernel, Point, Vector>* pSubEnv,
	Cell<Kernel, Point, Vector>* pCell,
	std::map<LifeCycles::LifeCycle, double> pLifeCycleRates)
{
	assert(pCell);
	assert(pSubEnv);
	assert(pLifeCycleRates.size()>0);

	// set position
	Point spot = pSubEnv->getSpatialDelimitation()->getSpot(Distribution::RANDOM);
	// set position and orientation randomly
	pCell->setPosition(spot);

	// set life state
	double LifeCycle = RandomEngineManager::getInstance()->randd(0., 1.);
	double pourcentage = 0.;
	auto itLC = pLifeCycleRates.begin();
	while(itLC != pLifeCycleRates.end()) {
		pourcentage += itLC->second;
		if(pourcentage >= LifeCycle) {
			pCell->setLifeCycle( itLC->first );
			itLC = pLifeCycleRates.end();
		} else {
			++itLC;
		}
	}

	// set nuclei radius
	std::vector<Nucleus<double, Point, Vector>*> nuclei = pCell->getNuclei();
	typename std::vector<Nucleus<double, Point, Vector>*>::iterator itNucleus;
	for(itNucleus = nuclei.begin(); itNucleus != nuclei.end(); ++itNucleus) {
		// if nucleus is a round nucleus
		auto* nucleus = static_cast<RoundNucleus<Kernel, Point, Vector>*> (*itNucleus);
		if(nucleus) {
			// set nucleus radius
			double nucleusRadius = RandomEngineManager::getInstance()->randd(
				pCell->getCellProperties()->getNucleusRadius(pCell->getLifeCycle()).var_min(),
				pCell->getCellProperties()->getNucleusRadius(pCell->getLifeCycle()).var_max()
			);
			assert(nucleusRadius > 0.);
			nucleus->setRadius(nucleusRadius);
		}
	}

	// if is ROUND Sphere : set radius
	auto const* roundProp = dynamic_cast<const RoundCellProperties*> (pCell->getCellProperties());
	if(roundProp) {
		auto* roundCell = dynamic_cast<RoundCell<Kernel, Point, Vector>* > (pCell);
		if(roundCell) {
			double cellRadius = RandomEngineManager::getInstance()->randd(
				(roundProp->getMembraneRadius(pCell->getLifeCycle())).var_min(),
				(roundProp->getMembraneRadius(pCell->getLifeCycle())).var_max()
			);
			roundCell->setRadius(cellRadius);
		}
	}

	/// add agent to the spatialable
	pSubEnv->addAgent(pCell);
}

#endif
