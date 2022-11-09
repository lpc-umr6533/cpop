#include "PGA_implMessenger.hh"

#include "PGA_impl.hh"
#include "HomogeneousSource.hh"
#include "NanoparticleSource.hh"

namespace cpop {

PGA_implMessenger::PGA_implMessenger(PGA_impl *pga_impl)
    :MessengerBase(), pga_impl_(pga_impl)
{

}

void PGA_implMessenger::BuildCommands(G4String base)
{
    // Save the base for source.BuildCommands()
    base_ = base;

    G4String cmd_base = base + "/addHomogeneous";
    homogeneous_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    homogeneous_cmd_->SetGuidance("Add a homogeneous source");
    homogeneous_cmd_->SetParameterName("HsourceName", false);
    homogeneous_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    cmd_base = base + "/addNanoparticle";
    nanoparticle_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_base, this);
    nanoparticle_cmd_->SetGuidance("Add a nanoparticle source");
    nanoparticle_cmd_->SetParameterName("NsourceName", false);
    nanoparticle_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

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
    if (command == homogeneous_cmd_.get()) {
        HomogeneousSource& added_source =  pga_impl_->addHomogeneousSource(newValue.data());
        G4String source_base = base_ + "/" + newValue;
        added_source.messenger().BuildCommands(source_base);
    } else if (command == nanoparticle_cmd_.get()) {
        NanoparticleSource& added_source = pga_impl_->addNanoparticleSource(newValue.data());
        G4String source_base = base_ + "/" + newValue;
        added_source.messenger().BuildCommands(source_base);
    } else if (command == diffusion_cmd_.get()) {
        pga_impl_->ActivateDiffusion(newValue);
    } else if (command == init_cmd_.get()) {
        pga_impl_->Initialize();
    }
}

}
