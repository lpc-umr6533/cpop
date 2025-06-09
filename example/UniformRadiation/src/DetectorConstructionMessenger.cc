/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DetectorConstructionMessenger.hh"

#include "DetectorConstruction.hh"

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *det)
    :G4UImessenger(),
      _detector(det)
{
    _dir = new G4UIdirectory("/detector/");

    _sizeCmd = new G4UIcmdWithADoubleAndUnit("/detector/size", this);
    _sizeCmd->SetGuidance("Set volume size");
    _sizeCmd->SetParameterName("WorldSize", false);
    _sizeCmd->SetRange("WorldSize>0");
    _sizeCmd->AvailableForStates(G4State_PreInit);
}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete _sizeCmd;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if( command == _sizeCmd ) {
        _detector->setWorldSize(_sizeCmd->GetNewDoubleValue(newValue));
    }
}
