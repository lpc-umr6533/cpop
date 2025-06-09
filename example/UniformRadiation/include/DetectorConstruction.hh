#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

class DetectorConstructionMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	~DetectorConstruction() override;

	G4VPhysicalVolume* Construct() override;

	[[nodiscard]] double getWorldSize() const;
	void setWorldSize(double value);

private:
	double _worldSize;
	DetectorConstructionMessenger* _messenger;
};

#endif
