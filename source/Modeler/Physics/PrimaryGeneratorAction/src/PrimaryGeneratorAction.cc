#include "PrimaryGeneratorAction.hh"

namespace cpop {

PrimaryGeneratorAction::PrimaryGeneratorAction(PGA_impl &pga_impl)
    :G4VUserPrimaryGeneratorAction(),
      pga_impl_(&pga_impl)
{

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    pga_impl_->GeneratePrimaries(event);
}

int PrimaryGeneratorAction::TotalEvent() const
{
    return pga_impl_->TotalEvent();
}

HomogeneousSource* PrimaryGeneratorAction::homogeneous_source() const {
    return pga_impl_->homogeneous_source();
}

NanoparticleSource *PrimaryGeneratorAction::nanoparticle_source() const
{
    return pga_impl_->nanoparticle_source();
}

PGA_implMessenger &PrimaryGeneratorAction::messenger()
{
    return pga_impl_->messenger();
}

}
