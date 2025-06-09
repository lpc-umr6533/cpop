#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

namespace cpop {

PhysicsListMessenger::PhysicsListMessenger(PhysicsList *physics_list):
	_physicsList(physics_list)
{
}

void PhysicsListMessenger::BuildCommands(G4String base) {
	G4String cmd_base = base + "/physicsList";
	_selectPhysics = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_selectPhysics->SetGuidance("Set physics list");
	_selectPhysics->SetParameterName("PhysicsList", false);

	G4String candidates = "emstandard emstandard_opt1 emstandard_opt2 emstandard_opt3 emstandard_opt4 emlivermore empenelope emlowenergy emDNAphysics emDNAphysics_opt2 emDNAphysics_opt4 emDNAphysics_opt6";
	_selectPhysics->SetCandidates(candidates.data());
	_selectPhysics->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/stepMax";
	_stepMaxCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>(cmd_base, this);
	_stepMaxCmd->SetGuidance("Set max allowed step length");
	_stepMaxCmd->SetParameterName("mxStep",false);
	_stepMaxCmd->SetRange("mxStep>0.");
	_stepMaxCmd->SetUnitCategory("Length");

	cmd_base = base + "/electronCut";
	_electronCutCmd = std::make_unique<G4UIcmdWithADoubleAndUnit>(cmd_base, this);
	_electronCutCmd->SetGuidance("Set energy below which electrons are killed");
	_electronCutCmd->SetParameterName("elCut",false);
	_electronCutCmd->SetRange("elCut>0.");
	_electronCutCmd->SetUnitCategory("Energy");
}

void PhysicsListMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if (command == _selectPhysics.get()) {
		_physicsList->AddPhysicsList(newValue);
	} else if (command == _stepMaxCmd.get()) {
		_physicsList->step_max_process()->SetMaxStep(_stepMaxCmd->GetNewDoubleValue(newValue));
	} else if (command == _electronCutCmd.get()) {
		_physicsList->SetElectronCut(_electronCutCmd->GetNewDoubleValue(newValue));
	}
}

}
