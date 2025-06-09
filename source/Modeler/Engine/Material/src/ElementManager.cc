#include "ElementManager.hh"
#include "InformationSystemManager.hh"

#include <cassert>
#include "CLHEP/Units/SystemOfUnits.h"

#ifdef WITH_GEANT_4
	#include "G4NistManager.hh"
#endif

static ElementManager* elementManager = nullptr;

ElementManager::ElementManager() {
#ifdef WITH_GEANT_4

#else
	generateStandardElements();

#endif // WITH WITH_GEANT_4
}

/// \return The instance of the element manager.
ElementManager* ElementManager::getInstance() {
	if(!elementManager)
		elementManager = new ElementManager();
	return elementManager;
}

/// \param pName The ID of the requested G4Element
/// \return The requested G4Element. NULL if doesn't defined yet.
G4Element* ElementManager::getElement(QString pName) {
#ifdef WITH_GEANT_4
	// call directly the NIST MANAGER
	G4NistManager* manager = G4NistManager::Instance();
	assert(manager);
	G4Element* elmt = manager->FindOrBuildElement(G4String(pName.toStdString().c_str()));
	assert(elmt);
	return elmt;
#else

	if(elementsMap.find(pName) == elementsMap.end()) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't furnish the element, not registred yet", "Element manager");
		return nullptr;
	}
	// else
	return elementsMap[pName];

#endif
}

/// \return True if success
/// \param pElement The element to register
/// \warning the G4Material name must be unique, if already exists
/// the add will be refused.
bool ElementManager::registerElement(G4Element* pElement) {
	assert(pElement);
	QString elementName = pElement->GetName().data();
	if(elementsMap.find(elementName) != elementsMap.end()) {
		QString mes = "unable to add the element. One with the same ID ( name ) already exists.";
		mes += "Please check this isn't the same.";
		mes += "if not, change your element name or remove the other element.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Element manager");
		return false;
	}

	elementsMap.insert(std::pair<QString, G4Element*>(elementName, pElement));
	return true;
}

/// \param pElementID The element ID (name) to unregister
void ElementManager::unregisterElement(QString pElementID) {
	elementsMap.erase(pElementID);
}

void ElementManager::generateStandardElements() {
	using namespace CLHEP;

	auto* N = new G4Element("Nitrogen", "N", 7., 14.01*g/mole);
	registerElement(N);
	auto* O = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
	registerElement(O);
	auto* H  =  new G4Element("Hydrogen", "H" , 1., 1.01*g/mole);
	registerElement(H);
}
