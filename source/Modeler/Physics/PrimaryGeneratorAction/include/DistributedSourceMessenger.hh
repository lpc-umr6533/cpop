#ifndef DISTRIBUTEDSOURCEMESSENGER_HH
#define DISTRIBUTEDSOURCEMESSENGER_HH

#include "G4UIcommand.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"

#include "MessengerBase.hh"
#include "SourceMessenger.hh"

namespace cpop {

class SourceMessenger;
class DistributedSource;

class DistributedSourceMessenger : public MessengerBase {
	/// Victor Levrague :addition of maximum particle per cell, % of cell labeling and different positions for all particles generated on the same cell ///
public:
	DistributedSourceMessenger(DistributedSource* source);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand* command , G4String newValue) override;

private:
	DistributedSource* _source;

	// We choose composition over inheritancce to avoid type casting
	std::unique_ptr<SourceMessenger> _sourceMessenger;

	// Commands
	/// \brief Set the number of distributed sources in the population
	std::unique_ptr<G4UIcmdWithAnInteger> _totalSourceCmd;
	/// \brief Set the number of particles to be generated from one source
	std::unique_ptr<G4UIcmdWithAnInteger> _partPerSourceCmd;
	/// \brief Distribute the nanoparticle in the 3 regions
	std::unique_ptr<G4UIcmdWith3Vector> _nanoPerRegionCmd;
	/// \brief Define the secondaries repartition in a cell
	std::unique_ptr<G4UIcommand> _posInCellCmd;
	/// \brief Maximum number of sources per cell to be generated, in each region
	std::unique_ptr<G4UIcmdWith3Vector> _maxSourcesPerCellCmd;
	/// \brief Percentage of labeled cells, in each region
	std::unique_ptr<G4UIcmdWith3Vector> _cellLabelingCmd;
	/// \brief Set the number of nanoparticle in the population
	std::unique_ptr<G4UIcmdWithAnInteger> _onlyOnePositionForAllParticlesOnACellCmd;
	/// \brief Define the choice of lognormal distribution for particles in cells
	std::unique_ptr<G4UIcommand> _logNormalDistribCmd;
};

}

#endif
