#ifndef SPHEROIDAL_CELL_HH
#define SPHEROIDAL_CELL_HH

#include "RoundCell.hh"

#include "CellSettings.hh"
#include "Mesh3DSettings.hh"
#include "MeshOutFormats.hh"

#include <map>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;
using namespace Settings::nCell;

/// \brief A spheroidal cell is defined by her cell membrane represented by a deformable spheroid.
/// \details spheroidal cells contained n organelles.
/// @author Henri Payno
class SpheroidalCell : public RoundCell<double, Point_3, Vector_3> {
	using shapePtIt = Mesh3D::Polyhedron_3::Point_iterator;		///< \brief 3D polyhedron point iterator
	using shapeFacetIt = Mesh3D::Polyhedron_3::Facet_iterator;	///< \brief 3D polyhedron facet iterator
public:
	SpheroidalCell(const CellProperties*, Point_3 pOrigin, double pSpheroidRadius, double pMass, Mesh3D::Polyhedron_3 pMembraneShape);
	~SpheroidalCell() override;

	/// \brief membrane volume getter
	[[nodiscard]] double getCytoplasmVolume() const;

	/// \brief iterator to the shape point begin
	shapePtIt shape_points_begin() { return _shape->points_begin(); }
	/// \brief iterator to the shape point begin
	shapePtIt shape_points_end() { return _shape->points_end(); }
	/// \brief shape getter
	Mesh3D::Polyhedron_3* getShape() { return _shape; }

	/// \brief shape facet begin getter
	shapeFacetIt shape_facets_begin() { return _shape->facets_begin(); }
	/// \brief shape facet end getter
	shapeFacetIt shape_facets_end() { return _shape->facets_end(); }
	/// \brief return a random point on/inside the membrane
	[[nodiscard]] Point_3 getSpotOnCellMembrane() const override;
	/// \brief return a random point inside the cytoplasm, avoiding membrane and nucleus
	[[nodiscard]] Point_3 getSpotOnCytoplasm() const override;
	/// \brief return a random point on/inside the nucleus
	[[nodiscard]] Point_3 getSpotOnNuclei() const override;
	/// \brief return a random point on/inside the nucleus
	[[nodiscard]] Point_3 getSpotInNuclei() const override;
	/// \brief return true if the point is inside the cell
	[[nodiscard]] bool hasIn(Point_3) const override;
	/// \brief will generate the nucleus shape
	virtual void generateNuclei(std::vector<Plane_3*>) = 0;

	/// \brief return the position of the nucleus according to his type of nucleus position
	[[nodiscard]] virtual Point_3 getNucleusCenter(eNucleusPosType nucleusPositionType) const = 0;
	/// \brief set the position of the nucleus according to his type of nucleus position
	virtual void setNucleusCenter() = 0;
	/// \brief return true if nuclei radius are coherent
	[[nodiscard]] bool checkNucleiRadius() const override = 0;

	virtual void exportNucleiToStream(std::ofstream&) const = 0;

	/// \brief reset the mesh
	void resetMesh() override;

	/// \brief return the statistic about the current mesh
	[[nodiscard]] std::string getMeshStats(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the cell description
	[[nodiscard]] std::string getDescription() const override = 0;
	/// \brief return the volume defined by the mesh
	[[nodiscard]] double getMeshVolume(MeshOutFormats::outputFormat meshType) const override;
	// \brief return the volume occupy by the cell mesh without nuclei meshes volumes
	[[nodiscard]] double getCytoplasmMeshVolume(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the volume occupy by the nuclei meshes
	[[nodiscard]] double getNucleiMeshesSumVolume(MeshOutFormats::outputFormat meshType) const;
	/// \brief return the surface represented by the mesh
	[[nodiscard]] double getMembraneMeshSurfaceArea() const { return _sumMembraneMeshArea; }
	/// \brief compute the mesh surface
	void computeMembraneSurfaceArea();
	/// \brief return true if the cell own a mesh
	[[nodiscard]] bool hasMesh() const override;

	/// \brief return true if the spatialable cross the bounding box
	bool cross(const BoundingBox<Point_3>*) const override;
	/// \brief return true if the point is in the bounding box
	bool isIn(const BoundingBox<Point_3>*) const override;

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	/// \brief convert the membrane shape to a G4 entity
	virtual G4LogicalVolume* convertMembraneToG4(std::string const&);
	/// \brief convert the cell geometries (including nuclei) to G4 geometries
	virtual G4PVPlacement* convertToG4Structure(
		G4LogicalVolume* pMother,
		std::string const& pName,
		bool checkOverLaps,
		const std::map<SpheroidalCell*, std::set<const SpheroidalCell*>>* pNeighbourCells,
		unsigned int pNbFacet,
		double pDeltaWin,
		std::map<const G4LogicalVolume*, const t_Cell_3*>* pCellMap = nullptr,
		std::map<const G4LogicalVolume*, const t_Nucleus_3*>* pNucleiMap = nullptr,
		bool pExportNuclei = true
	);
#endif

protected:
	/// \brief The polyhedron representing the cell boundaries.
	Mesh3D::Polyhedron_3* _shape;

	/// \brief link sum of facet areas to facet
	/// used to obtain uniform spot on facet.
	std::map<double, Triangle_3> areasToFacet;

	double _sumMembraneMeshArea; ///< \brief the membrane mesh surface ( sum of all facet surfaces )

};

#endif
