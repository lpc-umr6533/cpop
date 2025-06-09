#include "PhysicsList.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4DNAGenericIonsManager.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmLowEPPhysics.hh"
#include "G4EmDNAPhysics.hh"

#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"

#include "StepMax.hh"

namespace cpop {

PhysicsList::PhysicsList():
	G4VModularPhysicsList(),
	_messenger(std::make_unique<PhysicsListMessenger>(this))
{
	_stepMaxProcess = new StepMax();
}

void PhysicsList::ConstructParticle()
{
	G4BosonConstructor::ConstructParticle();
	G4LeptonConstructor::ConstructParticle();
	G4MesonConstructor::ConstructParticle();
	G4BaryonConstructor::ConstructParticle();
	G4IonConstructor::ConstructParticle();
	G4ShortLivedConstructor::ConstructParticle();

	auto* dnagenericIonsManager = G4DNAGenericIonsManager::Instance();
	dnagenericIonsManager->GetIon("hydrogen");
	dnagenericIonsManager->GetIon("alpha+");
	dnagenericIonsManager->GetIon("alpha++");
	dnagenericIonsManager->GetIon("helium");
	dnagenericIonsManager->GetIon("carbon");
	dnagenericIonsManager->GetIon("nitrogen");
	dnagenericIonsManager->GetIon("iron");
	dnagenericIonsManager->GetIon("oxygen");
}

void PhysicsList::ConstructProcess() {
	AddTransportation();
	_physicsList->ConstructProcess();
	AddStepMax();
}

void PhysicsList::AddPhysicsList(const G4String &name) {
	if (verboseLevel>1)
		std::cout << "PhysicsList::AddPhysicsList: <" << name << ">\n";

	if (name == _name) return;

	if (name == "emstandard") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics>(verboseLevel);
	} else if (name == "emstandard_opt1") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option1>(verboseLevel);
	} else if (name == "emstandard_opt2") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option2>(verboseLevel);
	} else if (name == "emstandard_opt3") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option3>(verboseLevel);
	} else if (name == "emstandard_opt4") {
		_name = name;
		_physicsList = std::make_unique<G4EmStandardPhysics_option4>(verboseLevel);
	} else if (name == "emlivermore") {
		_name = name;
		_physicsList = std::make_unique<G4EmLivermorePhysics>(verboseLevel);
	} else if (name == "empenelope") {
		_name = name;
		_physicsList = std::make_unique<G4EmPenelopePhysics>(verboseLevel);
	} else if (name == "emlowenergy") {
		_name = name;
		_physicsList = std::make_unique<G4EmLowEPPhysics>(verboseLevel);
	} else if (name == "emDNAphysics") {
		_name = name;
		_physicsList = std::make_unique<G4EmDNAPhysics>(verboseLevel);
	} else {
		std::cout << "PhysicsList::AddPhysicsList: <" << name << ">" << " is not defined\n";
	}
}

PhysicsListMessenger &PhysicsList::messenger() {
	return *_messenger;
}

void PhysicsList::AddStepMax() {
	auto theParticleIterator = GetParticleIterator();
	theParticleIterator->reset();
	while ((*theParticleIterator)()) {
		auto* particle = theParticleIterator->value();
		auto* pmanager = particle->GetProcessManager();

		if (_stepMaxProcess->IsApplicable(*particle) && pmanager)
			pmanager ->AddDiscreteProcess(new StepMax(*_stepMaxProcess));
	}
}

StepMax *PhysicsList::step_max_process() {
	return _stepMaxProcess;
}

}
