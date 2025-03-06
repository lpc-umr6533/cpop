#include "CpopActionInitialization.hh"

#include "CpopRunAction.hh"
#include "CpopEventAction.hh"
#include "CpopSteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Source.hh"

namespace cpop {

CpopActionInitialization::CpopActionInitialization(const Population &population)
    :G4VUserActionInitialization(),
      pga_impl_(std::make_unique<PGA_impl>(population)),
      population_(&population)
{
    G4String pga_base = "/cpop/source";
    pga_impl_->messenger().BuildCommands(pga_base);
}

void CpopActionInitialization::BuildForMaster() const
{
    SetUserAction(new CpopRunAction(*population_));
}

void CpopActionInitialization::Build() const
{
    // Build tuples
    CpopRunAction* runAction = new CpopRunAction(*population_);
    SetUserAction(runAction);
    // Periodic logging
    CpopEventAction* eventAction = new CpopEventAction(*population_, runAction);
    SetUserAction(eventAction);
    // Fill tuples
    SetUserAction(new CpopSteppingAction(*population_, eventAction, *pga_impl_));
    // Primary generator
    PrimaryGeneratorAction* pga = new PrimaryGeneratorAction(*pga_impl_);
    SetUserAction(pga);
}

}
