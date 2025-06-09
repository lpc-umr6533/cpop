#include "DetectorConstructionMessenger.hh"

#include "DetectorConstruction.hh"

namespace cpop {

DetectorConstructionMessenger::DetectorConstructionMessenger(DetectorConstruction *det):
	_detector(det)
{
	_dir = new G4UIdirectory("/detector/");

	_sizeCmd = new G4UIcmdWithADoubleAndUnit("/detector/size", this);
	_sizeCmd->SetGuidance("Set volume size");
	_sizeCmd->SetParameterName("WorldSize", false);
	_sizeCmd->SetRange("WorldSize>0");
	_sizeCmd->AvailableForStates(G4State_PreInit);
}

DetectorConstructionMessenger::~DetectorConstructionMessenger() {
	delete _sizeCmd;
}

void DetectorConstructionMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if(command == _sizeCmd)
		_detector->setWorldSize(_sizeCmd->GetNewDoubleValue(newValue));
}

}
