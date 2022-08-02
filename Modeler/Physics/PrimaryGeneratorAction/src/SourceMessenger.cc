#include "SourceMessenger.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "Source.hh"

namespace cpop {

SourceMessenger::SourceMessenger(Source *source)
    :MessengerBase(), source_(source)
{
    particle_table_ = G4ParticleTable::GetParticleTable();
}

void SourceMessenger::BuildCommands(G4String base)
{
    G4String cmd_base = base + "/particle";
    particle_cmd_ = make_unique<G4UIcmdWithAString>(cmd_base, this);
    particle_cmd_->SetGuidance("Set particle to be generated.");
    particle_cmd_->SetParameterName("ParticleName", false);
    G4String candidateList;
    G4ParticleTable::G4PTblDicIterator* itr = particle_table_->GetIterator();
    itr->reset();
    while( (*itr)() )
    {
        G4ParticleDefinition* pd = itr->value();
        if( !(pd->IsShortLived()) || pd->GetDecayTable() )
        {
            candidateList += pd->GetParticleName();
            candidateList += " ";
        }
    }
    particle_cmd_->SetCandidates(candidateList);
    particle_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/spectrum";
    user_spectrum_cmd_ = make_unique<G4UIcmdWithAString>(cmd_base, this);
    user_spectrum_cmd_->SetGuidance("Set spectrum file used to get energy (in MeV)");
    user_spectrum_cmd_->SetParameterName("UserSpectrum", false);
    user_spectrum_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);


}

void SourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == particle_cmd_.get()) {
        G4ParticleDefinition* pd = particle_table_->FindParticle(newValue);
        if(pd != 0) {
          source_->setParticle(pd);
        } else {
            std::string msg = "Particle [" + newValue + "] is not found.";
            particle_table_->DumpTable();
            throw std::runtime_error(msg);
        }
    } else if (command == user_spectrum_cmd_.get()) {
        source_->setUser_spectrum(newValue.data());
    }
}

}
