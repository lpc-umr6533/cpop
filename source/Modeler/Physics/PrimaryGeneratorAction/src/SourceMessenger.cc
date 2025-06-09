#include "SourceMessenger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "Source.hh"

namespace cpop {

SourceMessenger::SourceMessenger(Source *source):
	_source(source)
{
	_particleTable = G4ParticleTable::GetParticleTable();
}

void SourceMessenger::BuildCommands(G4String base) {
	G4String cmd_base = base + "/particle";
	_particleCmd = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_particleCmd->SetGuidance("Set particle to be generated.");
	_particleCmd->SetParameterName("ParticleName", false);
	G4String candidateList;

	G4ParticleTable::G4PTblDicIterator* itr = _particleTable->GetIterator();
	itr->reset();

	while((*itr)()) {
		G4ParticleDefinition* pd = itr->value();
		if(!(pd->IsShortLived()) || pd->GetDecayTable()) {
			candidateList += pd->GetParticleName();
			candidateList += " ";
		}
	}

	_particleCmd->SetCandidates(candidateList);
	_particleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/spectrum";
	_userSpectrumCmd = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_userSpectrumCmd->SetGuidance("Set spectrum file used to get energy (in MeV)");
	_userSpectrumCmd->SetParameterName("UserSpectrum", false);
	_userSpectrumCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/ion";
	_ionCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	_ionCmd->SetGuidance("Set ion to be generated.");
	auto* atomic_number = new G4UIparameter("Z", 'd', false);
	_ionCmd->SetParameter(atomic_number);
	auto* atomic_mass = new G4UIparameter("A", 'd', false);
	_ionCmd->SetParameter(atomic_mass);
	_ionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void SourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if (command == _particleCmd.get()) {
		G4ParticleDefinition* pd = _particleTable->FindParticle(newValue);
		if(pd) {
			_source->setParticle(pd);
		} else {
			std::string msg = "Particle [" + newValue + "] is not found.";
			_particleTable->DumpTable();
			throw std::runtime_error(msg);
		}
	} else if (command == _userSpectrumCmd.get()) {
		_source->setUser_spectrum(newValue.data());
	} else if (command == _ionCmd.get()) {
		G4int atomic_number;
		G4int atomic_mass;

		std::istringstream is(newValue.data());
		is >> atomic_number >> atomic_mass;

		_source->setIon(atomic_number, atomic_mass);
	}
}

}
