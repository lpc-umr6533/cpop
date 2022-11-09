/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef VORONOI_2D_MESH_HH
#define VORONOI_2D_MESH_HH

#include "Mesh.hh"

#include <set>
#include "Delaunay_2D_SDS.hh"
#include "DiscoidalCell.hh"
#include "Mesh2DSettings.hh"
#include "CellSettings.hh"

using namespace Settings::nCell;
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
using namespace Settings::nAgent;

//////////////////////////////////////////////////////////////////////////////
/// \brief Define a 2D_Voronoi mesh
/// \warning this class is able to deal with weighted points. 
/// If none specified will create homogeneous weight
/// @author Henri Payno
///
//////////////////////////////////////////////////////////////////////////////
class Voronoi_2D_Mesh : public Mesh<double, Point_2, Vector_2>, public Delaunay_2D_SDS
{
	using Delaunay_2D_SDS::add;
	using Delaunay_2D_SDS::remove;

public:
	/// \brief constructor
	Voronoi_2D_Mesh(unsigned int, double, std::set<t_Cell_2*> pInitSpatialables = std::set<t_Cell_2*>());
	/// \brief destructor
	virtual ~Voronoi_2D_Mesh();

	/// \brief add a point to the mesh
	virtual bool add(t_Cell_2*);
	/// \brief remove a point to the mesh
	virtual void remove(t_Cell_2*);
	/// \brief the mesh exporter
	virtual int exportToFile(QString, MeshOutFormats::outputFormat, bool);
	/// \brief return the bounding box of the mesh.
	Iso_rectangle_2 getBoundingBox() const;

	/// \brief max number of segment per cell setter
	void setNBMaxSegPerCell(unsigned int pNb)	{maxNumberOfSegPerCell = pNb;};
	/// \brief max number of segment per cell getter
	unsigned int getMaxNbSegPerCell()			{return maxNumberOfSegPerCell;};

	/// \brief threshold refinnement setter
	void setDeltaGain(double pGain)				{deltaGain = pGain;};
	/// \brief threshold refinnement setter
	double getDeltaWin()							{return deltaGain;};
	/// \brief get all polygons from the delaunay mesh.
	virtual std::vector<DiscoidalCell*> generateMesh();
	/// \brief check if the mesh is valid <=> no mesh recovery
	bool isValid();
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_2*> getCells() const;
	/// \brief return the cell contained on the mesh
	std::set<t_Cell_2*> getCellsWithShape() const;
protected:
	/// \brief remove node in conflict
	virtual void removeConflicts();
	/// \brief export to an off file
	int exportToFileOff_divided(QString pPath, std::vector<DiscoidalCell*>* cells);
	/// \brief export directly all discoidal cell given in paramater
	virtual int exportToFileOff_undivided(QString pPath, std::vector<DiscoidalCell*>* cells);
	/// \brief exportter to .OFF file format
	virtual int exportToFileOff(QString pPath, bool pDivided);
	/// \brief clean data structures
	virtual void clean();
	/// \brief get all polygons from the delaunay mesh.
	std::vector<DiscoidalCell*> getCellsStructure();
	/// \brief create the neighboursCells map.
	void generateNeighbourhood();

protected:
	std::map<Point_2, double> weights;			///< \brief weights of the each point included on the mesh
	/// \brief the neighbourhood construction
	std::map<DiscoidalCell*, std::set<const DiscoidalCell*> > neighboursCell;
	
private:
	double minWeight;							///< \brief the minimal weight included on the mesh.
	std::vector<RT_2::Vertex_handle> boundSpa;	///< \brief the vector of spatialables used to spatially limit the tesselation.

	unsigned int maxNumberOfSegPerCell;			///< \brief The maximal number of segment a cell must contained
	double deltaGain;							///< \brief The minimal value for which we continu to reffine
	
	std::map<const t_SpatialableAgent_2*, DiscoidalCell*> mConstCellToDiscoidal;	///< \brief map from T_Spatialable_2 to DiscoidalCell

};

#endif