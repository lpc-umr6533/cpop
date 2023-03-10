#include "PGA_implMessenger.hh"

#include "PGA_impl.hh"
#include "UniformSource.hh"
#include "DistributedSource.hh"

namespace cpop {

PGA_implMessenger::PGA_implMessenger(PGA_impl *pga_impl)
    :MessengerBase(), pga_impl_(pga_impl)
{

}

void PGA_implMessenger::BuildCommands(G4String base)
{
    // Save the base for source.BuildCommands()
    base_ = base;

    G4String cmd_base = base + "/addUniform";
    uniform_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    uniform_cmd_->SetGuidance("Add a uniform source");
    uniform_cmd_->SetParameterName("HsourceName", false);
    uniform_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    cmd_base = base + "/addDistribution";
    distributed_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    distributed_cmd_->SetGuidance("Add a distributed source");
    distributed_cmd_->SetParameterName("NsourceName", false);
    distributed_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    cmd_base = base + "/daughterDiffusion";
    diffusion_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    diffusion_cmd_->SetGuidance("Activate daughter diffusion");
    diffusion_cmd_->SetParameterName("Daughterdiffusion", false);
    diffusion_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    cmd_base = base + "/init";
    init_cmd_ = std::make_unique<G4UIcmdWithoutParameter>(cmd_base, this);
    init_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);
}

void PGA_implMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == uniform_cmd_.get()) {
        UniformSource& added_source =  pga_impl_->addUniformSource(newValue.data());
        G4String source_base = base_ + "/" + newValue;
        added_source.messenger().BuildCommands(source_base);
    } else if (command == distributed_cmd_.get()) {
        DistributedSource& added_source = pga_impl_->addDistributedSource(newValue.data());
        G4String source_base = base_ + "/" + newValue;
        added_source.messenger().BuildCommands(source_base);
    } else if (command == diffusion_cmd_.get()) {
        pga_impl_->ActivateDiffusion(newValue);
    } else if (command == init_cmd_.get()) {
        pga_impl_->Initialize();
    }
}

}
