#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include <memory>

#include "G4VModularPhysicsList.hh"
#include "PhysicsListMessenger.hh"

#include "globals.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4ElectronCapture.hh"

namespace  cpop {

class StepMax;
class PhysicsListMessenger;

class PhysicsList : public G4VModularPhysicsList
{
public:
    PhysicsList();
    ~PhysicsList() = default;

    void ConstructParticle();
    void ConstructProcess();

    void ConstructEM();

    void AddPhysicsList(const G4String& name);

    PhysicsListMessenger& messenger();

    StepMax *step_max_process();

private:
    void AddStepMax();
    void AddElectronCapture();

    std::unique_ptr<G4VPhysicsConstructor> physics_list_;
    G4String name_;
    StepMax* step_max_process_ = nullptr; //Not sure whether G4 owns it or not

    G4VPhysicsConstructor* decay_List;


    std::unique_ptr<PhysicsListMessenger> messenger_;
};

}
#endif // PHYSICSLIST_HH
