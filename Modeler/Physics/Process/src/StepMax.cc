#include "StepMax.hh"

#include "StepMaxMessenger.hh"

namespace cpop {

StepMax::StepMax(const G4String &processName)
    :G4VDiscreteProcess(processName),
      max_charged_step_(DBL_MAX)
      //messenger_(std::make_unique<StepMaxMessenger>(this))
{

}

G4bool StepMax::IsApplicable(const G4ParticleDefinition & particle)
{
    return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

void StepMax::SetMaxStep(G4double step)
{
    max_charged_step_ = step;
}

G4double StepMax::PostStepGetPhysicalInteractionLength(const G4Track &aTrack, G4double, G4ForceCondition *condition)
{
    // condition is set to "Not Forced"
    *condition = NotForced;

    G4double ProposedStep = DBL_MAX;

    if((max_charged_step_ > 0.) &&
            (aTrack.GetVolume() != NULL)
            && (aTrack.GetVolume()->GetName() != "World")
            )
        ProposedStep = max_charged_step_;

    return ProposedStep;
}

G4VParticleChange *StepMax::PostStepDoIt(const G4Track & aTrack, const G4Step &)
{
    // do nothing
    aParticleChange.Initialize(aTrack);
    return &aParticleChange;
}

/*
StepMaxMessenger &StepMax::messenger()
{
    *messenger_;
}
* */

}


