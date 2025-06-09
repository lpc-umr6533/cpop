#ifndef STEPMAXMESSENGER_HH
#define STEPMAXMESSENGER_HH

#include "G4UIcmdWithADoubleAndUnit.hh"

#include "MessengerBase.hh"

namespace cpop {

class StepMax;

class StepMaxMessenger : public MessengerBase {
public:
	StepMaxMessenger(StepMax* step_max);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	StepMax* _stepMax;

	std::unique_ptr<G4UIcmdWithADoubleAndUnit> _stepMaxCmd;

};

}

#endif
