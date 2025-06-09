#ifndef OCTREE_NODE_FOR_SPHEROIDAL_CELL_HH
#define OCTREE_NODE_FOR_SPHEROIDAL_CELL_HH

#include "OctreeNode.hh"

#include "Delaunay_3D_SDS.hh"

/// \brief define a specific behavior for spheroidal cells. Basically do the same as for spatialble agent but
/// take into account the cell radii. By creating a voronoi for each node
/// \note this structure is used to know in which agent a point is
class OctreeNodeForSpheroidalCell : public OctreeNode {
public:
	OctreeNodeForSpheroidalCell(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNode* pParent, unsigned int pMaxNbAgt);
	~OctreeNodeForSpheroidalCell() override;

	/// \brief init the node, once the construction of the complete tree is done
	void init() override;
	/// \brief return the nearest agent of the given point. Taking into account cell radii
	const t_SpatialableAgent_3* getNearestSpatialableAgent(Point_3 pPt) const override;

protected:
	OctreeNode* newChild(BoundingBox<Point_3>) override;

private:
	Delaunay_3D_SDS delaunay;
};

#endif
