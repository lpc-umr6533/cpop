/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
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
    ~DetectorConstructionMessenger();

    void SetNewValue(G4UIcommand* command, G4String newValue);

private:

    DetectorConstruction* m_detector;

    G4UIdirectory* m_dir = nullptr;
    G4UIcmdWithADoubleAndUnit* m_sizeCmd = nullptr;

};

#endif // DETECTORCONSTRUCTIONMESSENGER_HH
