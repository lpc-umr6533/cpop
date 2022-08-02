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

DetectorConstruction::DetectorConstruction()
    :m_worldSize(-1*CLHEP::micrometer)
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

    return new G4PVPlacement(  G4Transform3D(),// no rotation
                               logicWorld,     // its logical volume
                               "PV_World",     // its name
                               0,              // its mother  volume
                               false,          // no boolean operations
                               0,              // copy number
                               false);         // surface overlaps

}

double DetectorConstruction::getWorldSize() const
{
    return m_worldSize;
}

void DetectorConstruction::setWorldSize(double value)
{
    m_worldSize = value;
}
