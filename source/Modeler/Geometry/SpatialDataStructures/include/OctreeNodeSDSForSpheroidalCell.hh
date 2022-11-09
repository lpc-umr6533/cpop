/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef OCTREE_NODE_SDS_FOR_SPHEROIDAL_CELL_HH
#define OCTREE_NODE_SDS_FOR_SPHEROIDAL_CELL_HH

#include "OctreeNodeSDS.hh"
#include "Delaunay_3D_SDS.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief define a specific behavior for spheroidal cells. Basically do the same as for spatialble agent but 
/// take into account the cell radii. By creating a voronoi for each node
/// \note this structure is used to know spatialabla agent (cells) neighbors.
//////////////////////////////////////////////////////////////////////////////////////////////////
class OctreeNodeSDSForSpheroidalCell : public OctreeNodeSDS
{
public:
	/// \brief constructor
	OctreeNodeSDSForSpheroidalCell(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNodeSDS* pParent, unsigned int pMaxNbAgt, double pExtensionLength=0.);
	/// \brief destructor
	virtual ~OctreeNodeSDSForSpheroidalCell();

	/// \brief init the node, once the construction of the complete tree is done
	virtual void init();	
	/// \brief return the neighbors in contact of the spatialableAgent
	set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3*) const;

protected:
	virtual OctreeNode* newChild(BoundingBox<Point_3>);
	
private:
	Delaunay_3D_SDS delaunay;
};

#endif // OCTREE_NODE_SDS_FOR_SPHEROIDAL_CELL_HH