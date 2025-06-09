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

namespace cpop {

class StepMax;
class PhysicsListMessenger;

class PhysicsList : public G4VModularPhysicsList {
public:
	PhysicsList();

	void ConstructParticle() override;
	void ConstructProcess() override;

	void ConstructEM();

	void AddPhysicsList(const G4String& name);

	PhysicsListMessenger& messenger();

	StepMax *step_max_process();

	void SetElectronCut(G4double);

	G4double fElectronCut;

private:
	void AddStepMax();
	void AddElectronCapture();

	std::unique_ptr<G4VPhysicsConstructor> _physicsList;
	G4String _name;
	StepMax* _stepMaxProcess = nullptr; //Not sure whether G4 owns it or not

	G4VPhysicsConstructor* _decayList;

	std::unique_ptr<PhysicsListMessenger> _messenger;
};

}

#endif
