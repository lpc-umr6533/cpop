#include "StepMax.hh"

#include "StepMaxMessenger.hh"

namespace cpop {

StepMax::StepMax(const G4String &processName):
	G4VDiscreteProcess(processName),
	_maxChargedStep(DBL_MAX)
	//messenger_(std::make_unique<StepMaxMessenger>(this))
{
}

G4bool StepMax::IsApplicable(const G4ParticleDefinition & particle) {
	return (particle.GetPDGCharge() != 0. && !particle.IsShortLived());
}

void StepMax::SetMaxStep(G4double step) {
	_maxChargedStep = step;
}

G4double StepMax::PostStepGetPhysicalInteractionLength(const G4Track &aTrack, G4double, G4ForceCondition *condition) {
	// condition is set to "Not Forced"
	*condition = NotForced;

	G4double ProposedStep = DBL_MAX;

	if((_maxChargedStep > 0.) &&
		 (aTrack.GetVolume() != nullptr)
		 && (aTrack.GetVolume()->GetName() != "World")
	)
		ProposedStep = _maxChargedStep;

	return ProposedStep;
}

G4VParticleChange *StepMax::PostStepDoIt(const G4Track & aTrack, const G4Step &) {
	// do nothing
	aParticleChange.Initialize(aTrack);
	return &aParticleChange;
}

}
