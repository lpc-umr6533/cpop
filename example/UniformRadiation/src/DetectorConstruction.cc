#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include <stdexcept>

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

DetectorConstruction::DetectorConstruction():
	_worldSize(-1*CLHEP::micrometer)
{
	_messenger = new DetectorConstructionMessenger(this);
}

DetectorConstruction::~DetectorConstruction() {
	delete _messenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct() {
	if(this->getWorldSize() == -1*CLHEP::micrometer)
		std::runtime_error("World size not set. Please use /detector/size in your macro file.");

	G4Material* lWater = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER", false, false);

	auto* solidWorld = new G4Box("sWorld", this->getWorldSize(), this->getWorldSize(), this->getWorldSize());

	auto* logicWorld= new G4LogicalVolume( solidWorld, lWater, "LV_World", nullptr, nullptr, nullptr);

	return new G4PVPlacement(
		G4Transform3D(),// no rotation
		logicWorld,     // its logical volume
		"PV_World",     // its name
		nullptr,        // its mother  volume
		false,          // no boolean operations
		0,              // copy number
		false           // surface overlaps
	);
}

double DetectorConstruction::getWorldSize() const {
	return _worldSize;
}

void DetectorConstruction::setWorldSize(double value) {
	_worldSize = value;
}
