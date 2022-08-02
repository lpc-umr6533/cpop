#ifndef STEPMAX_HH
#define STEPMAX_HH

#include "G4VDiscreteProcess.hh"

namespace cpop {

class StepMaxMessenger;

class StepMax : public G4VDiscreteProcess
{
public:
    StepMax(const G4String& processName = "UserMaxStep");

    virtual G4bool IsApplicable(const G4ParticleDefinition&particle);

    void SetMaxStep(G4double step);

    G4double GetMaxStep() {return max_charged_step_;}

    virtual
    G4double PostStepGetPhysicalInteractionLength( const G4Track& aTrack,
                                                   G4double previousStepSize,
                                                   G4ForceCondition* condition);

    virtual
    G4VParticleChange* PostStepDoIt(const G4Track&aTrack, const G4Step&);

    virtual
    G4double GetMeanFreePath(const G4Track&, G4double,G4ForceCondition*)
    {return DBL_MAX;}

    //StepMaxMessenger& messenger();

private:
    G4double max_charged_step_;
    //std::unique_ptr<StepMaxMessenger> messenger_;
};

}

#endif // STEPMAX_HH
