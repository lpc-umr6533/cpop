#include "UniformSourceMessenger.hh"
#include "UniformSource.hh"

namespace cpop {

UniformSourceMessenger::UniformSourceMessenger(UniformSource *source):
	_source(source),
	_sourceMessenger(std::make_unique<SourceMessenger>(source))
{
}

void UniformSourceMessenger::BuildCommands(G4String base) {
	_sourceMessenger->BuildCommands(base);

	G4String cmd_base = base + "/totalParticle";
	_totalParticleCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_base, this);
	_totalParticleCmd->SetGuidance("Set number of particles to be generated from this source");
	_totalParticleCmd->SetParameterName("TotalParticle", false);
	_totalParticleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void UniformSourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	_sourceMessenger->SetNewValue(command, newValue);

	if (command == _totalParticleCmd.get()) {
		_source->setTotal_particle(_totalParticleCmd->GetNewIntValue(newValue));
	}
}

}
