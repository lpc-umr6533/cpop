#include <cstdio>

#ifndef SPHEROIDAL_CELL_MESH_HH
#define SPHEROIDAL_CELL_MESH_HH

#include "CellMesh.hh"
#include "Voronoi_3D_Mesh.hh"

#include "CellSettings.hh"
#include "SpheroidalCell.hh"

#ifdef WITH_GEANT_4
	#include "G4LogicalVolume.hh"
	#include "G4PVPlacement.hh"
	#include "G4LogicalVolume.hh"
#endif // WITH_GEANT_4

using namespace Settings::nCell;

#ifdef NDEBUG
	#define DEFAULT_OVER_LAP = 1;
#else
	#define DEFAULT_OVER_LAP = 0;
#endif

/// \brief The specific mesher for the spheroidal cell.
/// \warning the export will probably modify the nucleus radius and the nucleus
/// @author Henri Payno
class SpheroidalCellMesh : public Voronoi_3D_Mesh, public CellMesh {
public:
	SpheroidalCellMesh(unsigned int nbFacetPerCell, double delta, std::set<t_Cell_3* > pCells = std::set<t_Cell_3*>());
	~SpheroidalCellMesh() override;

	/// \brief the mesh exporter
	int exportToFile(std::string const&, MeshOutFormats::outputFormat, bool divided = false) override;
	/// \brief return curently the number of visible cell
	unsigned int getNumberOfVisibleCell()	{ return Voronoi_3D_Mesh::_delaunay.number_of_vertices(); }
	/// \brief generate all cell structures.
	std::vector<SpheroidalCell*> generateMesh() override;

#if defined(WITH_GEANT_4) || defined(WITH_GDML_EXPORT)
	/// \brief export the configuration to a G4PVPlacement. The one returned is the "world"/top G4 entity
	virtual G4PVPlacement* convertToG4World(
		bool checkOverlaps = false,
		G4Material* pMaterialBetwwenCell = nullptr,
		std::map<const G4LogicalVolume*, const t_Cell_3* > * mapCells = nullptr,
		std::map<const G4LogicalVolume*, const t_Nucleus_3*> * mapNuclei = nullptr,
		bool exportNuclei = true
	);

	/// \brief return the bounding box englobing the spheroid. Without converting cell into G4 entities
	virtual G4LogicalVolume* getG4BoundingLogicalVolume(G4ThreeVector&, G4Material* pMaterialBetwwenCell = nullptr);

	/// \brief export the configuration to a G4LogicalVolume. The one returned is the "world"/top G4 entity
	virtual G4LogicalVolume* convertToG4Logical(
		G4LogicalVolume* parent,
		bool checkOverlaps = false,
		G4Material* pMaterialBetwwenCell = nullptr,
		std::map<const G4LogicalVolume*,
		const t_Cell_3* > * mapCells = nullptr,
		std::map<const G4LogicalVolume*, const t_Nucleus_3*> * mapNuclei = nullptr,
		bool pExportNuclei = true
	);
#endif

protected:
	/// \brief export all cells on the same file
	int exportToFileOff_undivided(std::string const&, SpheroidalCells const&) override;
	/// \brief clean data structures
	void clean() override;

#ifdef WITH_GDML_EXPORT
	/// \brief export to a GDML file
	virtual int exportToFileGDML(std::string const&, SpheroidalCells const&, bool);
#endif

};

#endif
