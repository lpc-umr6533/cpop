#include "PrimaryGeneratorAction.hh"

namespace cpop {

PrimaryGeneratorAction::PrimaryGeneratorAction(PGA_impl &pga_impl):
	pga_impl_(&pga_impl)
{
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
	// pga_impl_->setNumberOfParticles(10);
	pga_impl_->GeneratePrimaries(event);
}

int PrimaryGeneratorAction::TotalEvent() const {
	return pga_impl_->TotalEvent();
}

UniformSource* PrimaryGeneratorAction::uniform_source() const {
	return pga_impl_->uniform_source();
}

DistributedSource *PrimaryGeneratorAction::distributed_source() const {
	return pga_impl_->distributed_source();
}

PGA_implMessenger &PrimaryGeneratorAction::messenger() {
	return pga_impl_->messenger();
}

}
