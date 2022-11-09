/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef VORONOI_3D_MESH_HH
#define VORONOI_3D_MESH_HH

#include "AgentSettings.hh"
#include "CellSettings.hh"
#include "CGAL_Utils.hh"
#include "Mesh.hh"
#include "Mesh3DSettings.hh"
#include "SpheroidalCell.hh"
#include "Delaunay_3D_SDS.hh"

#include <set>

//// CGAL import
// includes for defining the Voronoi diagram adaptor
#include <CGAL/basic.h>
#include <CGAL/bounding_box.h>

// export file
#include <fstream>

using namespace Settings::nCell;
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

//////////////////////////////////////////////////////////////////////////////
/// \brief Define a 2D_Voronoi mesh
/// \warning this class is able to deal with weighted points.
/// If none specified will create homogeneous weight
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Voronoi_3D_Mesh : public Mesh<double, Point_3, Vector_3>, public Delaunay_3D_SDS
{
	/// Victor Levrague : creation of an IDCell.txt file with cells deleted in the conversion from .xml in G4 geometry ///

	using Delaunay_3D_SDS::add;			// overloaded function
	using Delaunay_3D_SDS::remove;		// overloaded function

public:
	/// \brief constructor
	Voronoi_3D_Mesh(unsigned int pMaxNbFacet, double delta, std::set<t_Cell_3*> pInitSpatialables = std::set<t_Cell_3*>());
	/// \brief destructor
	virtual ~Voronoi_3D_Mesh();

	/// \brief add a point to the mesh
	virtual bool add(t_Cell_3*);
	/// \brief remove a point to the mesh
	virtual void remove(t_Cell_3*);
	/// \brief the mesh exporter
	virtual int exportToFile(QString, MeshOutFormats::outputFormat, bool);
	/// \brief the bounding box getter.
	K::Iso_cuboid_3 getBoundingBox() const;

	/// \brief max number of segment per cell setter
	void setNBMaxFacetPerCell(unsigned int pNb)	{maxNumberOfFacetPerCell = pNb;};
	/// \brief max number of segment per cell getter
	unsigned int getMaxNbFacetPerCell()			{return maxNumberOfFacetPerCell;};

	/// \brief threshold refinnement setter
	void setDeltaGain(double pGain)				{deltaGain = pGain;};
	/// \brief threshold refinnement setter
	double getDeltaWin()							{return deltaGain;};
	/// \brief update cell shapes according to other cell contained on the mesh
	virtual std::vector<SpheroidalCell*> generateMesh();
	/// \brief check if the mesh is valid <=> no mesh recovery
	bool isValid();
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_3*> getCells() const;
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_3*> getCellsWithShape() const;
protected:
	/// \brief export to an off file
	virtual int exportToFileOff(QString pPath, std::vector<SpheroidalCell*> cells, bool pDivided);
	/// \brief export all cells on the same file
	virtual int exportToFileOff_undivided(QString pPath, std::vector<SpheroidalCell*>* );
	/// \brief export all cells on the same file
	virtual int exportToFileOff_divided(QString pPath, std::vector<SpheroidalCell*>* );
	/// \brief get all polygons from the delaunay mesh.
	std::vector<SpheroidalCell*> getCellsStructure();
	/// \brief define all the neighbouring cell for each cell
	void generateNeighbourhood();
	/// \brief clean data structures
	virtual void clean();

protected:
	/// \brief remove all point on the mesh with are "include" on the area of influence of an other point.
	virtual void removeConflicts();

protected:
	std::map<Point_3, double> weights;										///< \brief the map of weights
	std::map<SpheroidalCell*, std::set<const SpheroidalCell*> > neighboursCell;	///< \brief for each cell the neighbourhood

private:
	double minWeight;														///< \brief the minimal weight existing, used to create the Bounding box
	double maxWeight;														///< \brief the maximal weight existing, used to create the Bounding box
	std::vector<RT_3::Vertex_handle> boundSpa;								///< \brief the spatialables used as bounderies

	unsigned int maxNumberOfFacetPerCell;									///< \brief The maximal number of facet a cell must contained
	double deltaGain;														///< \brief The minimal value for which we continu to reffine

	std::map<const t_SpatialableAgent_3*, SpheroidalCell*> mConstCellToSpheroidal;	///< \brief map from spatiable agent to Spheroidal Cell
};

#endif
