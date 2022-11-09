/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_PROPERTIES_HH
#define CELL_PROPERTIES_HH

#include <map>
#include <set>

#include "CellVariableAttribute.hh"
#include "ECellTypes.hh"
#include "ELifeCycles.hh"
#include "ENucleusPosType.hh"
#include "IDManager.hh"
#include "InformationSystemManager.hh"
#include "MaterialManager.hh"
#include "Shape.hh"
#include "Writable.hh"

// G4 import
#include "G4Material.hh"

static const QString CellProperties_IDMapName = "cellProp_IDsMap";

using namespace LifeCycles;
using namespace std;
//////////////////////////////////////////////////////////////////////////////
/// \brief Define all properties needed to defined a cell type
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class CellProperties : public Writable
{
public: 
	typedef CellVariableAttribute<double> t_CellVarAtt_d;		///< \brief cell variable attribute 

	friend class CellFactory;
public:
	/// \brief constructor
	CellProperties(	CellType pCellType, 
					eNucleusPosType pNucleusPosType						= BARYCENTER,
	 				map<LifeCycle, t_CellVarAtt_d > pMasses 			= map<LifeCycle, t_CellVarAtt_d >(),
	 				map<LifeCycle, t_CellVarAtt_d > pNucleusRadius 		= map<LifeCycle, t_CellVarAtt_d >(),
	 				map<LifeCycle, G4Material* > pCytoplasmMaterials 	= map<LifeCycle, G4Material* >(),
	 				map<LifeCycle, G4Material* > pNucleiMaterials 		= map<LifeCycle, G4Material* >(),
	 				bool pDeformable = true
	 				);
	/// \brief destructor
	virtual ~CellProperties();
	/// \brief return tje type of cell we have ( ROUND...)
	CellType	getCellType()	const										{return cellType;};

	/// \brief masses getter
	map<LifeCycle, t_CellVarAtt_d >  getMasses() const						{ return masses;};
	/// \brief masses setter
	void setMasses(map<LifeCycle, t_CellVarAtt_d > pMasses)					{ masses = pMasses;};
	/// \brief nucleus radius getter
	map<LifeCycle, t_CellVarAtt_d >  getNucleusRadius() const				{ return nucleusRadius;};
	/// \brief nucleus radius setter
	void setNucleusRadius(map<LifeCycle, t_CellVarAtt_d > pNucRadius)		{ nucleusRadius = pNucRadius;};
	/// \brief Weights getter for a specific life state.
	t_CellVarAtt_d getMasses(LifeCycle) const;
	/// \brief nucleus radius getter for a specific life state.
	t_CellVarAtt_d getNucleusRadius(LifeCycle) const;
	/// \brief return if the cell properties contain a nucleus radius for this life cycle.
	bool hasNucleusRadius(LifeCycle) const;
	/// \brief return the minimal radius from all radius possible throught all life cycle
	double getMinRadius() const;
	/// \brief return the maximal radius from all radius possible throught all life cycle
	double getMaxRadius() const;

	/// \brief cytoplsam material getter
	map<LifeCycle, G4Material*> getCytoplasmMaterials()	const				{ return cytoplasmMaterials;};
	/// \brief cyctoplam materials setted
	void setCytoplasmMaterials(map<LifeCycle, G4Material*> pCytoMat)		{ cytoplasmMaterials = pCytoMat;};
	/// \brief will set the given material for all the already setted life state
	void setCytoplasmMaterial(G4Material* pCytoMat);
	/// \brief nucleus material getter
	map<LifeCycle, G4Material*> getNucleusMaterials()	const				{ return nucleiMaterials;};
	/// \brief nucleus material setter
	void setNucleusMaterials(map<LifeCycle, G4Material*> pNucMat)			{ nucleiMaterials = pNucMat;};
	/// \brief will set the given material for all the already setted life state
	void setNucleusMaterial(G4Material* pCytoMat);	
	/// \brief cytoplsam material getter
	G4Material* getCytoplasmMaterial(LifeCycle)	const;		
	/// \brief nucleus material getter
	G4Material* getNucleusMaterial(LifeCycle)	const;		
	/// \brief display properties inforamtion to the QStrng format
	virtual QString propertiesInfos() const {return QString("Generic cell properties.");};
	/// \brief display information at the text format to show properties
	virtual void print() const;

	/// \brief deformable setter
	void setDeformable(bool b)												{deformable = b;};
	/// \brief deformable getter
	bool isDeformable()	const												{return deformable;};

	/// \brief  will write the cell proeprties flag 
	void write(QXmlStreamWriter& ) const;
	/// \brief will write cell properties 
	virtual void writeProperties(QXmlStreamWriter&) const;

	/// \brief nucleus position type getter
	eNucleusPosType getNucleusPosType() const 								{ return nucleusPosType;};
	/// \brief nucleus position type setter
	void setNucleusPosType(eNucleusPosType pPosType )						{ nucleusPosType = pPosType;};
	/// \brief return the ID attributed to the cell property
	unsigned long int getID() const 										{ return ID;};			
	/// \brief set all bottom range value (minimal interval value) nucleus radius to the given value
	void setMinNucleusRadius(double);
	/// \brief set all top range value (maximal interval value) nucleus radius to the given value
	void setMaxNucleusRadius(double);

	/// \brief will automatically fill cellPropertis with the given values. 
	/// \warning this will erase your previous settings
	void automaticFill(t_CellVarAtt_d pMasses, t_CellVarAtt_d nucleusRadius, 
		eNucleusPosType eNucleusPosType = BARYCENTER, G4Material* cytoplasmMaterial=NULL, G4Material* nucleiMaterial=NULL);

protected:
	CellType cellType;									///< \brief type of cell ( round...)
	eNucleusPosType nucleusPosType;						///< \brief return the nucleus position type						
	set<LifeCycle> availableLifeCycle;					///< \brief the available life cycles. If empty : all available.

	map<LifeCycle, t_CellVarAtt_d > masses;				///< \brief defined the weight variation throw life cycle from a min/max interval
	map<LifeCycle, t_CellVarAtt_d > nucleusRadius;		///< \brief defined the volume variation throw life cycle from a min/max interval

	map<LifeCycle, G4Material*> cytoplasmMaterials;		///< \brief the material present on cells membrane
	map<LifeCycle, G4Material*> nucleiMaterials;		///< \brief the material present on nuclei 
	bool deformable;									///< \brief is the cell membrane deformable ?

private:
	unsigned long int ID;								///< \brief the ID of the cell property
};

#endif // CELL_PROPERTIES_HH