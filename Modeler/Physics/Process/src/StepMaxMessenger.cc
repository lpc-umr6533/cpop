#include "StepMaxMessenger.hh"

#include "StepMax.hh"

namespace cpop {

StepMaxMessenger::StepMaxMessenger(StepMax *step_max)
    :MessengerBase(),
      step_max_(step_max)
{
}

void StepMaxMessenger::BuildCommands(G4String base)
{
    G4String cmd_base = base + "/stepMax";
    step_max_cmd_ = std::make_unique<G4UIcmdWithADoubleAndUnit>(cmd_base, this);
    step_max_cmd_->SetGuidance("Set max allowed step length");
    step_max_cmd_->SetParameterName("mxStep",false);
    step_max_cmd_->SetRange("mxStep>0.");
    step_max_cmd_->SetUnitCategory("Length");
}

void StepMaxMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == step_max_cmd_.get()) {
        step_max_->SetMaxStep(step_max_cmd_->GetNewDoubleValue(newValue));
    }
}

}
