#ifndef STEPMAX_HH
#define STEPMAX_HH

#include "G4VDiscreteProcess.hh"

namespace cpop {

class StepMaxMessenger;

class StepMax : public G4VDiscreteProcess {
public:
	StepMax(const G4String& processName = "UserMaxStep");

	G4bool IsApplicable(const G4ParticleDefinition&particle) override;

	void SetMaxStep(G4double step);

	G4double GetMaxStep() { return _maxChargedStep; }

	G4double PostStepGetPhysicalInteractionLength(
		const G4Track& aTrack, G4double previousStepSize, G4ForceCondition* condition
	) override;

	G4VParticleChange* PostStepDoIt(const G4Track&aTrack, const G4Step&) override;

	G4double GetMeanFreePath(const G4Track&, G4double,G4ForceCondition*) override {
		return DBL_MAX;
	}

private:
	G4double _maxChargedStep;
	//std::unique_ptr<StepMaxMessenger> messenger_;
};

}

#endif
