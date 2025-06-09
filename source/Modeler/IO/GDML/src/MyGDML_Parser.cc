#include "MyGDML_Parser.hh"
#include "InformationSystemManager.hh"

bool MyGDML_Parser::write(QString path, G4PVPlacement* topWorld) {
	parser.Write(path.toStdString(), topWorld);

	InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, "write on file with sucess", "GDML_Parser");
	return true;
}
