#include "StepMaxMessenger.hh"

#include "StepMax.hh"

namespace cpop {

StepMaxMessenger::StepMaxMessenger(StepMax *step_max):
	_stepMax(step_max)
{
}

void StepMaxMessenger::BuildCommands(G4String base) {
	G4String cmd_base = base + "/stepMax";
	_stepMaxCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>(cmd_base, this);
	_stepMaxCmd->SetGuidance("Set max allowed step length");
	_stepMaxCmd->SetParameterName("mxStep",false);
	_stepMaxCmd->SetRange("mxStep>0.");
	_stepMaxCmd->SetUnitCategory("Length");
}

void StepMaxMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if (command == _stepMaxCmd.get()) {
		_stepMax->SetMaxStep(_stepMaxCmd->GetNewDoubleValue(newValue));
	}
}

}
