#ifndef TARGETEDALPHATHERAPY_DETECTORCONSTRUCTION_HH
#define TARGETEDALPHATHERAPY_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

#include "G4LogicalVolume.hh"
#include "Population.hh"

namespace cpop {

class DetectorConstructionMessenger;
class Population;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction(const Population& population);
	~DetectorConstruction() override;

	G4VPhysicalVolume* Construct() override;

	[[nodiscard]] double getWorldSize() const;
	void setWorldSize(double value);

private:
	double _worldSize;
	DetectorConstructionMessenger* _messenger;

	G4VPhysicalVolume*  fSpheroid;
	G4LogicalVolume*    fLogicalSpheroid;
	G4Region*           fpRegion;
	G4VPhysicalVolume*  fWorld;

	const Population* _population;
};

}

#endif
