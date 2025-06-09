#ifndef ROUND_CELL_PROPERTIES_HH
#define ROUND_CELL_PROPERTIES_HH

#include "CellProperties.hh"

/// \brief Define all properties needed to defined a discoid cell
/// @author Henri Payno
class RoundCellProperties : public CellProperties {
public:
	RoundCellProperties(
		std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pRadius = {},
	 	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pMasses = {},
	 	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pNucleusRadius = {},
		eNucleusPosType pNucleusPosType = BARYCENTER,
		std::map<LifeCycles::LifeCycle, G4Material*> pCytoplasmMaterial = {},
		std::map<LifeCycles::LifeCycle, G4Material*> pNucleiMaterial = {}
	);

	/// \brief radius getter.
	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> getMembraneRadius() { return _membraneRadius; }
	/// \brief radius setter
	void setMembraneRadius(std::map<LifeCycles::LifeCycle, t_CellVarAtt_d> pMembraneRad) { _membraneRadius = pMembraneRad; }
	/// \brief radius getter for a specific LifeCycle
	[[nodiscard]] t_CellVarAtt_d getMembraneRadius(LifeCycles::LifeCycle) const;
 	/// \brief write cell proeprties information
	[[nodiscard]] QString propertiesInfos() const override { return {"Properties for a discoid or a spheroid."}; }
	/// \brief export the cell proeprties to be saved
	void writeProperties(QXmlStreamWriter&) const override;

	/// \brief will automatically fill cellPropertis with the given values.
	/// \warning this will erase your previous settings
	void automaticFill(
		double radius, double pMasses, double nucleusRadius, eNucleusPosType eNucleusPosType = BARYCENTER,
		G4Material* cytoplasmMaterial = nullptr, G4Material* nucleiMaterial = nullptr
	);
	/// \brief will automatically fill cellPropertis with the given values.
	/// \warning this will erase your previous settings
	void automaticFill(
		t_CellVarAtt_d radius, t_CellVarAtt_d pMasses, t_CellVarAtt_d nucleusRadius,
		eNucleusPosType eNucleusPosType = BARYCENTER, G4Material* cytoplasmMaterial=nullptr, G4Material* nucleiMaterial=nullptr
	);

	/// \brief set all lower bounddary value (minimal interval value) membrane radius to the given value
	void setMinMembraneRadius(double);
	/// \brief get all lower bounddary value (minimal interval value) membrane radius to the given value
	[[nodiscard]] double getMinMembraneRadius() const;
	/// \brief set all upper boundary value (maximal interval value) membrane radius to the given value
	void setMaxMembraneRadius(double);
	/// \brief get all upper boundary value (maximal interval value) membrane radius to the given value
	[[nodiscard]] double getMaxMembraneRadius() const;

private:
	/// \brief init the roundCellProperties. Called during construction
	void init();

private:
	std::map<LifeCycles::LifeCycle, t_CellVarAtt_d > _membraneRadius;	///< \brief the radius of the cell.

};

#endif
