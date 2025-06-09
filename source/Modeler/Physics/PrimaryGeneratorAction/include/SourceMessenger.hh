#ifndef SOURCEMESSENGER_HH
#define SOURCEMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"

class G4ParticleTable;

#include "MessengerBase.hh"

namespace cpop {

class Source;

class SourceMessenger : public MessengerBase {
public:
	SourceMessenger(Source* source);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	Source* _source;

	// Commands
	/// \brief Set particle to be generated
	std::unique_ptr<G4UIcmdWithAString> _particleCmd;
	/// \brief Set ion to be generated
	std::unique_ptr<G4UIcommand> _ionCmd;
	/// \brief Set spectrum file
	std::unique_ptr<G4UIcmdWithAString> _userSpectrumCmd;
	/// \brief Particle table
	G4ParticleTable* _particleTable;

};

}

#endif
