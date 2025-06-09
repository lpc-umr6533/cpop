#ifndef PGA_IMPLMESSENGER_HH
#define PGA_IMPLMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "MessengerBase.hh"

namespace cpop {

class PGA_impl;

class PGA_implMessenger : public MessengerBase {
public:
	PGA_implMessenger(PGA_impl* pga_impl);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	PGA_impl* _pgaImpl;

	G4String _base;

	// Commands
	/// \brief Add a uniform source
	std::unique_ptr<G4UIcmdWithAString> _uniformCmd;
	/// \brief Add a distributed source
	std::unique_ptr<G4UIcmdWithAString> _distributedCmd;
	/// \brief Activate radionuclide's daughter diffusion
	std::unique_ptr<G4UIcommand> _diffusionCmd;
	/// \brief Use a txt file to choose postions and directions of primary particles
	std::unique_ptr<G4UIcommand> _posiDirecTxtCmd;
	/// \brief Choose Li7 spectrum if the energy of the
	///usePositionsDirectionsTxt file contained Helium energies from BNCT reaction
	std::unique_ptr<G4UIcmdWithAString> _li7BNCTCmd;
	/// \brief Initialize the sources
	std::unique_ptr<G4UIcmdWithoutParameter> _initCmd;
};

}

#endif
