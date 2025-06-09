#ifndef DETECTORCONSTRUCTIONMESSENGER_HH
#define DETECTORCONSTRUCTIONMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

class DetectorConstruction;

class DetectorConstructionMessenger : public G4UImessenger
{
public:
	DetectorConstructionMessenger(DetectorConstruction* det);
	~DetectorConstructionMessenger() override;

	void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
	DetectorConstruction* _detector;

	G4UIdirectory* _dir = nullptr;
	G4UIcmdWithADoubleAndUnit* _sizeCmd = nullptr;
};

#endif
