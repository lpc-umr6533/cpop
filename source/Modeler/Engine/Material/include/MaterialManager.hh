#ifndef MATERIAL_MANAGER_HH
#define MATERIAL_MANAGER_HH

#include "G4Material.hh"

#include <QString>

/// \brief define the material manager
/// \warning materials are defined from the G4Material definition
/// @author Henri Payno
class MaterialManager {
public:
	MaterialManager();

	/// \brief return the instance of G4Material
	static MaterialManager* getInstance();
	/// \brief return the material defined as the default one
	G4Material* getDefaultMaterial();
	/// \brief return the material defined as the default one for membrane
	G4Material* getDefaultMaterialForCellCytoplasm();
	/// \brief return the material defined as the default one for nucleus
	G4Material* getDefaultMaterialForCellNucleus();
	/// \brief return a material from his name (id )
	G4Material* getMaterial(QString);
	/// \brief add a material to the manager.
	bool registerMaterial(G4Material*);
	/// \brief remove a material setted
	void unregisterMaterial(QString);

private:
	/// \brief for now crate basic materials. On the future will load materials from a file ( XML ? )
	void generateStandardMaterials();
	/// \brief define the Air G4material
	G4Material* defineAir();
	/// \brief define the Water material
	G4Material* defineWater();
	/// \brief define the water with differente density ???
	G4Material* defineWater_d1_2();

	std::map<QString, G4Material*> materialsMap;  ///< \brief The registration of the materials
};

#endif
