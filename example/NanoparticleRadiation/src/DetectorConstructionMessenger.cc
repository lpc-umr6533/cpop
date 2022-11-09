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
      m_detector(det)
{
    m_dir = new G4UIdirectory("/detector/");

    m_sizeCmd = new G4UIcmdWithADoubleAndUnit("/detector/size", this);
    m_sizeCmd->SetGuidance("Set volume size");
    m_sizeCmd->SetParameterName("WorldSize", false);
    m_sizeCmd->SetRange("WorldSize>0");
    m_sizeCmd->AvailableForStates(G4State_PreInit);
}

DetectorConstructionMessenger::~DetectorConstructionMessenger()
{
    delete m_sizeCmd;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if( command == m_sizeCmd ) {
        m_detector->setWorldSize(m_sizeCmd->GetNewDoubleValue(newValue));
    }
}
