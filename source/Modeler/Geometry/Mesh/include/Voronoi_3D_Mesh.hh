#ifndef VORONOI_3D_MESH_HH
#define VORONOI_3D_MESH_HH

#include "AgentSettings.hh"
#include "CellSettings.hh"
#include "Mesh.hh"
#include "Mesh3DSettings.hh"
#include "SpheroidalCell.hh"
#include "Delaunay_3D_SDS.hh"

#include <set>

//// CGAL import
// includes for defining the Voronoi diagram adaptor
#include <CGAL/basic.h>
#include <CGAL/bounding_box.h>

using namespace Settings::nCell;
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

/// \brief Define a 2D_Voronoi mesh
/// \warning this class is able to deal with weighted points.
/// If none specified will create homogeneous weight
/// @author Henri Payno
class Voronoi_3D_Mesh : public Mesh<double, Point_3, Vector_3>, public Delaunay_3D_SDS {
	using Delaunay_3D_SDS::add;     // overloaded function
	using Delaunay_3D_SDS::remove;  // overloaded function

public:
	using SpheroidalCells = std::vector<SpheroidalCell*>;

public:
	Voronoi_3D_Mesh(unsigned int pMaxNbFacet, double delta, std::set<t_Cell_3*> pInitSpatialables = std::set<t_Cell_3*>());
	~Voronoi_3D_Mesh() override;

	/// \brief add a point to the mesh
	bool add(t_Cell_3*) override;
	/// \brief remove a point to the mesh
	void remove(t_Cell_3*) override;
	/// \brief the mesh exporter
	int exportToFile(std::string const&, MeshOutFormats::outputFormat, bool) override;
	/// \brief the bounding box getter.
	K::Iso_cuboid_3 getBoundingBox() const;

	/// \brief max number of segment per cell setter
	void setNBMaxFacetPerCell(unsigned int pNb)	{ _maxNumberOfFacetPerCell = pNb; }
	/// \brief max number of segment per cell getter
	unsigned int getMaxNbFacetPerCell() { return _maxNumberOfFacetPerCell; }

	/// \brief threshold refinnement setter
	void setDeltaGain(double pGain) { _deltaGain = pGain; }
	/// \brief threshold refinnement setter
	double getDeltaWin() { return _deltaGain; }
	/// \brief update cell shapes according to other cell contained on the mesh
	virtual std::vector<SpheroidalCell*> generateMesh();
	/// \brief check if the mesh is valid <=> no mesh recovery
	bool isValid();
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_3*> getCells() const override;
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_3*> getCellsWithShape() const override;

protected:
	/// \brief export to an off file
	virtual int exportToFileOff(std::string const& path, SpheroidalCells const& cells, bool divided);
	/// \brief export all cells on the same file
	virtual int exportToFileOff_undivided(std::string const& path, SpheroidalCells const& cells);
	/// \brief export all cells on the same file
	virtual int exportToFileOff_divided(std::string const& path, SpheroidalCells const& cells);

	virtual int exportToFileSTL(std::string const& path, SpheroidalCells const& cells, bool divided);
	virtual int exportToFileSTL_undivided(std::string const& path, SpheroidalCells const& cells);
	virtual int exportToFileSTL_divided(std::string const& path, SpheroidalCells const& cells);

	virtual int exportNucleiToFile(std::string const& path, SpheroidalCells const&, bool divided);
	virtual int exportNucleiToFileUndivided(std::string const& path, SpheroidalCells const&);
	virtual int exportNucleiToFileDivided(std::string const& path, SpheroidalCells const&);

	/// \brief get all polygons from the delaunay mesh.
	std::vector<SpheroidalCell*> getCellsStructure();
	/// \brief define all the neighbouring cell for each cell
	void generateNeighbourhood();
	/// \brief clean data structures
	void clean() override;

protected:
	/// \brief remove all point on the mesh with are "include" on the area of influence of an other point.
	virtual void removeConflicts();

protected:
	std::map<Point_3, double> _weights; ///< \brief the map of weights
	std::map<SpheroidalCell*, std::set<const SpheroidalCell*>> _neighboursCell; ///< \brief for each cell the neighbourhood

private:
	double _minWeight;                          ///< \brief the minimal weight existing, used to create the Bounding box
	double _maxWeight;                          ///< \brief the maximal weight existing, used to create the Bounding box
	std::vector<RT_3::Vertex_handle> _boundSpa;	///< \brief the spatialables used as bounderies

	unsigned int _maxNumberOfFacetPerCell;			///< \brief The maximal number of facet a cell must contained
	double _deltaGain;													///< \brief The minimal value for which we continu to reffine

	std::map<const t_SpatialableAgent_3*, SpheroidalCell*> _constCellToSpheroidal;	///< \brief map from spatiable agent to Spheroidal Cell
};

#endif
