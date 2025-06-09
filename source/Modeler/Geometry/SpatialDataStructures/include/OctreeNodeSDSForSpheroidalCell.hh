#ifndef OCTREE_NODE_SDS_FOR_SPHEROIDAL_CELL_HH
#define OCTREE_NODE_SDS_FOR_SPHEROIDAL_CELL_HH

#include "OctreeNodeSDS.hh"
#include "Delaunay_3D_SDS.hh"

/// \brief define a specific behavior for spheroidal cells. Basically do the same as for spatialble agent but
/// take into account the cell radii. By creating a voronoi for each node
/// \note this structure is used to know spatialabla agent (cells) neighbors.
class OctreeNodeSDSForSpheroidalCell : public OctreeNodeSDS {
public:
	OctreeNodeSDSForSpheroidalCell(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNodeSDS* pParent, unsigned int pMaxNbAgt, double pExtensionLength=0.);
	~OctreeNodeSDSForSpheroidalCell() override;

	/// \brief init the node, once the construction of the complete tree is done
	void init() override;
	/// \brief return the neighbors in contact of the spatialableAgent
	std::set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3*) const override;

protected:
	OctreeNode* newChild(BoundingBox<Point_3>) override;

private:
	Delaunay_3D_SDS delaunay;
};

#endif
