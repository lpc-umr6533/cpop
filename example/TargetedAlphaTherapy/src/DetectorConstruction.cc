/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DetectorConstruction.hh"
#include "DetectorConstructionMessenger.hh"

#include <stdexcept>

#include "UnitSystemManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4ProductionCuts.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4UserLimits.hh"

#include "G4Orb.hh"
#include "G4Ellipsoid.hh"

#include "G4Sphere.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"

namespace cpop {

DetectorConstruction::DetectorConstruction(const Population &population)
    :m_worldSize(-1*CLHEP::micrometer),
    population_(&population)
{
    m_messenger = new DetectorConstructionMessenger(this);
}

DetectorConstruction::~DetectorConstruction()
{
    delete m_messenger;
}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    if(this->getWorldSize() == -1*CLHEP::micrometer)
        std::runtime_error("World size not set. Please use /detector/size in your macro file.");

    G4Material* lWater = G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER", false, false);

    G4Box* solidWorld= new G4Box( "sWorld", this->getWorldSize(), this->getWorldSize(), this->getWorldSize() );

    G4LogicalVolume* logicWorld= new G4LogicalVolume( solidWorld, lWater, "LV_World", 0, 0, 0);

    fWorld = new G4PVPlacement(  G4Transform3D(),// no rotation
                               logicWorld,     // its logical volume
                               "PV_World",     // its name
                               0,              // its mother  volume
                               false,          // no boolean operations
                               0,              // copy number
                               false);         // surface overlaps

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

double DetectorConstruction::getWorldSize() const
{
    return m_worldSize;
}

void DetectorConstruction::setWorldSize(double value)
{
    m_worldSize = value;
}

}
