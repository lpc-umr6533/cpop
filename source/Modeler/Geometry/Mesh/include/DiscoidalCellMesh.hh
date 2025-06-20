#ifndef DISCOIDAL_CELL_MESH_HH
#define DISCOIDAL_CELL_MESH_HH

#include "Voronoi_2D_Mesh.hh"

#include "CellSettings.hh"
#include "CellMesh.hh"
#include "DiscoidalCell.hh"

using namespace Settings::nCell;

/// \brief The specific mesher for the discoidal cell.
/// \warning the export will probably modify the nucleus radius and the nucleus
/// @author Henri Payno
class DiscoidalCellMesh : public Voronoi_2D_Mesh, public CellMesh {
public:
	DiscoidalCellMesh(unsigned int nbFacetPerCell, double delta, std::set<t_Cell_2* > pCells = std::set<t_Cell_2*>());
	~DiscoidalCellMesh() override;

	/// \brief the mesh exporter
	int exportToFile(std::string const&, MeshOutFormats::outputFormat, bool divided = false) override;

	/// \brief export all cells on the same file
	int exportToFileOff_undivided(std::string const& path, DiscoidalCells const&) override;
	// /// \brief return the spheroidal cell structure.
	// virtual DiscoidalCell* getDiscoidalCell(Point_2 ptOrigin);
	/// \brief clean data structures
	void clean() override;
	/// \brief get all polygons from the delaunay mesh.
	std::vector<DiscoidalCell*> generateMesh() override;

};

#endif
