/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "CellProperties.hh"
#include "XMLSettings.hh"

#include <assert.h>

using namespace XML::CPOP_Flag;
//////////////////////////////////////////////////////////////////////////////////////////
///	\param pCellType 			The type of cellule
/// \param pNucleusPosType 		The type of position of cell's nucleus
/// \param pMasses 				The masses min/max values for each life state of the cell
/// \param pNucleusRadius		The radii of nuclei
/// \param pCytoplasmMaterials 	The material included on the membrane.
/// \param pNucleiMaterials 	The material included on the nucleus.
/// \param pDeformable 			True if the cell membrane deformable
//////////////////////////////////////////////////////////////////////////////////////////
CellProperties::CellProperties(	CellType pCellType,
								eNucleusPosType pNucleusPosType,
	 							std::map<LifeCycle, CellVariableAttribute<double> > pMasses,
	 							std::map<LifeCycle, CellVariableAttribute<double> > pNucleusRadius,
	 							std::map<LifeCycle, G4Material* > pCytoplasmMaterials,
	 							std::map<LifeCycle, G4Material* > pNucleiMaterials,
	 							bool pDeformable
	 						) :
	cellType(pCellType),
	nucleusPosType(pNucleusPosType),
	masses(pMasses),
	nucleusRadius(pNucleusRadius),
	cytoplasmMaterials(pCytoplasmMaterials),
	nucleiMaterials(pNucleiMaterials),
	deformable(pDeformable)
{
	ID = IDManager::getInstance()->getSpecificIDFor(CellProperties_IDMapName);
}

//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
CellProperties::~CellProperties()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::print() const
{
	std::cout << std::endl;
	std::cout << propertiesInfos().toStdString() << std::endl;
	std::map<LifeCycle, CellVariableAttribute<double> >::const_iterator itAtt;
	std::cout << "   masses " << std::endl;
	for(itAtt = masses.begin(); itAtt != masses.end(); ++itAtt)
	{
		std::cout << "      - " << getName(itAtt->first).toStdString() << std::endl;
		std::cout << itAtt->second.var_min() << "  -> " << itAtt->second.var_max() << std::endl;
	}

	std::cout << "   nucleus radius : " << std::endl;
	for(itAtt = nucleusRadius.begin(); itAtt != nucleusRadius.end(); ++itAtt)
	{
		std::cout << "      - " << getName(itAtt->first).toStdString() << std::endl;
		std::cout << itAtt->second.var_min() << "  -> " << itAtt->second.var_max() << std::endl;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the masses for
//////////////////////////////////////////////////////////////////////////////////////////
CellVariableAttribute<double> CellProperties::getMasses(LifeCycle pLifeCycle) const
{
	if(masses.find(pLifeCycle) == masses.end())
	{
		QString mes = "unknow masses for requested LifeCycle" + QString::number(pLifeCycle);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Cell properties");
		return CellVariableAttribute<double>(0., 0.);
	}
	return masses.find(pLifeCycle)->second;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the nucleusRadius for
//////////////////////////////////////////////////////////////////////////////////////////
bool CellProperties::hasNucleusRadius(LifeCycle pLifeCycle) const
{
	return (nucleusRadius.find(pLifeCycle) != nucleusRadius.end());
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the nucleusRadius for
//////////////////////////////////////////////////////////////////////////////////////////
CellVariableAttribute<double> CellProperties::getNucleusRadius(LifeCycle pLifeCycle) const
{
	if(!hasNucleusRadius(pLifeCycle))
	{
		QString mes = "unknow nucleus radius for requested LifeCycle " + QString::number(pLifeCycle);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Cell properties");
		return CellVariableAttribute<double>(0., 0.);
	}
	return nucleusRadius.find(pLifeCycle)->second;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the nucleusRadius for
//////////////////////////////////////////////////////////////////////////////////////////
G4Material* CellProperties::getCytoplasmMaterial(LifeCycle pLifeCycle)	const
{
	// if no material
	if(cytoplasmMaterials.find(pLifeCycle) == cytoplasmMaterials.end())
	{
		return NULL;
	}else
	{
		return cytoplasmMaterials.find(pLifeCycle)->second;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the nucleusRadius for
/// \return the nucleus material for the given life cycle
//////////////////////////////////////////////////////////////////////////////////////////
G4Material* CellProperties::getNucleusMaterial(LifeCycle pLifeCycle)	const
{
	// if no material
	if(nucleiMaterials.find(pLifeCycle) == nucleiMaterials.end())
	{
		return NULL;
	}else
	{
		return nucleiMaterials.find(pLifeCycle)->second;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param writer where to redirect writing of information
//////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::write(QXmlStreamWriter& writer) const
{
	writer.writeStartElement(cell_property_flag);
	/// write cell attributes
	writeProperties(writer);

	writer.writeEndElement();

}


//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::writeProperties(QXmlStreamWriter& writer) const
{
	// write cell properties ID
	writer.writeAttribute(cell_properties_ID_flag, 	QString::number(getID()));
	// write cell types
	writer.writeAttribute(cell_shape_type_flag, 	getCellTypeName( cellType));
	// write nucleus pos type
	writer.writeAttribute(nucleus_pos_type_flag, 	QString::number(nucleusPosType) );

	// write active life cycle
	set<LifeCycle>::const_iterator itLC;
	for(itLC = availableLifeCycle.begin(); itLC != availableLifeCycle.end(); ++ itLC)
	{
		writer.writeTextElement(life_cycle_flag, 	QString::number(*itLC) );
	}

	// write masses
	{
		writer.writeStartElement(mass_flag);
		map<LifeCycle, t_CellVarAtt_d >::const_iterator itMass;
		for(itMass = masses.begin(); itMass != masses.end(); ++itMass)
		{
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, 	QString::number(itMass->first));
			writer.writeAttribute(min_flag, 		QString::number(itMass->second.var_min()) );
			writer.writeAttribute(max_flag, 		QString::number(itMass->second.var_max()) );
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // mass_flag
	}

	// write nucleus prop
	{
		writer.writeStartElement(nucleus_radius_flag);
		map<LifeCycle, t_CellVarAtt_d >::const_iterator itNucleus;
		for(itNucleus = nucleusRadius.begin(); itNucleus != nucleusRadius.end(); ++itNucleus)
		{
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, 	QString::number(itNucleus->first));
			writer.writeAttribute(min_flag, 		QString::number(itNucleus->second.var_min()) );
			writer.writeAttribute(max_flag, 		QString::number(itNucleus->second.var_max()) );
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // nucleus_radius_flag
	}

	// write cytplasm materials
	{
		writer.writeStartElement(cytoplasms_mat_flag);
		map<LifeCycle, G4Material* >::const_iterator itCytoMaterial;
		for(itCytoMaterial = cytoplasmMaterials.begin(); itCytoMaterial != cytoplasmMaterials.end(); ++itCytoMaterial)
		{
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, 	QString::number(itCytoMaterial->first));
			writer.writeAttribute(material_flag, 	QString(itCytoMaterial->second->GetName()) );
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // cytoplasms_mat_flag
	}

	// write nucleus materials
	{
		writer.writeStartElement(nuclei_mat_flag);
		map<LifeCycle, G4Material* >::const_iterator itNucleiMaterial;
		for(itNucleiMaterial = nucleiMaterials.begin(); itNucleiMaterial != nucleiMaterials.end(); ++itNucleiMaterial)
		{
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, 	QString::number(itNucleiMaterial->first));
			writer.writeAttribute(material_flag, 	QString(itNucleiMaterial->second->GetName()) );
			writer.writeEndElement(); // var_prop
		}
		writer.writeEndElement(); // nuclei_mat_flag
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pMasses The default masses values
/// \param pNucleusRadius The default nuclei radius values
/// \param pNucleusPosType The default nuclei position type
/// \param pCytoplasmMaterial The default cytoplsam material
/// \param pNucleiMaterial The default nuclei material
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::automaticFill(t_CellVarAtt_d pMasses, t_CellVarAtt_d pNucleusRadius,
	eNucleusPosType pNucleusPosType, G4Material* pCytoplasmMaterial, G4Material* pNucleiMaterial)
{

	std::map<LifeCycle, t_CellVarAtt_d > newMasses;
	std::map<LifeCycle, t_CellVarAtt_d > newNucleusRadii;
	std::map<LifeCycle, G4Material*> newCytoplasmMaterials;				///< \brief the material present on cells membrane
	std::map<LifeCycle, G4Material*> newNucleiMaterials;				///< \brief the material present on nuclei

	for(int iState = 0; iState != LifeCycles::NA; iState++)
	{
		LifeCycle lLifeState = static_cast<LifeCycle>(iState);

		// set mass
		newMasses.insert(make_pair(lLifeState, pMasses));
		// set nucleus radius
		newNucleusRadii.insert(make_pair(lLifeState, pNucleusRadius));
		// set cytoplsam materials
		newCytoplasmMaterials.insert(make_pair(lLifeState, pCytoplasmMaterial));
		// set nuclei materials
		newNucleiMaterials.insert(make_pair(lLifeState, pNucleiMaterial));
	}

	setNucleusPosType(pNucleusPosType);
	setMasses(newMasses);
	setNucleusRadius(newNucleusRadii);

	cytoplasmMaterials = newCytoplasmMaterials;
	nucleiMaterials = newNucleiMaterials;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double CellProperties::getMinRadius() const
{
	double minRadius = std::numeric_limits<double>::max();
	map<LifeCycle, t_CellVarAtt_d >::const_iterator itRad;

	for(itRad = nucleusRadius.begin(); itRad != nucleusRadius.end(); ++itRad)
	{
		if( itRad->second.var_min() < minRadius)
		{
			minRadius = itRad->second.var_min();
		}
	}
	return minRadius;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double CellProperties::getMaxRadius() const
{
	double maxRadius = -1;
	map<LifeCycle, t_CellVarAtt_d >::const_iterator itRad;

	for(itRad = nucleusRadius.begin(); itRad != nucleusRadius.end(); ++itRad)
	{
		if( itRad->second.var_max() > maxRadius)
		{
			maxRadius = itRad->second.var_max();
		}
	}
	return maxRadius;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pRad the new value to set
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::setMinNucleusRadius(double pRad)
{
	map<LifeCycle, t_CellVarAtt_d >::iterator itRad;

	for(itRad = nucleusRadius.begin(); itRad != nucleusRadius.end(); ++itRad)
	{
		itRad->second.setMin(pRad);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pRad the new value to set
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::setMaxNucleusRadius(double pRad)
{
	map<LifeCycle, t_CellVarAtt_d >::iterator itRad;

	for(itRad = nucleusRadius.begin(); itRad != nucleusRadius.end(); ++itRad)
	{
		itRad->second.setMax(pRad);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pMat the material to set for cytoplasm for all defined life cycles
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::setCytoplasmMaterial(G4Material* pMat)
{
	assert(pMat);

	map<LifeCycle, G4Material* >::iterator itMat;
	for(itMat = cytoplasmMaterials.begin(); itMat != cytoplasmMaterials.end(); ++itMat)
	{
		itMat->second = pMat;
	}

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pMat the material to set for nucleus for all defined life cycles
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CellProperties::setNucleusMaterial(G4Material* pMat)
{
	assert(pMat);

	map<LifeCycle, G4Material* >::iterator itMat;
	for(itMat = nucleiMaterials.begin(); itMat != nucleiMaterials.end(); ++itMat)
	{
		itMat->second = pMat;
	}
}
