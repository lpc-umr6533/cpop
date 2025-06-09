#include "CpopActionInitialization.hh"

#include "CpopRunAction.hh"
#include "CpopEventAction.hh"
#include "CpopSteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Source.hh"

namespace cpop {

CpopActionInitialization::CpopActionInitialization(const Population &population):
	_pgaImpl(std::make_unique<PGA_impl>(population)),
	_population(&population)
{
	G4String pga_base = "/cpop/source";
	_pgaImpl->messenger().BuildCommands(pga_base);
}

void CpopActionInitialization::BuildForMaster() const {
	SetUserAction(new CpopRunAction(*_population));
}

void CpopActionInitialization::Build() const {
	// Build tuples
	auto* runAction = new CpopRunAction(*_population);
	SetUserAction(runAction);
	// Periodic logging
	auto* eventAction = new CpopEventAction(*_population, runAction);
	SetUserAction(eventAction);
	// Fill tuples
	SetUserAction(new CpopSteppingAction(*_population, eventAction, *_pgaImpl));
	// Primary generator
	auto* pga = new PrimaryGeneratorAction(*_pgaImpl);
	SetUserAction(pga);
}

}
