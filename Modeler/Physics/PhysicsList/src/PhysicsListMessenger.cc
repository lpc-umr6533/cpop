#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"

#include "StepMax.hh"
#include "StepMaxMessenger.hh"

namespace cpop {

PhysicsListMessenger::PhysicsListMessenger(PhysicsList *physics_list)
    :MessengerBase(),
      physics_list_(physics_list)
{

}

void PhysicsListMessenger::BuildCommands(G4String base)
{

    G4String cmd_base = base + "/physicsList";
    select_physics_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    select_physics_->SetGuidance("Set physics list");
    select_physics_->SetParameterName("PhysicsList", false);
    G4String candidates = "emstandard emstandard_opt1 emstandard_opt2 emstandard_opt3 emstandard_opt4 emlivermore empenelope emlowenergy emDNAphysics emDNAphysics_opt2";
    select_physics_->SetCandidates(candidates.data());
    select_physics_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/stepMax";
    step_max_cmd_ = std::make_unique<G4UIcmdWithADoubleAndUnit>(cmd_base, this);
    step_max_cmd_->SetGuidance("Set max allowed step length");
    step_max_cmd_->SetParameterName("mxStep",false);
    step_max_cmd_->SetRange("mxStep>0.");
    step_max_cmd_->SetUnitCategory("Length");
}

void PhysicsListMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == select_physics_.get()) {
        physics_list_->AddPhysicsList(newValue);
    } else if (command == step_max_cmd_.get()) {
        physics_list_->step_max_process()->SetMaxStep(step_max_cmd_->GetNewDoubleValue(newValue));
    }
}

}
