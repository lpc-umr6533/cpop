/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MyGDML_Parser.hh"


#include "G4Material.hh"

#include "InformationSystemManager.hh"

MyGDML_Parser::MyGDML_Parser()
{
	

}

bool MyGDML_Parser::write(QString path, G4PVPlacement* topWorld)
{
	parser.Write(path.toStdString(), topWorld);
	
	InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, "write on file with sucess", "GDML_Parser");
	return true;

}

MyGDML_Parser::~MyGDML_Parser()
{

}

void MyGDML_Parser::close()
{

}