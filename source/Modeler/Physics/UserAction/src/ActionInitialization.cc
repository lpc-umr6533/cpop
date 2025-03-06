#include "ActionInitialization.hh"

#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Source.hh"

namespace cpop {

ActionInitialization::ActionInitialization(const Population &population)
    :G4VUserActionInitialization(),
      pga_impl_(make_unique<PGA_impl>(population)),
      population_(&population)
{
    G4String pga_base = "/cpop/source";
    pga_impl_->messenger().BuildCommands(pga_base);
}

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    // Build tuples
    SetUserAction(new RunAction());
    // Periodic logging
    SetUserAction(new EventAction());
    // Fill tuples
    SetUserAction(new SteppingAction(*population_));
    // Primary generator
    PrimaryGeneratorAction* pga = new PrimaryGeneratorAction(*pga_impl_);
    SetUserAction(pga);
}

}
