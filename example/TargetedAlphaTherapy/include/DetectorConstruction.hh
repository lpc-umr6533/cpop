/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"

#include "globals.hh"
#include "G4LogicalVolume.hh"
#include "G4UserLimits.hh"
#include "Population.hh"

namespace cpop {

class DetectorConstructionMessenger;
class Population;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction(const Population& population);
    ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    double getWorldSize() const;
    void setWorldSize(double value);

private:
    double m_worldSize;
    DetectorConstructionMessenger* m_messenger;

    G4VPhysicalVolume* fSpheroid;
    G4LogicalVolume* fLogicalSpheroid;
    G4Region*          fpRegion;
    G4VPhysicalVolume* fWorld;

    const Population* population_;

};

}

#endif // DETECTORCONSTRUCTION_HH
