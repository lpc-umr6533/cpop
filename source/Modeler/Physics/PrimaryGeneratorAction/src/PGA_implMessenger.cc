#include "PGA_implMessenger.hh"

#include "PGA_impl.hh"
#include "UniformSource.hh"
#include "DistributedSource.hh"

namespace cpop {

PGA_implMessenger::PGA_implMessenger(PGA_impl *pga_impl):
	_pgaImpl(pga_impl)
{
}

void PGA_implMessenger::BuildCommands(G4String base) {
	// Save the base for source.BuildCommands()
	_base = base;

	G4String cmd_base = base + "/addUniform";
	_uniformCmd = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_uniformCmd->SetGuidance("Add a uniform source");
	_uniformCmd->SetParameterName("HsourceName", false);
	_uniformCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	cmd_base = base + "/addDistribution";
	_distributedCmd = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_distributedCmd->SetGuidance("Add a distributed source");
	_distributedCmd->SetParameterName("NsourceName", false);
	_distributedCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	cmd_base = base + "/daughterDiffusion";
	_diffusionCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	_diffusionCmd->SetGuidance("Activate daughter diffusion");
	auto* bool_diffusion = new G4UIparameter("bool_diffusion", 's', false);
	_diffusionCmd->SetParameter(bool_diffusion);
	auto* half_life = new G4UIparameter("half_life", 'f', false);
	_diffusionCmd->SetParameter(half_life);
	_diffusionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	cmd_base = base + "/usePositionsDirectionsTxt";
	_posiDirecTxtCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	_posiDirecTxtCmd->SetGuidance("usePositionsDirectionsTxt");
	auto* name_file = new G4UIparameter("name_file", 's', false);
	_posiDirecTxtCmd->SetParameter(name_file);
	auto* name_method = new G4UIparameter("name_method", 's', false);
	_posiDirecTxtCmd->SetParameter(name_method);
	_posiDirecTxtCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	cmd_base = base + "/EmitLi7BNCTSpectrum";
	_li7BNCTCmd = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
	_li7BNCTCmd->SetGuidance(
		"Choose Li7 spectrum if the energy of the"
		"usePositionsDirectionsTxt file contained Helium energies from BNCT reaction"
	);
	_li7BNCTCmd->SetParameterName("EmitLi7BNCTSpectrum", false);
	_li7BNCTCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

	cmd_base = base + "/init";
	_initCmd = std::make_unique<G4UIcmdWithoutParameter>(cmd_base, this);
	_initCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void PGA_implMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if (command == _uniformCmd.get()) {
		UniformSource& added_source =  _pgaImpl->addUniformSource(newValue.data());
		G4String source_base = _base + "/" + newValue;
		added_source.messenger().BuildCommands(source_base);
	} else if (command == _distributedCmd.get()) {
		DistributedSource& added_source = _pgaImpl->addDistributedSource(newValue.data());
		G4String source_base = _base + "/" + newValue;
		added_source.messenger().BuildCommands(source_base);
	} else if (command == _diffusionCmd.get()) {
		G4String bool_diffusion;
		G4double half_life; //s
		std::istringstream is(newValue.data());
		is >> bool_diffusion >> half_life;
		_pgaImpl->ActivateDiffusion(bool_diffusion, half_life);
	} else if (command == _initCmd.get()) {
		_pgaImpl->Initialize();
	} else if (command == _posiDirecTxtCmd.get()) {
		G4String name_file;
		G4String name_method;

		std::istringstream is(newValue.data());
		is >> name_file >> name_method;

		_pgaImpl->SetTxtInfoPrimariesName_and_MethodName(name_file, name_method);
	} else if (command == _li7BNCTCmd.get()) {
		_pgaImpl->SetLi7BNCTSpectrumBool(newValue);
	}
}

}
