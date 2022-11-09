#ifndef STEPMAXMESSENGER_HH
#define STEPMAXMESSENGER_HH

#include "G4UIcmdWithADoubleAndUnit.hh"

#include "MessengerBase.hh"

namespace cpop {

class StepMax;

class StepMaxMessenger : public MessengerBase
{
public:
    StepMaxMessenger(StepMax* step_max);

    virtual void BuildCommands(G4String base) override;
    virtual void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    StepMax* step_max_;

    std::unique_ptr<G4UIcmdWithADoubleAndUnit> step_max_cmd_;

};

}

#endif // STEPMAXMESSENGER_HH
