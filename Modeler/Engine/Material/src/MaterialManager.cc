/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MaterialManager.hh"

#include "ElementManager.hh"
#include "InformationSystemManager.hh"
#include <assert.h>

#include "CLHEP/Units/SystemOfUnits.h"


#ifdef WITH_GEANT_4
	#include "G4NistManager.hh"
#endif


static MaterialManager* materialManager = 0;

static QString defaultMaterial = "G4_WATER";
static QString defaultMaterialNucleus = "G4_WATER";
static QString defaultMaterialMembrane = "G4_WATER";

using namespace CLHEP;

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
MaterialManager::MaterialManager()
{
	generateStandardMaterials();
}
/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
MaterialManager::~MaterialManager()
{

}

/////////////////////////////////////////////////////////////////////////////
/// \warning NISTManager will be available only if you use the WITH_GEANT_4 option
/////////////////////////////////////////////////////////////////////////////
void MaterialManager::generateStandardMaterials()
{
#ifdef WITH_GEANT_4

#else 
	registerMaterial(defineAir());
	registerMaterial(defineWater());
	registerMaterial(defineWater_d1_2());
	
#endif // WITH WITH_GEANT_4

}

/////////////////////////////////////////////////////////////////////////////
/// \return  The instance of the material manager.
/////////////////////////////////////////////////////////////////////////////
MaterialManager* MaterialManager::getInstance()
{
	if(!materialManager)
	{
		materialManager = new MaterialManager();
	}
	return materialManager;
}

/////////////////////////////////////////////////////////////////////////////
/// \param pMaterialID The ID of the material.
/// \return The requested G4Material, Null if doesn't exists.
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::getMaterial(QString pMaterialID)
{
#ifdef WITH_GEANT_4
	// call directly the NIST MANAGER
	G4NistManager* manager = G4NistManager::Instance();
	assert(manager);
	G4Material* mat = manager->FindOrBuildMaterial(G4String(pMaterialID.toStdString().c_str()));
	assert(mat);
	return mat;
#else 
	
	if(materialsMap.find(pMaterialID) == materialsMap.end())
	{
   		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't furnish the material, not registred", "Material manager");
   		return NULL;
	}
	// else
	return materialsMap[pMaterialID];

#endif // WITH WITH_GEANT_4
	

}

/////////////////////////////////////////////////////////////////////////////
/// \return true if registration succeded
/// \param pMaterial The material to register
/// \warning the G4Material name must be unique, if already exists
/// the add will be refused.
/////////////////////////////////////////////////////////////////////////////
bool MaterialManager::registerMaterial( G4Material* pMaterial)
{
	assert(pMaterial);
	QString materialName = pMaterial->GetName().data();
	if(materialsMap.find(materialName) != materialsMap.end())
	{
		QString mes = "unable to add the material. One with the same ID ( name ) already exists.";
		mes += "Please check this isn't the same.";
		mes += "if not, change your material name or remove the other material.";
   		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Material manager");
		return false;
	}
	materialsMap.insert(std::pair<QString, G4Material*>(materialName, pMaterial));
	return true;
}

/////////////////////////////////////////////////////////////////////////////
/// \return The default G4Material* used Null if not defined.
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::getDefaultMaterial()
{
	return getMaterial(defaultMaterial);
}

/////////////////////////////////////////////////////////////////////////////
/// \param pMaterialID The id of the material to unregister
/////////////////////////////////////////////////////////////////////////////
void MaterialManager::unregisterMaterial(QString pMaterialID)
{
	materialsMap.erase(pMaterialID);	// if doesn't exists, manage by the std::map.
}

///////////////////////////// Basic Material creation ///////////////////
// TODO : load a file with this material definitions.

/////////////////////////////////////////////////////////////////////////////
/// \return the Air material
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::defineAir()
{
	G4Element* N = ElementManager::getInstance()->getElement("Nitrogen");
	G4Element* O = ElementManager::getInstance()->getElement("Oxygen");
	G4double density = 1.29*mg/cm3;   
	G4int ncomponents = 2;
	G4Material* Air = new G4Material("Air", density, ncomponents);
	Air->AddElement(N, 0.7);
	Air->AddElement(O, 0.3);

	return Air;
}

/////////////////////////////////////////////////////////////////////////////
/// \return the Water material
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::defineWater()
{
  G4Element* H  = ElementManager::getInstance()->getElement("Hydrogen");
  G4Element* O  = ElementManager::getInstance()->getElement("Oxygen");
  G4double density = 1.000*g/cm3;
  G4int ncomponents = 2;

  G4Material* H2O = new G4Material("G4_WATER", density, ncomponents=2);
  H2O->AddElement(H, 2);
  H2O->AddElement(O, 1);
  //H2O->GetIonisation()->SetMeanEnergyExcitation(75.0*eV);

  return H2O;
}

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::defineWater_d1_2()
{
  G4Element* H  = ElementManager::getInstance()->getElement("Hydrogen");
  G4Element* O  = ElementManager::getInstance()->getElement("Oxygen");
  G4double density = 1.000*g/cm3;
  G4int ncomponents = 2;

  G4Material* H2O = new G4Material("Water_d1_2", density, ncomponents=2);
  H2O->AddElement(H, 2);
  H2O->AddElement(O, 1);

  return H2O;	
}

/////////////////////////////////////////////////////////////////////////////
/// \return the default material used for cytoplasm
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::getDefaultMaterialForCellCytoplasm()	
{
	return getMaterial(defaultMaterialMembrane);
}

/////////////////////////////////////////////////////////////////////////////
/// \return the default material used for nucleoplasm
/////////////////////////////////////////////////////////////////////////////
G4Material* MaterialManager::getDefaultMaterialForCellNucleus()
{
	return getMaterial(defaultMaterialNucleus);
}
