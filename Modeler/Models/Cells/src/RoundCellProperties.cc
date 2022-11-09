/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "InformationSystemManager.hh"
#include "RoundCellProperties.hh"
#include "XMLSettings.hh"

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define _USE_MATH_DEFINES
	#include <math.h>
#endif

#include <map>

using namespace XML::CPOP_Flag;

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pRadius 				The radius min/max values for each life state of the cell
/// \param pMasses 				The masses min/max values for each life state of the cell
/// \param pNucleusRadius 		The radii to set to the nucleus
/// \param pNucleusPosType		The position type to set to nuclei
/// \param pCytoplasmMaterial 	The material included on the membrane.
/// \param pNucleiMaterial 		The material included on the nucleus.
//////////////////////////////////////////////////////////////////////////////////////////
RoundCellProperties::RoundCellProperties(
		std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pRadius,
	 	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pMasses,
	 	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pNucleusRadius,
		eNucleusPosType pNucleusPosType,
		std::map<LifeCycles::LifeCycle, G4Material*> pCytoplasmMaterial,
		std::map<LifeCycles::LifeCycle, G4Material*> pNucleiMaterial ):
	CellProperties( SIMPLE_ROUND, pNucleusPosType, pMasses, pNucleusRadius, pCytoplasmMaterial, pNucleiMaterial),
	membraneRadius(pRadius)
{
	init();
}

//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::init()
{
	setDeformable(true); 					//< discoid and spheroid are crossable.
}

//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
RoundCellProperties::~RoundCellProperties()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::writeProperties(QXmlStreamWriter& writer) const
{

	CellProperties::writeProperties(writer);
	// write radius...
	{
		writer.writeStartElement(membrane_radius_flag);
		map<LifeCycle, t_CellVarAtt_d >::const_iterator itMenbraneRadParam;
		for(itMenbraneRadParam = membraneRadius.begin(); itMenbraneRadParam != membraneRadius.end(); ++itMenbraneRadParam)
		{
			writer.writeStartElement(variable_attribute_flag);
			writer.writeAttribute(life_cycle_flag, 	QString::number(itMenbraneRadParam->first));
			writer.writeAttribute(min_flag, 		QString::number(itMenbraneRadParam->second.var_min()) );
			writer.writeAttribute(max_flag, 		QString::number(itMenbraneRadParam->second.var_max()) );
			writer.writeEndElement(); // var_prop
		}	
		writer.writeEndElement(); // nucleus_radius_flag
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pLifeCycle The life state we want the radius for
//////////////////////////////////////////////////////////////////////////////////////////
CellVariableAttribute<double> RoundCellProperties::getMembraneRadius(LifeCycles::LifeCycle pLifeCycle) const
{
	if(membraneRadius.find(pLifeCycle) == membraneRadius.end())
	{
		QString mes = "unknow membrane radius for requested LifeCycle" + QString::number(pLifeCycle);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mes.toStdString(), "Cell properties");
		return CellVariableAttribute<double>(0., 0.);
	}
	return membraneRadius.find(pLifeCycle)->second;
}

//////////////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::automaticFill(
	double pRadius, 
	double pMass, 
	double pNucleusRadius, 
	eNucleusPosType eNucleusPosType, 
	G4Material* cytoplasmMaterial, 
	G4Material* nucleiMaterial)
{
	automaticFill(
		t_CellVarAtt_d(pRadius, pRadius),
		t_CellVarAtt_d(pMass, pMass),
		t_CellVarAtt_d(pNucleusRadius, pNucleusRadius),
		eNucleusPosType,
		cytoplasmMaterial,
		nucleiMaterial
	 );
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \param pMembraneRadius The default membrane radius values 
/// \param pMasses The default masses values 
/// \param pNucleusRadius The default nuclei radius values 
/// \param pNucleusPosType The default nuclei position type
/// \param pCytoplasmMaterial The default cytoplsam material
/// \param pNucleiMaterial The default nuclei material
//////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::automaticFill(t_CellVarAtt_d pMembraneRadius, t_CellVarAtt_d pMasses, 
	t_CellVarAtt_d pNucleusRadius, eNucleusPosType pNucleusPosType, G4Material* pCytoplasmMaterial, G4Material* pNucleiMaterial)
{
	CellProperties::automaticFill(
		pMasses,				// masses
		pNucleusRadius, 		// nucleus radius
		pNucleusPosType,		// nucleus pos type
		pCytoplasmMaterial,		// cytoplasm material
		pNucleiMaterial			// nucleus material
		);

	std::map<LifeCycle, t_CellVarAtt_d > newMembraneRadii;	
	
	for(int iState = 0; iState != LifeCycles::NA; iState++)
	{
		LifeCycle lLifeState = static_cast<LifeCycle>(iState);
		// set membrane radius
		newMembraneRadii.insert(make_pair(lLifeState, pMembraneRadius));
	}
	this->setMembraneRadius(newMembraneRadii);
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::setMinMembraneRadius(double pRad)
{
	map<LifeCycle, t_CellVarAtt_d >::iterator itRad; 

	for(itRad = membraneRadius.begin(); itRad != membraneRadius.end(); ++itRad)
	{
		itRad->second.setMin(pRad);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void RoundCellProperties::setMaxMembraneRadius(double pRad)
{
	map<LifeCycle, t_CellVarAtt_d >::iterator itRad; 

	for(itRad = membraneRadius.begin(); itRad != membraneRadius.end(); ++itRad)
	{
		itRad->second.setMax(pRad);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
double RoundCellProperties::getMinMembraneRadius() const
{
	double minRadius = std::numeric_limits<double>::max();
	map<LifeCycle, t_CellVarAtt_d >::const_iterator itRad; 
	
	for(itRad = membraneRadius.begin(); itRad != membraneRadius.end(); ++itRad)
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
double RoundCellProperties::getMaxMembraneRadius() const
{
	double maxRadius = -1;
	map<LifeCycle, t_CellVarAtt_d >::const_iterator itRad; 

	for(itRad = membraneRadius.begin(); itRad != membraneRadius.end(); ++itRad)
	{
		if( itRad->second.var_max() > maxRadius)
		{
			maxRadius = itRad->second.var_max();
		}
	}
	return maxRadius;
}