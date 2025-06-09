#include "ActionInitialization.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Source.hh"

namespace cpop {

ActionInitialization::ActionInitialization(const Population &population):
	_pgaImpl(std::make_unique<PGA_impl>(population)),
	_population(&population)
{
	G4String pga_base = "/cpop/source";
	_pgaImpl->messenger().BuildCommands(pga_base);
}

void ActionInitialization::BuildForMaster() const {
	SetUserAction(new RunAction());
}

void ActionInitialization::Build() const {
	// Build tuples
	SetUserAction(new RunAction());
	// Periodic logging
	SetUserAction(new EventAction());
	// Fill tuples
	SetUserAction(new SteppingAction(*_population));
	// Primary generator
	auto* pga = new PrimaryGeneratorAction(*_pgaImpl);
	SetUserAction(pga);
}

}
