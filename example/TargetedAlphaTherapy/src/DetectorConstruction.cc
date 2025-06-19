#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include <stdexcept>

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"

#include "G4VPhysicalVolume.hh"
#include "G4GeometryManager.hh"
#include "G4RunManager.hh"

namespace cpop {

DetectorConstruction::DetectorConstruction(const Population &population):
	_worldSize(-1*CLHEP::micrometer),
	_population(&population)
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

	auto* solidWorld = new G4Box( "sWorld", this->getWorldSize(), this->getWorldSize(), this->getWorldSize() );

	auto* logicWorld = new G4LogicalVolume( solidWorld, lWater, "LV_World", nullptr, nullptr, nullptr);

	fWorld = new G4PVPlacement(
		G4Transform3D(),// no rotation
		logicWorld,     // its logical volume
		"PV_World",     // its name
		nullptr,        // its mother  volume
		false,          // no boolean operations
		0,              // copy number
		true // surface overlaps
	);

	// const Population* population = population_;
	//
	// G4Sphere* sSpheroid = new G4Sphere("Spheroid", 0., population->spheroid_radius_, 0., twopi, 0., pi);
	//
	// // G4cout << "\n\n Spheroid Radius = " <<G4BestUnit(population->spheroid_radius_,"Length") << G4endl;
	//
	// fLogicalSpheroid = new G4LogicalVolume(sSpheroid, lWater, "Spheroid");
	//
	// fSpheroid = new G4PVPlacement(0, G4ThreeVector(0,0,0), "Spheroid", fLogicalSpheroid, fWorld, false, 0);
	//
	// //fLogicalSpheroid->SetVisAttributes(G4VisAttributes::Invisible);
	//
	// fpRegion = new G4Region("Spheroid");
	// fpRegion->AddRootLogicalVolume(fLogicalSpheroid);

	return fWorld;
}

double DetectorConstruction::getWorldSize() const {
	return _worldSize;
}

void DetectorConstruction::setWorldSize(double value) {
	_worldSize = value;
}

}
