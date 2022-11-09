/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ElementManager.hh"
#include "InformationSystemManager.hh"

#include <assert.h>
#include "CLHEP/Units/SystemOfUnits.h"


#ifdef WITH_GEANT_4
	#include "G4NistManager.hh"
#endif

static ElementManager* elementManager = 0;

using namespace CLHEP;

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
ElementManager::ElementManager()
{
#ifdef WITH_GEANT_4

#else	
	generateStandardElements();

#endif // WITH WITH_GEANT_4
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
ElementManager::~ElementManager()
{

}

/////////////////////////////////////////////////////////////////////
/// \return The instance of the element manager.
/////////////////////////////////////////////////////////////////////
ElementManager* ElementManager::getInstance()
{
	if(!elementManager)
	{
		elementManager = new ElementManager();
	}
	return elementManager;
}

/////////////////////////////////////////////////////////////////////
/// \param pName The ID of the requested G4Element
/// \return The requested G4Element. NULL if doesn't defined yet.
/////////////////////////////////////////////////////////////////////
G4Element* ElementManager::getElement(QString pName)
{

#ifdef WITH_GEANT_4
	// call directly the NIST MANAGER
	G4NistManager* manager = G4NistManager::Instance();
	assert(manager);
	G4Element* elmt = manager->FindOrBuildElement(G4String(pName.toStdString().c_str()));
	assert(elmt);
	return elmt;
#else 

	if(elementsMap.find(pName) == elementsMap.end())
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't furnish the element, not registred yet", "Element manager");
   		return NULL;
	} 
	// else
	return elementsMap[pName];

#endif // WITH WITH_GEANT_4

}

/////////////////////////////////////////////////////////////////////////////
/// \return True if success
/// \param pElement The element to register
/// \warning the G4Material name must be unique, if already exists
/// the add will be refused.
/////////////////////////////////////////////////////////////////////////////
bool ElementManager::registerElement(G4Element* pElement)
{
	assert(pElement);
	QString elementName = pElement->GetName().data();
	if(elementsMap.find(elementName) != elementsMap.end())
	{
		QString mes = "unable to add the element. One with the same ID ( name ) already exists.";
		mes += "Please check this isn't the same.";
		mes += "if not, change your element name or remove the other element.";
   		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Element manager");
		return false;
	}
	elementsMap.insert(std::pair<QString, G4Element*>(elementName, pElement));
	return true;
}

/////////////////////////////////////////////////////////////////////////////
/// \param pElementID The element ID (name) to unregister
/////////////////////////////////////////////////////////////////////////////
void ElementManager::unregisterElement(QString pElementID)
{
	elementsMap.erase(pElementID);	// if doesn't exists, manage by the std::map.
}

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
void ElementManager::generateStandardElements()
{
	G4Element* N = new G4Element("Nitrogen", "N", 7.,  14.01*g/mole);
	registerElement(N);
	G4Element* O = new G4Element("Oxygen"  , "O", 8., 16.00*g/mole);
	registerElement(O);
  	G4Element* H  =  new G4Element("Hydrogen","H" , 1., 1.01*g/mole);
  	registerElement(H);
}