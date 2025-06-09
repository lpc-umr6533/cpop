#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"

namespace cpop {

class EventAction : public G4UserEventAction
{
public:
	void BeginOfEventAction(const G4Event*evt) override;
};

}

#endif
