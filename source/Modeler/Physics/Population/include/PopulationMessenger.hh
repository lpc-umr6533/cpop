#ifndef POPULATIONMESSENGER_HH
#define POPULATIONMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "MessengerBase.hh"

namespace cpop {

class Population;

class PopulationMessenger : public MessengerBase {
public:
	PopulationMessenger(Population* population);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	Population* _population;

	// Commands
	/// \brief Set verbose level
	std::unique_ptr<G4UIcmdWithAnInteger> _verboseCmd;
	/// \brief Set population file
	std::unique_ptr<G4UIcmdWithAString> _populationCmd;
	/// \brief Set number of facet per poly
	std::unique_ptr<G4UIcmdWithAnInteger> _numberFacetCmd;
	/// \brief Set delta refinement
	std::unique_ptr<G4UIcmdWithADouble> _deltaRefCmd;
	/// \brief Set internal layer ratio
	std::unique_ptr<G4UIcmdWithADouble> _internalRatioCmd;
	/// \brief Set intermediary layer ratio
	std::unique_ptr<G4UIcmdWithADouble> _intermediaryRatioCmd;
	/// \brief Set number of sampling cell
	std::unique_ptr<G4UIcmdWithAnInteger> _numberSamplingCmd;
	/// \brief Initialize population and regions
	std::unique_ptr<G4UIcmdWithoutParameter> _initCmd;
	/// \brief Enable writing of infos about primaries in a .txt
	std::unique_ptr<G4UIcommand> _infosPrimariesCmd;
	/// \brief Bool to get info at the stepping level, i.e.  all energy deposited and positions for each step
	std::unique_ptr<G4UIcmdWithAnInteger> _getSteppingLevelInfoCmd;
	/// \brief  Bool to get info at the event level, i.e.  all entrance and exit energies of alpha particles in nuclei
	std::unique_ptr<G4UIcmdWithAnInteger> _getEventLevelInfoCmd;
};

}

#endif
