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
    SetUserAction(new RunAction(*population_));
}

void ActionInitialization::Build() const
{
    // Build tuples
    RunAction* runAction = new RunAction(*population_);
    SetUserAction(runAction);
    // Periodic logging
    EventAction* eventAction = new EventAction(*population_, runAction);
    SetUserAction(eventAction);
    // Fill tuples
    SetUserAction(new SteppingAction(*population_, eventAction, *pga_impl_));
    // Primary generator
    PrimaryGeneratorAction* pga = new PrimaryGeneratorAction(*pga_impl_);
    SetUserAction(pga);
}

}
