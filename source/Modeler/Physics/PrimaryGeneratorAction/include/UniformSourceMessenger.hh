#ifndef UNIFORMSOURCEMESSENGER_HH
#define UNIFORMSOURCEMESSENGER_HH

#include "G4UIcmdWithAnInteger.hh"

#include "MessengerBase.hh"
#include "SourceMessenger.hh"

namespace cpop {

class SourceMessenger;
class UniformSource;

class UniformSourceMessenger : public MessengerBase {
public:
	UniformSourceMessenger(UniformSource *source);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	UniformSource* _source;

	// We choose composition over inheritancce to avoid type casting
	std::unique_ptr<SourceMessenger> _sourceMessenger;

	// Commands
	std::unique_ptr<G4UIcmdWithAnInteger> _totalParticleCmd;
};

}

#endif
