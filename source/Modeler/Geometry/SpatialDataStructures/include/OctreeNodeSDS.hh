#ifndef OCTREE_NODE_SDS_HH
#define OCTREE_NODE_SDS_HH

#include "OctreeNode.hh"

using namespace Settings::Geometry;
using namespace Settings::nAgent;

/// \brief define an octree node used for the usage of an Spatial Data Structure
/// We add a new condition for Nodes in order to make sure each touching neighboring cells is included in the same node
/// ( so make sure each neighbors are registred)
///
/// in order to complete this we have to define a larger outlying area of inclusion (Node extension). And add the cells which can be crossing this area.
/// This larger if based on the largest spheroidalCellRadius contained by the node.
/// But to speed up this process we will use the same radius for all node. Oterwise we might be forced to process way many time the association process. so :
/// \warning if we add a new Cell with an higher radius of all previous cells, we have to recompute the entire tree. So the add function must be used carrefully and can be time consuming.
/// \details Octree heritates froml OctreeNode because he is the first node of the tree
class OctreeNodeSDS : public OctreeNode {
public:
	OctreeNodeSDS(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNodeSDS* pParent, unsigned int pMaxNbAgt, double pExtensionLength =0.);

	/// \brief return the neighbors in contact of the spatialableAgent
	virtual std::set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3*) const = 0;
	/// \brief set the extension length
	void setExtensionLength(double pLength)	{ assert(pLength >= 0.); _extensionLength = pLength; }
	[[nodiscard]] double getExtensionLength() const { return _extensionLength; }

protected:
	/// \brief return true if the spatialable agent should be add on the node
	bool shouldBeAdd(const t_SpatialableAgent_3*) const override;

protected:
	/// \brief the extension length on which we should add the cell crossing
	double _extensionLength;
	/// \brief the extended delimitation we must check to
	BoundingBox<Point_3> delimitationExtended;
};

#endif
