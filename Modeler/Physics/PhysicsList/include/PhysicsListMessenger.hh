#ifndef PHYSICSLISTMESSENGER_HH
#define PHYSICSLISTMESSENGER_HH

#include "MessengerBase.hh"

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

namespace cpop {

class PhysicsList;

class PhysicsListMessenger : public MessengerBase
{
public:
    PhysicsListMessenger(PhysicsList* physics_list);
    ~PhysicsListMessenger() = default;

    virtual void BuildCommands(G4String base) override;
    virtual void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    PhysicsList* physics_list_;

    /// \brief Select a eletromagnetic physics list
    std::unique_ptr<G4UIcmdWithAString> select_physics_;

    std::unique_ptr<G4UIcmdWithADoubleAndUnit> step_max_cmd_;
};

}



#endif // PHYSICSLISTMESSENGER_HH
