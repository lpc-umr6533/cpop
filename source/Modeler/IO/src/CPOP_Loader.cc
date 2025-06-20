#include "CPOP_Loader.hh"
#include "RoundCellProperties.hh"
#include "MaterialManager.hh"
#include "SimpleDiscoidalCell.hh"
#include "SimpleSpheroidalCell.hh"
#include "SpheresSDelimitation.hh"

QString CLOSING_XML_NODE(QString nodeName) {
	return ("/" + nodeName);
}

/// \return map the map of each cellProperties with their IDs from the XML
/// \param elmt The xml node where cell properties are stored
/// \warning IDs from the map and CellPropertie ID can changed because ID Cellproperties is manage from the IDManager.
/// \return the map of cellProperties and their Ids
std::map<unsigned long int, CellProperties* > CPOP_Loader::getCellPropertiesMap(QXmlStreamReader* xmlReader) {
	assert(xmlReader);
	std::map<unsigned long int, CellProperties* > lCellPropertiesMap;

	while(!xmlReader->atEnd() &&  !xmlReader->hasError()) {
		// find the flag defining all cell properties
		xmlReader->readNext();
		if(xmlReader->name() == all_cell_properties_flag) {
			while( !xmlReader->atEnd() &&  !xmlReader->hasError() && (xmlReader->name() != CLOSING_XML_NODE(all_cell_properties_flag))) {
				xmlReader->readNext();
				// until we have cell properties to parse : parse them
				if(xmlReader->name() == cell_property_flag) {
					unsigned long int lCellPropID;
					CellProperties* lCellProperty = getCellProperties(xmlReader, lCellPropID);
					lCellPropertiesMap.insert(std::make_pair( lCellPropID, lCellProperty));
				}
			}
		}
	}
	return lCellPropertiesMap;
}

/// \param node the node where is store the CellProperties
/// \return CellProperties* the requested CellProperties
CellProperties* CPOP_Loader::getCellProperties(QXmlStreamReader* xmlReader, unsigned long int & propertyID) {
	assert(xmlReader);
	assert(xmlReader->name() == cell_property_flag);
	CellProperties* result= nullptr;

	QXmlStreamAttributes attributes = xmlReader->attributes();
	assert(attributes.hasAttribute(cell_shape_type_flag));
	// set RoundCellProperties specificities
	if(attributes.value(cell_shape_type_flag) == getCellTypeName(SIMPLE_ROUND))
		result = new RoundCellProperties();

	// set the cell property Id
	assert(attributes.hasAttribute(cell_properties_ID_flag));
	propertyID = attributes.value(cell_properties_ID_flag).toString().toLong();

	assert(result);

	xmlReader->readNext();
	// until we are not at the end of the document or an error append and we are still in the "cellproperty node"
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != cell_property_flag)) {
		// set nucleus radius
		if(xmlReader->name() == nucleus_radius_flag)
			result->setNucleusRadius(getCellProp_d_Value(xmlReader, nucleus_radius_flag.toStdString()));

		// set masses
		if(xmlReader->name() == mass_flag)
			result->setMasses(getCellProp_d_Value(xmlReader, mass_flag.toStdString()));

		// set membrane material
		if(xmlReader->name() == cytoplasms_mat_flag )
			result->setCytoplasmMaterials(getCellProp_Mat_Value(xmlReader, cytoplasms_mat_flag.toStdString()));

		// set nuclei material
		if(xmlReader->name() == nuclei_mat_flag)
			result->setNucleusMaterials(getCellProp_Mat_Value(xmlReader, nuclei_mat_flag.toStdString()));

		if(xmlReader->name() == membrane_radius_flag) {
				assert(dynamic_cast<RoundCellProperties*>(result));
				dynamic_cast<RoundCellProperties*>(result)->setMembraneRadius(getCellProp_d_Value(xmlReader, membrane_radius_flag.toStdString()));
		}

		xmlReader->readNext();
	}

	return result;
}

/// \param node The node where is stored the cell property
/// \param nodeID The name of the node we are looking for
/// \return The map linking LifeCycle and the associated CellProperties
std::map<LifeCycles::LifeCycle, CellProperties::t_CellVarAtt_d > CPOP_Loader::getCellProp_d_Value(QXmlStreamReader* xmlReader, std::string const& closingNodeName) {
	assert(xmlReader);
	assert(xmlReader->name() == QString::fromStdString(closingNodeName));
	xmlReader->readNext();

	std::map<LifeCycles::LifeCycle, CellProperties::t_CellVarAtt_d> result;

	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != QString::fromStdString(closingNodeName))) {
		if(xmlReader->name() == variable_attribute_flag) {
			QXmlStreamAttributes attributes = xmlReader->attributes();
			assert(attributes.hasAttribute(life_cycle_flag));
			assert(attributes.hasAttribute(min_flag));
			assert(attributes.hasAttribute(max_flag));

			result.insert(
				std::make_pair(
					(LifeCycle) attributes.value(life_cycle_flag).toString().toInt(),
					CellProperties::t_CellVarAtt_d(
						attributes.value(min_flag).toString().toDouble(),
						attributes.value(max_flag).toString().toDouble() )
				)
			);

			// skip the closing node "/>"
			xmlReader->readNext();
		}

		xmlReader->readNext();
	}

	return result;
}

/// \param node The node where is stored the cell property
/// \param nodeID The name of the node we are looking for
/// \return The map linking LifeCycle and G4Material
std::map<LifeCycles::LifeCycle, G4Material* > CPOP_Loader::getCellProp_Mat_Value(QXmlStreamReader* xmlReader, std::string const& closingNodeName) {
	assert(xmlReader);
	assert(xmlReader->name() == QString::fromStdString(closingNodeName));
	xmlReader->readNext();

	std::map<LifeCycles::LifeCycle, G4Material*> result;

	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != QString::fromStdString(closingNodeName))) {
		if(xmlReader->name() == variable_attribute_flag) {
			QXmlStreamAttributes attributes = xmlReader->attributes();
			assert(attributes.hasAttribute(life_cycle_flag));
			assert(attributes.hasAttribute(material_flag));

			result.insert(
				std::make_pair(
					(LifeCycle) attributes.value(life_cycle_flag).toString().toInt(),
					MaterialManager::getInstance()->getMaterial( attributes.value(material_flag).toString())
				)
			);

			// skip the closing node "/>"
			xmlReader->readNext();
		}

		xmlReader->readNext();
	}

	return result;
}

// pathc used for now, deal only with round nuclei
struct nucleiInfo {
	double nucleusRadius;
	eNucleusPosType positionType;
	eNucleusType nucleusType;

	nucleiInfo(double pNucRad, eNucleusPosType pPosType, eNucleusType eNucType) {
		nucleusRadius = pNucRad;
		positionType = pPosType;
		nucleusType = eNucType;
	}
};

std::vector<nucleiInfo> getNucleiInfo(QXmlStreamReader* xmlReader) {
	assert(xmlReader->name() == nuclei_flag);
	xmlReader->readNext();
	std::vector<nucleiInfo> result;
	while(xmlReader->name() != nuclei_flag) {
		if(xmlReader->name().toString() == nucleus_flag) {
			QXmlStreamAttributes attributes = xmlReader->attributes();

			assert(attributes.hasAttribute(nucleus_shape_type_flag));
			assert(attributes.hasAttribute(nucleus_pos_type_flag));
			assert(attributes.hasAttribute(radius_flag));

			double lNucleusRadius = attributes.value(radius_flag).toString().toDouble() ;
			eNucleusPosType lPositionType = (eNucleusPosType) attributes.value(nucleus_pos_type_flag).toString().toInt();
			eNucleusType lNucleusType = (eNucleusType) attributes.value(nucleus_shape_type_flag).toString().toInt();

			result.emplace_back(lNucleusRadius, lPositionType, lNucleusType);

			// skip the closing node "/>"
			xmlReader->readNext();
		}
		xmlReader->readNext();
	}
	return result;
}

/// \brief generate a simple discoidal cell from a given node
/// \return The created DiscoidalCell
/// \param node The node where to find the agent
/// \param forceID if true, we will force the agent ID to the one referenced on the XML
/// \warning if agent already exists on the similation, forcing ID can create conflicting ID
/// must be used cautiously
DiscoidalCell* CPOP_Loader::createDiscoidalCell(QXmlStreamReader* xmlReader, bool forceID) {
	assert(xmlReader);

	QXmlStreamAttributes attributes = xmlReader->attributes();
	assert(attributes.hasAttribute(agent_ID_flag));

	unsigned long int lID = attributes.value(agent_ID_flag).toString().toLong();
	int lCellPropertyID = attributes.value(cell_properties_ID_flag).toString().toInt();
	//assert(attributes.hasAttribute(position_flag));
	QString lifeCycleID = attributes.value(life_cycle_flag).toString();
	LifeCycle lifeCycle = (lifeCycleID != QString()) ? (LifeCycle)lifeCycleID.toInt() : LifeCycle::NA;
	double mass = attributes.value(mass_flag).toString().toDouble();

	// 1. Read XML stream
	xmlReader->readNext();
	// while inside the node :
	Point_2 position;
	double cellRadius = 0.;
	std::vector<nucleiInfo> nucleiInfo;
	// get information
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != cell_flag)) {
		// attributes = xmlReader->attributes();
		if(xmlReader->name().toString() ==position_flag)
			getPointFrmXMLStream(xmlReader, &position);

		if(xmlReader->name().toString() == radius_flag) {
			// get in the node value
			xmlReader->readNext();
			cellRadius = xmlReader->text().toString().toDouble();
			// move out to the radius node
			xmlReader->readNext();
			assert(xmlReader->name().toString() == radius_flag);
		}

		if(xmlReader->name().toString() == nuclei_flag)
			nucleiInfo = getNucleiInfo(xmlReader);

		xmlReader->readNext();
	}

	// 2. treat reded informations
	// get CellProperties
	CellProperties* cellProperties = nullptr;
	if(_cellPropertiesMap.find(lCellPropertyID) == _cellPropertiesMap.end()) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::WARNING_MES, "Unable to set cell properties, not found", "FILE::XML::createSpheroidalCell");
	} else {
		cellProperties = _cellPropertiesMap.find(lCellPropertyID)->second;
	}

	// if is a simple Spheroidal cell
	if(nucleiInfo.size() == 1 && (nucleiInfo.front().nucleusType == 0)) { // todo : set round nucleus wil be better
		// get nucleus attribute
		double nucleusRadius = 				nucleiInfo.front().nucleusRadius;
		eNucleusPosType nucleusPosType = 	nucleiInfo.front().positionType;

		/// \warning : we give the nucleusPosType because can be random on cellProperties and otherwise could be changed from a save file to a load file
		auto* cell = new SimpleDiscoidalCell(cellProperties, position, cellRadius, nucleusRadius, nucleusPosType, mass);
		cell->setLifeCycle(lifeCycle);

		// force the cell ID
		if(forceID)
			cell->forceID(lID);

		return cell;
	}

	return nullptr;
}

/// \brief generate a simple spheroidal cell from a given node
/// \return the created SpheroidalCell
/// \param node The node where to find the agent
/// \param forceID if true, we will force the agent ID to the one referenced on the XML
/// \warning if agent already exists on the similation, forcing ID can create conflicting ID
/// must be used cautiously

SpheroidalCell* CPOP_Loader::createSpheroidalCell(QXmlStreamReader* xmlReader, bool forceID) {
	assert(xmlReader);

	QXmlStreamAttributes attributes = xmlReader->attributes();
	assert(attributes.hasAttribute(agent_ID_flag));

	unsigned long int lID = attributes.value(agent_ID_flag).toString().toLong();
	int lCellPropertyID = attributes.value(cell_properties_ID_flag).toString().toInt();
	//assert(attributes.hasAttribute(position_flag));
	QString lifeCycleID = attributes.value(life_cycle_flag).toString();
	LifeCycle lifeCycle = (lifeCycleID != QString()) ? (LifeCycle)lifeCycleID.toInt() : LifeCycle::NA;
	double mass = attributes.value(mass_flag).toString().toDouble();

	// 1. Read XML stream
	xmlReader->readNext();
	// while inside the node :
	Point_3 position;
	double cellRadius = -1.;
	std::vector<nucleiInfo> nucleiInfo;
	// get information
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != cell_flag)) {
		if(xmlReader->name().toString() == position_flag)
			getPointFrmXMLStream(xmlReader, &position);

		if((xmlReader->name().toString() == radius_flag) && (cellRadius < 0.)) {
			// get in the node value
			xmlReader->readNext();
			cellRadius = xmlReader->text().toString().toDouble();
			// // move out to the radius node
			// xmlReader->readNext();
			// assert(xmlReader->name().toString() == radius_flag);
		}

		if(xmlReader->name().toString() == nuclei_flag)
				nucleiInfo = getNucleiInfo(xmlReader);

		xmlReader->readNext();
	}

	// 2. treat reded informations
	// get CellProperties
	CellProperties* cellProperties = nullptr;
	if(_cellPropertiesMap.find(lCellPropertyID) == _cellPropertiesMap.end()) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::WARNING_MES, "Unable to set cell properties, not found", "FILE::XML::createSpheroidalCell");
	} else {
		cellProperties = _cellPropertiesMap.find(lCellPropertyID)->second;
	}

	// if is a simple Spheroidal cell
	if(nucleiInfo.size() == 1 && (nucleiInfo.front().nucleusType == 0)) { // todo : set round nucleus wil be better
		// get nucleus attribute
		double nucleusRadius = nucleiInfo.front().nucleusRadius;
		eNucleusPosType nucleusPosType = nucleiInfo.front().positionType;

		/// \warning : we give the nucleusPosType because can be random on cellProperties and otherwise could be changed from a save file to a load file
		auto* cell = new SimpleSpheroidalCell(cellProperties, position, cellRadius, nucleusRadius, nucleusPosType, mass);
		//SpheroidalCell* cell = new SpheroidalCell(cellProperties, position, cellRadius, nucleusRadius, nucleusPosType, mass);
		cell->setLifeCycle(lifeCycle);

		// force the cell ID
		if(forceID)
			cell->forceID(lID);

		return cell;
	}

	return nullptr;
}

/// \param node The node where to find the agent
/// \return the agent created from the given node
/// \param forceID if true, we will force the agent ID to the one referenced on the XML
/// \warning if agent already exists on the similation, forcing ID can create conflicting ID
/// must be used cautiously
Agent* CPOP_Loader::getAgentFromNode( QXmlStreamReader* xmlReader, bool forceID) {
    assert(xmlReader);

    QXmlStreamAttributes attributes = xmlReader->attributes();
    assert(attributes.hasAttribute(dimension_flag));
    assert(attributes.hasAttribute(mass_flag));

    Dimension lDim = (Dimension)	attributes.value(dimension_flag).toString().toInt();
    CellType lCellType = (CellType)	attributes.value(cell_shape_type_flag).toString().toInt();

		// TODO check
    switch(lDim) {
    case _2D:
    {
			switch(lCellType) {
			case ROUND:

				return createDiscoidalCell(xmlReader, forceID);

			default:
				return nullptr;
			}
    }
    case _3D:
    {
    case ROUND :
				return createSpheroidalCell(xmlReader, forceID);
			default:
				return nullptr;
		}
    }

    return nullptr;
}

/// \param path 	The path to the requested file
/// \param forceID 	if true, we will force the agent ID to the one referenced on the XML
/// \return 		The map of agent founded with the agent ID as key
/// \warning if agent already exists on the similation, forcing ID can create conflicting ID
/// must be used cautiously
std::map<unsigned long int, Agent*> CPOP_Loader::getAgents(QXmlStreamReader* xmlReader, bool forceID) {
	assert(xmlReader);

	std::map<unsigned long int, Agent*> agents;

	// skip information until found the cells_flag
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != cells_flag))
			xmlReader->readNext();

	// parse all cells on the node
	xmlReader->readNext();
	while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != cells_flag)) {
		if(xmlReader->name() == cell_flag) {
			QXmlStreamAttributes attributes = xmlReader->attributes();
			assert(attributes.hasAttribute(agent_ID_flag));

			unsigned long int lID = attributes.value(agent_ID_flag).toString().toLong();
			// G4cout << "lID xmlReader : " << lID << G4endl;
			Agent* lAgt = getAgentFromNode(xmlReader, forceID);
			assert(lAgt);
			if(agents.find(lID) != agents.end()) {
				std::string mess = "Error - found two agent with the same ID : " + std::to_string(lAgt->getID());
				InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "FILE::XML::getAgents");
			} else {
				agents.insert(std::make_pair(lID, lAgt));
			}
		}
		xmlReader->readNext();
	}

	return agents;
}

/// \cond
void CPOP_Loader::getPointFrmXMLStream(QXmlStreamReader* xmlReader, Point_2* pointToSet) {
	assert(xmlReader);
	assert(xmlReader->name() == position_flag);

	// get x
	xmlReader->readNext();
	xmlReader->readNext();
	assert(xmlReader->name().toString() == "x");
	xmlReader->readNext();  // get in node value
	double x = xmlReader->text().toString().toDouble();
	xmlReader->readNext();	// move out of the x value
	xmlReader->readNext();

	// get y
	xmlReader->readNext();
	assert(xmlReader->name().toString() == "y");
	xmlReader->readNext();  // get in node value
	double y = xmlReader->text().toString().toDouble();
	xmlReader->readNext();	// move out of the x value
	xmlReader->readNext();

	// create the point
	*pointToSet = Point_2( x, y );

	// move to the closing "position" flag
	xmlReader->readNext();
	assert(xmlReader->name() == position_flag);
}

void CPOP_Loader::getPointFrmXMLStream(QXmlStreamReader* xmlReader, Point_3* pointToSet) {
	assert(xmlReader);
	assert(xmlReader->name() == position_flag);

	// get x
	xmlReader->readNext();
	xmlReader->readNext();
	assert(xmlReader->name().toString() == "x");
	xmlReader->readNext();  // get in node value
	double x = xmlReader->text().toString().toDouble();
	xmlReader->readNext();	// move out of the x value
	xmlReader->readNext();

	// get y
	xmlReader->readNext();
	assert(xmlReader->name().toString() == "y");
	xmlReader->readNext();  // get in node value
	double y = xmlReader->text().toString().toDouble();
	xmlReader->readNext();	// move out of the x value
	xmlReader->readNext();

	// get y
	xmlReader->readNext();
	assert(xmlReader->name().toString() == "z");
	xmlReader->readNext();  // get in node value
	double z = xmlReader->text().toString().toDouble();
	xmlReader->readNext();	// move out of the x value
	xmlReader->readNext();

	// create the point
	*pointToSet = Point_3( x, y, z );

	// move to the closing "position" flag
	xmlReader->readNext();
	assert(xmlReader->name() == position_flag);
}

t_SpatialDelimitation_2* CPOP_Loader::getDelimitationFromDomNode(QXmlStreamReader* xmlReader, t_SpatialDelimitation_2* delimitation) {
	while(xmlReader->name().toString() != discsSDelimitation_flag)
		xmlReader->readNext();

	assert(xmlReader->name().toString() ==  discsSDelimitation_flag);

	// check discs
	if(xmlReader->name().toString() ==  discsSDelimitation_flag) {
		xmlReader->readNext();
		Point_2 origin;
		double internalRadius = 0.;
		double externalRadius = 0.;
		while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&	(xmlReader->name() != discsSDelimitation_flag)) {
			if(xmlReader->name().toString() == internalDelimitation_flag) {
				assert(xmlReader->attributes().hasAttribute(radius_flag));
				internalRadius = xmlReader->attributes().value(radius_flag).toString().toDouble();
			}

			if(xmlReader->name().toString() == externalDelimitation_flag) {
				assert(xmlReader->attributes().hasAttribute(radius_flag));
				externalRadius = xmlReader->attributes().value(radius_flag).toString().toDouble();
			}

			if(xmlReader->name().toString() == position_flag)
				getPointFrmXMLStream(xmlReader, &origin);
		}

		delimitation = new t_DiscsSDelimitation_2( internalRadius, externalRadius, origin );

		assert(delimitation);
	}

	return delimitation;
}

t_SpatialDelimitation_3* CPOP_Loader::getDelimitationFromDomNode(QXmlStreamReader* xmlReader, t_SpatialDelimitation_3* delimitation) {
	while(
		( xmlReader->name().toString() != discsSDelimitation_flag  ) &&
		( xmlReader->name().toString() != spheresSDelimitation_flag)
	) {
		xmlReader->readNext();
	}

	// check discs
	if(( xmlReader->name().toString() == discsSDelimitation_flag) || (xmlReader->name().toString() == spheresSDelimitation_flag)) {
		bool isASphere = (xmlReader->name().toString() == spheresSDelimitation_flag);
		xmlReader->readNext();
		Point_3 origin;
		double internalRadius = -1.;
		double externalRadius = -1.;
		bool foundPosition = false;

		while(!xmlReader->atEnd() &&  !xmlReader->hasError() &&
					(xmlReader->name() != discsSDelimitation_flag) && (xmlReader->name() != spheresSDelimitation_flag)
		) {
			if((internalRadius < 0.) && (xmlReader->name().toString() == internalDelimitation_flag)) {
				assert(xmlReader->attributes().hasAttribute(radius_flag));
				internalRadius = xmlReader->attributes().value(radius_flag).toString().toDouble();
				// skip the closing node "/>"
				// xmlReader->readNext();
			}

			if((externalRadius < 0.) && (xmlReader->name().toString() == externalDelimitation_flag)) {
				assert(xmlReader->attributes().hasAttribute(radius_flag));
				externalRadius = xmlReader->attributes().value(radius_flag).toString().toDouble();
				// skip the closing node "/>"
				// xmlReader->readNext();
			}

			if((xmlReader->name().toString() == position_flag) && (!foundPosition)) {
				foundPosition = true;
				getPointFrmXMLStream(xmlReader, &origin);
				// xmlReader->readNext();
			}

			xmlReader->readNext();
		}

		if(isASphere) {
			delimitation = new SpheresSDelimitation( internalRadius, externalRadius, origin );
		} else {
			delimitation = new t_DiscsSDelimitation_3( internalRadius, externalRadius, origin );
		}

		assert(delimitation);
		xmlReader->readNext();
	}

	if(xmlReader->atEnd()) {
		assert(false);
		exit(0);
	}

	if(xmlReader->hasError()) {
		assert(false);
		exit(0);
	}

	return delimitation;
}

/// \endcond

bool CPOP_Loader::foundEnvironmentName(QXmlStreamReader* xmlReader, std::string& theName) {
	while(!xmlReader->atEnd() &&  !xmlReader->hasError()) {
		xmlReader->readNext();
		QXmlStreamAttributes attributes = xmlReader->attributes();
		if(attributes.hasAttribute(name_flag)) {
			theName = attributes.value(name_flag).toString().toStdString();
			return true;
		}
	}

	return false;
}
