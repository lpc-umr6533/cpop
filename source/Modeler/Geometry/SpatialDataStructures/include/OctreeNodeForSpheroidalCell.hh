/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef OCTREE_NODE_FOR_SPHEROIDAL_CELL_HH
#define OCTREE_NODE_FOR_SPHEROIDAL_CELL_HH

#include "OctreeNode.hh"

#include "Delaunay_3D_SDS.hh"

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief define a specific behavior for spheroidal cells. Basically do the same as for spatialble agent but 
/// take into account the cell radii. By creating a voronoi for each node
/// \note this structure is used to know in which agent a point is 
//////////////////////////////////////////////////////////////////////////////////////////////////
class OctreeNodeForSpheroidalCell : public OctreeNode
{
public:
	/// \brief constructor
	OctreeNodeForSpheroidalCell(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNode* pParent, unsigned int pMaxNbAgt);
	/// \brief destructor
	virtual ~OctreeNodeForSpheroidalCell();

	/// \brief init the node, once the construction of the complete tree is done
	virtual void init();	
	/// \brief return the nearest agent of the given point. Taking into account cell radii
	const t_SpatialableAgent_3* getNearestSpatialableAgent(Point_3 pPt) const;

protected:
	virtual OctreeNode* newChild(BoundingBox<Point_3>);

private:
	Delaunay_3D_SDS delaunay;
};

#endif // OCTREE_NODE_FOR_SPHEROIDAL_CELL_HH