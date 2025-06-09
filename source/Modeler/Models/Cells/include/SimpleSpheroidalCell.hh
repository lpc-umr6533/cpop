#ifndef SIMPLE_SPHEROIDAL_CELL_HH
#define SIMPLE_SPHEROIDAL_CELL_HH

#include "SpheroidalCell.hh"
#include "RoundNucleus.hh"

/// \brief define a simple spheroidal cell.
/// A simple cell is composed of a deformable round cell membrane and a single round nucleus.
/// @author Henri Payno
class SimpleSpheroidalCell : public SpheroidalCell {
public:
	SimpleSpheroidalCell(
		const CellProperties*, Point_3 pOrigin, double pSpheroidRadius, double pNucleusRadius, eNucleusPosType pPosType = BARYCENTER,
		double pMass=1., Mesh3D::Polyhedron_3 pMembraneShape = {}
	);

	/// \brief return the position of the nucleus according to his type of nucleus position
	/// \warning this is not necessarily the one setted
	[[nodiscard]] Point_3 getNucleusCenter(eNucleusPosType nucleusPositionType) const override;
	/// \brief set the position of the nucleus according to his type of nucleus position
	void setNucleusCenter() override;
	/// \brief will generate the nucleus shape
	void generateNuclei(std::vector<Plane_3*>) override;
	/// \brief return true if nuclei radius are coherent
	[[nodiscard]] bool checkNucleiRadius() const override { return _nucleus->getRadius() > 0; }
	/// \brief return the cell description
	[[nodiscard]] QString getDescription() const override { return "SimpleSpheroidalCell"; }
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	// virtual Point_3 getSpotOnCytoplasm() const;
	/// \brief return the round nucleus
	[[nodiscard]] RoundNucleus<double, Point_3, Vector_3>* getNucleus() const { return _nucleus; }
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	[[nodiscard]] Point_3 getSpotOnCytoplasm() const override;

protected:
	/// \brief max rattio getter
	[[nodiscard]] double getMaxRatioNucleusToCell() const { return _maxRatioToCellRadius; }

	/// \brief reset the mesh
	void resetMesh() override;

private:
	/// \brief the nucleus
	RoundNucleus<double, Point_3, Vector_3>* _nucleus;

};

#endif
