/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ROUND_CELL_PROPERTIES_HH
#define ROUND_CELL_PROPERTIES_HH

#include "CellProperties.hh"

using namespace std;
//////////////////////////////////////////////////////////////////////////////
/// \brief Define all properties needed to defined a discoid cell
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class RoundCellProperties : public CellProperties
{
public:
	/// \brief constructor
	RoundCellProperties(	
		map<LifeCycles::LifeCycle, t_CellVarAtt_d > pRadius 		= map<LifeCycles::LifeCycle, t_CellVarAtt_d >(),
	 	map<LifeCycles::LifeCycle, t_CellVarAtt_d > pMasses			= map<LifeCycles::LifeCycle, t_CellVarAtt_d >(),
	 	map<LifeCycles::LifeCycle, t_CellVarAtt_d > pNucleusRadius 	= map<LifeCycles::LifeCycle, t_CellVarAtt_d >(),
		eNucleusPosType pNucleusPosType 							= BARYCENTER,
		map<LifeCycles::LifeCycle, G4Material*> pCytoplasmMaterial 	= map<LifeCycles::LifeCycle, G4Material*>(),
		map<LifeCycles::LifeCycle, G4Material*> pNucleiMaterial 	= map<LifeCycles::LifeCycle, G4Material*>()
		);

	/// \brief destructor
	virtual ~RoundCellProperties();

	/// \brief radius getter.
	map<LifeCycles::LifeCycle, t_CellVarAtt_d > getMembraneRadius() 						{ return membraneRadius;};
	/// \brief radius setter
	void setMembraneRadius(map<LifeCycles::LifeCycle, t_CellVarAtt_d > pMembraneRad)		{ membraneRadius = pMembraneRad;};
	/// \brief radius getter for a specific LifeCycle
	t_CellVarAtt_d getMembraneRadius(LifeCycles::LifeCycle) const;
 	/// \brief write cell proeprties information
	virtual QString propertiesInfos() const {return QString("Properties for a discoid or a spheroid.");};
	/// \brief export the cell proeprties to be saved
	virtual void writeProperties(QXmlStreamWriter& ) const;

	/// \brief will automatically fill cellPropertis with the given values. 
	/// \warning this will erase your previous settings
	void automaticFill(double radius, double pMasses, double nucleusRadius, eNucleusPosType eNucleusPosType = BARYCENTER, 
		G4Material* cytoplasmMaterial = NULL, G4Material* nucleiMaterial = NULL);
	/// \brief will automatically fill cellPropertis with the given values. 
	/// \warning this will erase your previous settings
	void automaticFill(t_CellVarAtt_d radius, t_CellVarAtt_d pMasses, t_CellVarAtt_d nucleusRadius, 
		eNucleusPosType eNucleusPosType = BARYCENTER, G4Material* cytoplasmMaterial=NULL, G4Material* nucleiMaterial=NULL);

	/// \brief set all lower bounddary value (minimal interval value) membrane radius to the given value
	void setMinMembraneRadius(double);
	/// \brief get all lower bounddary value (minimal interval value) membrane radius to the given value
	double getMinMembraneRadius() const;
	/// \brief set all upper boundary value (maximal interval value) membrane radius to the given value
	void setMaxMembraneRadius(double);
	/// \brief get all upper boundary value (maximal interval value) membrane radius to the given value
	double getMaxMembraneRadius() const;

private:
	/// \brief init the roundCellProperties. Called during construction
	void init();

private:
	map<LifeCycles::LifeCycle, t_CellVarAtt_d > membraneRadius;	///< \brief the radius of the cell.
	
};

#endif // DISCOID_OR_SPHEROID_CELL_PROPERTIES_HH