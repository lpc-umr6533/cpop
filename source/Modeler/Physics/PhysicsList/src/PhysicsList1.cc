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

PhysicsList::PhysicsList()
    :G4VModularPhysicsList(),
      messenger_(std::make_unique<PhysicsListMessenger>(this))
{
    step_max_process_ = new StepMax();
}

void PhysicsList::ConstructParticle()
{
    // Construct all bosons
    G4BosonConstructor boson;
    boson.ConstructParticle();

    // Construct all leptons
    G4LeptonConstructor lepton;
    lepton.ConstructParticle();

    //  Construct all mesons
    G4MesonConstructor meson;
    meson.ConstructParticle();

    //  Construct all barions
    G4BaryonConstructor barion;
    barion.ConstructParticle();

    //  Construct light ions
    G4IonConstructor ion;
    ion.ConstructParticle();

    //  Construct  resonaces and quarks
    G4ShortLivedConstructor slive;
    slive.ConstructParticle();

    //Construct G4DNA particles


    G4DNAGenericIonsManager* dnagenericIonsManager;
    dnagenericIonsManager=G4DNAGenericIonsManager::Instance();
    dnagenericIonsManager->GetIon("hydrogen");
    dnagenericIonsManager->GetIon("alpha+");
    dnagenericIonsManager->GetIon("alpha++");
    dnagenericIonsManager->GetIon("helium");
    dnagenericIonsManager->GetIon("carbon");
    dnagenericIonsManager->GetIon("nitrogen");
    dnagenericIonsManager->GetIon("iron");
    dnagenericIonsManager->GetIon("oxygen");
}

void PhysicsList::ConstructProcess()
{
    // Transportation
    //
    AddTransportation();

    // Electromagnetic physics list
    //
    physics_list_->ConstructProcess();

    // step limitation (as a full process)
    //
    AddStepMax();
}

void PhysicsList::AddPhysicsList(const G4String &name)
{
    if (verboseLevel>1) {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">\n";
    }

    if (name == name_) return;

    if (name == "emstandard") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics>(verboseLevel);

    } else if (name == "emstandard_opt1") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option1>(verboseLevel);

    } else if (name == "emstandard_opt2") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option2>(verboseLevel);

    } else if (name == "emstandard_opt3") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option3>(verboseLevel);

    } else if (name == "emstandard_opt4") {

        name_ = name;
        physics_list_ = std::make_unique<G4EmStandardPhysics_option4>(verboseLevel);

    } else if (name == "emlivermore") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmLivermorePhysics>(verboseLevel);

    } else if (name == "empenelope") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmPenelopePhysics>(verboseLevel);

    } else if (name == "emlowenergy") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmLowEPPhysics>(verboseLevel);

    } else if (name == "emDNAphysics") {
        name_ = name;
        physics_list_ = std::make_unique<G4EmDNAPhysics>(verboseLevel);

    } else {
        std::cout << "PhysicsList::AddPhysicsList: <" << name << ">"
               << " is not defined\n";
    }
}

PhysicsListMessenger &PhysicsList::messenger()
{
    return *messenger_;
}

void PhysicsList::AddStepMax()
{
    auto theParticleIterator = this->GetParticleIterator();
    theParticleIterator->reset();
    while ((*theParticleIterator)()){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();

        if (step_max_process_->IsApplicable(*particle) && pmanager)
        {
            pmanager ->AddDiscreteProcess(new StepMax(*step_max_process_));
        }
    }
}

StepMax *PhysicsList::step_max_process()
{
    return step_max_process_;
}

}


