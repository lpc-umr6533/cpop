#ifndef PHYSICSLISTMESSENGER_HH
#define PHYSICSLISTMESSENGER_HH

#include "MessengerBase.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

namespace cpop {

class PhysicsList;

class PhysicsListMessenger : public MessengerBase {
public:
	PhysicsListMessenger(PhysicsList* physics_list);

	void BuildCommands(G4String base) override;
	void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
	PhysicsList* _physicsList;

	/// \brief Select a eletromagnetic physics list
	std::unique_ptr<G4UIcmdWithAString> _selectPhysics;
	std::unique_ptr<G4UIcmdWithADoubleAndUnit> _stepMaxCmd;
	std::unique_ptr<G4UIcmdWithADoubleAndUnit> _electronCutCmd;
};

}



#endif // PHYSICSLISTMESSENGER_HH
