#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"

namespace cpop {

class EventAction : public G4UserEventAction
{
public:
    EventAction();
    ~EventAction() = default;

    virtual void BeginOfEventAction(const G4Event*evt);
    virtual void EndOfEventAction(const G4Event*);
};

}

#endif // EVENTACTION_HH
