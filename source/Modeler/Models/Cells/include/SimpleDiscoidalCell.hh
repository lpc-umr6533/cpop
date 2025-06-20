#ifndef SIMPLE_DISCOIDAL_CELL_STRUCTURE_HH
#define SIMPLE_DISCOIDAL_CELL_STRUCTURE_HH

#include "DiscoidalCell.hh"
#include "GeometrySettings.hh"
#include "Mesh2DSettings.hh"
#include "RoundNucleus.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;

/// \brief define a simple discoidal cell.
/// A simple cell is composed of a deformable round cell membrane and a single round nucleus.
/// @author Henri Payno
class SimpleDiscoidalCell : public DiscoidalCell {
	using shapeIterator = std::vector<Mesh2D::Segment_2>::iterator;	///< \brief iterator on membrane cell mesh

public:
	SimpleDiscoidalCell(
		const CellProperties*, Point_2 pOrigin, double pSpheroidRadius, double pNucleusRadius, eNucleusPosType pPosType = BARYCENTER,
		double pMass = 1., std::vector<Mesh2D::Segment_2> pMembraneShape=std::vector<Mesh2D::Segment_2>()
	);

	/// \brief membrane area getter
	//virtual double getCytoplasmArea() const = 0;

	/// \brief return the position of the nucleus according to his type of nucleus position
	/// \warning this is not necessarily the one setted
	[[nodiscard]] Point_2 getNucleusCenter(eNucleusPosType nucleusPositionType) const override;
	/// \brief set the position of the nucleus according to his type of nucleus position
	void setNucleusCenter() override;
	/// \brief return the set of points defining nuclei
	[[nodiscard]] std::vector<std::set<Point_2>> getNucleiPoints() const;
	/// \brief will generate the nucleus shape
	void generateNuclei(std::vector<Line_2*> intersections) override;
	/// \brief return true if nuclei radius are coherent
	[[nodiscard]] bool checkNucleiRadius() const override { return _nucleus->getRadius() > 0; }
	/// \brief reset the mesh
	void resetMesh() override;
	/// \brief return the cell description
	[[nodiscard]] std::string getDescription() const override { return "SimpleDiscoidalCell"; }

private:
	/// \brief the nucleus
	RoundNucleus<double, Point_2, Vector_2>* _nucleus;

};

#endif
