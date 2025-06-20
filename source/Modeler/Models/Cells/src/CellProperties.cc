#include "CellProperties.hh"
#include "XMLSettings.hh"
#include "IDManager.hh"
#include "InformationSystemManager.hh"

#include <cassert>
#include <utility>

using namespace XML::CPOP_Flag;

///	\param pCellType 			The type of cellule
/// \param pNucleusPosType 		The type of position of cell's nucleus
/// \param pMasses 				The masses min/max values for each life state of the cell
/// \param pNucleusRadius		The radii of nuclei
/// \param pCytoplasmMaterials 	The material included on the membrane.
/// \param pNucleiMaterials 	The material included on the nucleus.
/// \param pDeformable 			True if the cell membrane deformable
CellProperties::CellProperties(
	CellType pCellType,
	eNucleusPosType pNucleusPosType,
	std::map<LifeCycle, CellVariableAttribute<double>> pMasses,
	std::map<LifeCycle, CellVariableAttribute<double>> pNucleusRadius,
	std::map<LifeCycle, G4Material*> pCytoplasmMaterials,
	std::map<LifeCycle, G4Material*> pNucleiMaterials,
	bool pDeformable
):
	_cellType(pCellType),
	_nucleusPosType(pNucleusPosType),
	_masses(std::move(pMasses)),
	_nucleusRadius(std::move(pNucleusRadius)),
	_cytoplasmMaterials(std::move(pCytoplasmMaterials)),
	_nucleiMaterials(std::move(pNucleiMaterials)),
	_deformable(pDeformable)
{
	_id = IDManager::getInstance()->getSpecificIDFor(CellProperties_IDMapName);
}

void CellProperties::print() const {
	std::cout << std::endl;
	std::cout << propertiesInfos().toStdString() << std::endl;
	std::cout << "   masses " << std::endl;

	for(auto const& masse : _masses) {
		std::cout << "      - " << getName(masse.first).toStdString() << std::endl;
		std::cout << masse.second.var_min() << "  -> " << masse.second.var_max() << std::endl;
	}
	std::cout << "   nucleus radius : " << std::endl;
	for(auto const& nucleusRadiu : _nucleusRadius) {
		std::cout << "      - " << getName(nucleusRadiu.first).toStdString() << std::endl;
		std::cout << nucleusRadiu.second.var_min() << "  -> " << nucleusRadiu.second.var_max() << std::endl;
	}
}

/// \param pLifeCycle The life state we want the masses for
CellVariableAttribute<double> CellProperties::getMasses(LifeCycle pLifeCycle) const {
	if(_masses.find(pLifeCycle) == _masses.end()) {
		std::string mes = "unknow masses for requested LifeCycle" + std::to_string(pLifeCycle);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes, "Cell properties");
		return {0., 0.};
	}

	return _masses.find(pLifeCycle)->second;
}

/// \param pLifeCycle The life state we want the nucleusRadius for
bool CellProperties::hasNucleusRadius(LifeCycle pLifeCycle) const {
	return _nucleusRadius.find(pLifeCycle) != _nucleusRadius.end();
}

/// \param pLifeCycle The life state we want the nucleusRadius for
CellVariableAttribute<double> CellProperties::getNucleusRadius(LifeCycle pLifeCycle) const {
	if(!hasNucleusRadius(pLifeCycle)) {
		std::string mes = "unknow nucleus radius for requested LifeCycle " + std::to_string(pLifeCycle);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes, "Cell properties");
		return {0., 0.};
	}

	return _nucleusRadius.find(pLifeCycle)->second;
}

/// \param pLifeCycle The life state we want the nucleusRadius for
G4Material* CellProperties::getCytoplasmMaterial(LifeCycle pLifeCycle) const {
	// if no material
	if(_cytoplasmMaterials.find(pLifeCycle) == _cytoplasmMaterials.end()) {
		return nullptr;
	} else {
		return _cytoplasmMaterials.find(pLifeCycle)->second;
	}
}

/// \param pLifeCycle The life state we want the nucleusRadius for
/// \return the nucleus material for the given life cycle
G4Material* CellProperties::getNucleusMaterial(LifeCycle pLifeCycle) const {
	// if no material
	if(_nucleiMaterials.find(pLifeCycle) == _nucleiMaterials.end()) {
		return nullptr;
	} else {
		return _nucleiMaterials.find(pLifeCycle)->second;
	}
}

/// \param writer where to redirect writing of information
void CellProperties::write(QXmlStreamWriter& writer) const {
	writer.writeStartElement(cell_property_flag);
	/// write cell attributes
	writeProperties(writer);

	writer.writeEndElement();
}

void CellProperties::writeProperties(QXmlStreamWriter& writer) const {
	// write cell properties ID
	writer.writeAttribute(cell_properties_ID_flag, QString::number(getID()));
	// write cell types
	writer.writeAttribute(cell_shape_type_flag, getCellTypeName(_cellType));
	// write nucleus pos type
	writer.writeAttribute(nucleus_pos_type_flag, QString::number(_nucleusPosType));

	// write active life cycle
	for(auto const& itLC : availableLifeCycle)
		writer.writeTextElement(life_cycle_flag, QString::number(itLC));

	// write masses
	{
		writer.writeStartElement(mass_flag);
		for(auto const& _masse : _masses) {
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, QString::number(_masse.first));
			writer.writeAttribute(min_flag, QString::number(_masse.second.var_min()) );
			writer.writeAttribute(max_flag, QString::number(_masse.second.var_max()) );
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // mass_flag
	}

	// write nucleus prop
	{
		writer.writeStartElement(nucleus_radius_flag);
		for(auto const& nucleusRadiu : _nucleusRadius) {
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, QString::number(nucleusRadiu.first));
			writer.writeAttribute(min_flag, QString::number(nucleusRadiu.second.var_min()));
			writer.writeAttribute(max_flag, QString::number(nucleusRadiu.second.var_max()));
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // nucleus_radius_flag
	}

	// write cytplasm materials
	{
		writer.writeStartElement(cytoplasms_mat_flag);
		std::map<LifeCycle, G4Material* >::const_iterator itCytoMaterial;
		for(auto const& cytoplasmMaterial : _cytoplasmMaterials) {
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, QString::number(cytoplasmMaterial.first));
			writer.writeAttribute(material_flag, QString(cytoplasmMaterial.second->GetName()));
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // cytoplasms_mat_flag
	}

	// write nucleus materials
	{
		writer.writeStartElement(nuclei_mat_flag);
		for(auto const& nucleiMaterial : _nucleiMaterials) {
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, QString::number(nucleiMaterial.first));
			writer.writeAttribute(material_flag, QString(nucleiMaterial.second->GetName()));
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // nuclei_mat_flag
	}
}

/// \param pMasses The default masses values
/// \param pNucleusRadius The default nuclei radius values
/// \param pNucleusPosType The default nuclei position type
/// \param pCytoplasmMaterial The default cytoplsam material
/// \param pNucleiMaterial The default nuclei material
void CellProperties::automaticFill(t_CellVarAtt_d pMasses, t_CellVarAtt_d pNucleusRadius,
	eNucleusPosType pNucleusPosType, G4Material* pCytoplasmMaterial, G4Material* pNucleiMaterial)
{
	std::map<LifeCycle, t_CellVarAtt_d > newMasses;
	std::map<LifeCycle, t_CellVarAtt_d > newNucleusRadii;
	std::map<LifeCycle, G4Material*> newCytoplasmMaterials; ///< \brief the material present on cells membrane
	std::map<LifeCycle, G4Material*> newNucleiMaterials;    ///< \brief the material present on nuclei

	for(int iState = 0; iState != LifeCycles::NA; iState++) {
		auto lLifeState = static_cast<LifeCycle>(iState);

		// set mass
		newMasses.insert(std::make_pair(lLifeState, pMasses));
		// set nucleus radius
		newNucleusRadii.insert(std::make_pair(lLifeState, pNucleusRadius));
		// set cytoplsam materials
		newCytoplasmMaterials.insert(std::make_pair(lLifeState, pCytoplasmMaterial));
		// set nuclei materials
		newNucleiMaterials.insert(std::make_pair(lLifeState, pNucleiMaterial));
	}

	setNucleusPosType(pNucleusPosType);
	setMasses(newMasses);
	setNucleusRadius(newNucleusRadii);

	_cytoplasmMaterials = newCytoplasmMaterials;
	_nucleiMaterials = newNucleiMaterials;
}

double CellProperties::getMinRadius() const {
	double minRadius = std::numeric_limits<double>::max();

	for(auto const& nucleusRadius : _nucleusRadius) {
		if(nucleusRadius.second.var_min() < minRadius)
			minRadius = nucleusRadius.second.var_min();
	}

	return minRadius;
}

double CellProperties::getMaxRadius() const {
	double maxRadius = -1;

	for(auto const& nucleusRadius : _nucleusRadius) {
		if(nucleusRadius.second.var_max() > maxRadius)
			maxRadius = nucleusRadius.second.var_max();
	}

	return maxRadius;
}

/// \param pRad the new value to set
void CellProperties::setMinNucleusRadius(double pRad) {
	for(auto& nucleusRadius : _nucleusRadius)
		nucleusRadius.second.setMin(pRad);
}

/// \param pRad the new value to set
void CellProperties::setMaxNucleusRadius(double pRad) {
	for(auto& nucleusRadius : _nucleusRadius)
		nucleusRadius.second.setMax(pRad);
}

/// \param pMat the material to set for cytoplasm for all defined life cycles
void CellProperties::setCytoplasmMaterial(G4Material* pMat) {
	assert(pMat);

	for(auto& cytoplasmMaterial : _cytoplasmMaterials)
		cytoplasmMaterial.second = pMat;
}

/// \param pMat the material to set for nucleus for all defined life cycles
void CellProperties::setNucleusMaterial(G4Material* pMat) {
	assert(pMat);

	for(auto& nucleiMaterial : _nucleiMaterials)
		nucleiMaterial.second = pMat;
}
