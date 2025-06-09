#ifndef DISCOIDAL_CELL_STRUCTURE_HH
#define DISCOIDAL_CELL_STRUCTURE_HH

#include "RoundCell.hh"
#include "RoundNucleus.hh"
#include "Mesh2DSettings.hh"

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;

/// \brief A discoidal cell is defined by her cell membrane represented by a deformable circle/disc.
/// \details discoidal cells contained n organelles.
class DiscoidalCell : public RoundCell<double, Point_2, Vector_2> {
	using shapeIterator = std::vector<Mesh2D::Segment_2>::iterator;	///< \brief iterator on Discoidal cell membrane mesh

public:
	DiscoidalCell(const CellProperties*, Point_2 pOrigin, double pSpheroidRadius, double pMass, std::vector<Mesh2D::Segment_2> pMembraneShape);
	~DiscoidalCell() override;

	/// \brief membrane area getter
	[[nodiscard]] virtual double getCellArea() const;
	/// \brief membrane area getter
	[[nodiscard]] virtual double getMembraneSurface() const;
	/// \brief return the iterator on the begining of the shape
	shapeIterator shape_begin() { return _shape->begin(); }
	/// \brief return the iterator on the end of the shape
	shapeIterator shape_end() { return _shape->end(); }
	/// \brief return the number of element on the shape
	unsigned int shape_size() { return _shape->size(); }
	/// \brief add a segment to the shae
	void add_to_shape(Segment_2 s) { _shape->push_back(s); }
	/// \brief shape getter
	[[nodiscard]] std::vector<Mesh2D::Segment_2>* getShape() const { return _shape; }
	/// \brief return a random point on/inside the membrane
	[[nodiscard]] Point_2 getSpotOnCellMembrane() const override;
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	[[nodiscard]] Point_2 getSpotOnCytoplasm() const override;
	/// \brief return a random point on/inside the nucleus
	[[nodiscard]] Point_2 getSpotOnNuclei() const override;
	/// \brief return a random point on/inside the nucleus
	[[nodiscard]] Point_2 getSpotInNuclei() const override;
	/// \brief return true if the point is inside the cell
	[[nodiscard]] bool hasIn(Point_2) const override;
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(std::vector<Line_2*> intersections) = 0;

	/// \brief return the position of the nucleus according to his type of nucleus position
	[[nodiscard]] virtual Point_2 getNucleusCenter(eNucleusPosType nucleusPositionType) const = 0;
	/// \brief set the position of the nucleus according to his type of nucleus position
	virtual void setNucleusCenter() = 0;
	/// \brief return true if nuclei radius are coherent
	[[nodiscard]] bool checkNucleiRadius() const override = 0;

	/// \brief reset the mesh
	void resetMesh() override;
	/// \brief return the cell description
	[[nodiscard]] QString getDescription() const override = 0;

	/// \brief return the volume defined by the mesh
	[[nodiscard]] double getMeshVolume(MeshOutFormats::outputFormat ) const override { return -1.; }

	/// \brief return true if the cell own a mesh
	[[nodiscard]] bool hasMesh() const override;

	/// \brief return true if the spatialable cross the bounding box
	bool cross(const BoundingBox<Point_2>*) const override;
	/// \brief return true if the point is in the bounding box
	bool isIn(const BoundingBox<Point_2>*) const override;

protected:
	/// \brief The polyhedron representing the cell boundaries.
	std::vector<Mesh2D::Segment_2>* _shape;

};

#endif
