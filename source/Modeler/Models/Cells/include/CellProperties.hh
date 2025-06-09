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

/// \brief Define all properties needed to defined a cell type
/// @author Henri Payno
class CellProperties : public Writable {
public:
	typedef CellVariableAttribute<double> t_CellVarAtt_d;		///< \brief cell variable attribute

	friend class CellFactory;

public:
	CellProperties(
		CellType pCellType,
		eNucleusPosType pNucleusPosType	= BARYCENTER,
		std::map<LifeCycle, t_CellVarAtt_d> pMasses = {},
		std::map<LifeCycle, t_CellVarAtt_d> pNucleusRadius = {},
		std::map<LifeCycle, G4Material*> pCytoplasmMaterials = {},
		std::map<LifeCycle, G4Material*> pNucleiMaterials = {},
		bool pDeformable = true
	);

	/// \brief return tje type of cell we have ( ROUND...)
	[[nodiscard]] CellType getCellType()	const { return _cellType; }

	/// \brief masses getter
	[[nodiscard]] std::map<LifeCycle, t_CellVarAtt_d> getMasses() const { return _masses; };
	/// \brief masses setter
	void setMasses(std::map<LifeCycle, t_CellVarAtt_d> pMasses) { _masses = pMasses; }
	/// \brief nucleus radius getter
	[[nodiscard]] std::map<LifeCycle, t_CellVarAtt_d> getNucleusRadius() const { return _nucleusRadius; }
	/// \brief nucleus radius setter
	void setNucleusRadius(std::map<LifeCycle, t_CellVarAtt_d > pNucRadius) { _nucleusRadius = pNucRadius; }
	/// \brief Weights getter for a specific life state.
	[[nodiscard]] t_CellVarAtt_d getMasses(LifeCycle) const;
	/// \brief nucleus radius getter for a specific life state.
	[[nodiscard]] t_CellVarAtt_d getNucleusRadius(LifeCycle) const;
	/// \brief return if the cell properties contain a nucleus radius for this life cycle.
	[[nodiscard]] bool hasNucleusRadius(LifeCycle) const;
	/// \brief return the minimal radius from all radius possible throught all life cycle
	[[nodiscard]] double getMinRadius() const;
	/// \brief return the maximal radius from all radius possible throught all life cycle
	[[nodiscard]] double getMaxRadius() const;

	/// \brief cytoplsam material getter
	[[nodiscard]] std::map<LifeCycle, G4Material*> getCytoplasmMaterials() const { return _cytoplasmMaterials; }
	/// \brief cyctoplam materials setted
	void setCytoplasmMaterials(std::map<LifeCycle, G4Material*> pCytoMat) { _cytoplasmMaterials = pCytoMat; }
	/// \brief will set the given material for all the already setted life state
	void setCytoplasmMaterial(G4Material* pCytoMat);
	/// \brief nucleus material getter
	[[nodiscard]] std::map<LifeCycle, G4Material*> getNucleusMaterials() const { return _nucleiMaterials; }
	/// \brief nucleus material setter
	void setNucleusMaterials(std::map<LifeCycle, G4Material*> pNucMat) { _nucleiMaterials = pNucMat; }
	/// \brief will set the given material for all the already setted life state
	void setNucleusMaterial(G4Material* pCytoMat);
	/// \brief cytoplsam material getter
	[[nodiscard]] G4Material* getCytoplasmMaterial(LifeCycle)	const;
	/// \brief nucleus material getter
	[[nodiscard]] G4Material* getNucleusMaterial(LifeCycle)	const;
	/// \brief display properties inforamtion to the QStrng format
	[[nodiscard]] virtual QString propertiesInfos() const {return {"Generic cell properties."}; }
	/// \brief display information at the text format to show properties
	virtual void print() const;

	/// \brief deformable setter
	void setDeformable(bool b) { _deformable = b; }
	/// \brief deformable getter
	[[nodiscard]] bool isDeformable()	const { return _deformable; }

	/// \brief  will write the cell proeprties flag
	void write(QXmlStreamWriter& ) const override;
	/// \brief will write cell properties
	virtual void writeProperties(QXmlStreamWriter&) const;

	/// \brief nucleus position type getter
	[[nodiscard]] eNucleusPosType getNucleusPosType() const { return _nucleusPosType; }
	/// \brief nucleus position type setter
	void setNucleusPosType(eNucleusPosType pPosType) { _nucleusPosType = pPosType; }
	/// \brief return the ID attributed to the cell property
	[[nodiscard]] unsigned long int getID() const { return _id; }
	/// \brief set all bottom range value (minimal interval value) nucleus radius to the given value
	void setMinNucleusRadius(double);
	/// \brief set all top range value (maximal interval value) nucleus radius to the given value
	void setMaxNucleusRadius(double);

	/// \brief will automatically fill cellPropertis with the given values.
	/// \warning this will erase your previous settings
	void automaticFill(t_CellVarAtt_d pMasses, t_CellVarAtt_d nucleusRadius,
		eNucleusPosType eNucleusPosType = BARYCENTER, G4Material* cytoplasmMaterial = nullptr, G4Material* nucleiMaterial = nullptr);

protected:
	CellType _cellType;                         ///< \brief type of cell ( round...)
	eNucleusPosType _nucleusPosType;            ///< \brief return the nucleus position type
	std::set<LifeCycle> availableLifeCycle;     ///< \brief the available life cycles. If empty : all available.

	std::map<LifeCycle, t_CellVarAtt_d > _masses;         ///< \brief defined the weight variation throw life cycle from a min/max interval
	std::map<LifeCycle, t_CellVarAtt_d > _nucleusRadius;  ///< \brief defined the volume variation throw life cycle from a min/max interval

	std::map<LifeCycle, G4Material*> _cytoplasmMaterials; ///< \brief the material present on cells membrane
	std::map<LifeCycle, G4Material*> _nucleiMaterials;    ///< \brief the material present on nuclei
	bool _deformable;                                     ///< \brief is the cell membrane deformable ?

private:
	unsigned long int _id;                      ///< \brief the ID of the cell property
};

#endif
