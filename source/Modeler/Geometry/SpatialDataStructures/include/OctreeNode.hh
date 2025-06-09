#ifndef OCTREE_NODE_HH
#define OCTREE_NODE_HH

#include "AgentSettings.hh"
#include "GeometrySettings.hh"
#include <vector>

using namespace Settings::Geometry;
using namespace Settings::nAgent;

/// \brief define an octree node which is part of an octree, associate a list of spatialables
class OctreeNode {
public:
	OctreeNode(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNode* pParent, unsigned int pMaxNbAgt);
	virtual ~OctreeNode() = default;

	/// \brief add a spatialable to this node
	bool add(const t_SpatialableAgent_3* pSpa);
	/// \brief add a spatialable to this node
	void remove(const t_SpatialableAgent_3* pSpa);
	/// \brief return the nearest spatialable agent
	[[nodiscard]] virtual const t_SpatialableAgent_3* getNearestSpatialableAgent(Point_3) const;
	/// \brief used to know if this node has children or not
	/// \return true if the node is a clossing node
	[[nodiscard]] bool isClosingNode() const { return children.size() == 0; }
	/// \brief used to know if a point is included on the node
	[[nodiscard]] bool contains(Point_3 pt) const;
	/// \brief init the node, once the construction of the complete tree is done
	virtual void init();
	/// \brief return the boundary of the node
	[[nodiscard]] const BoundingBox<Point_3>* getBoundary() const { return &delimitation; }
	/// \brief clear the node and the children node
	void clear();
	/// \brief return contained agent on this node and the children nodes
	void getContainedAgents(std::set<const t_SpatialableAgent_3*>& pStruct) const;

protected:
	/// \brief return true if the spatialable agent should be add on the node
	virtual bool shouldBeAdd(const t_SpatialableAgent_3*) const;
	/// \brief function used to subdived a node
	virtual void subdivide();
	/// \brief mothe spatialable to the children nodes
	void moveToChildrens(const t_SpatialableAgent_3*);
	/// \brief return a new child with the given bounding box
	virtual OctreeNode* newChild(BoundingBox<Point_3>);

protected:

	std::vector<OctreeNode*> children;	///< \brief the children node of this node. value is 8 or 0 if is a clossing/bottom node ( no children)
	/// \brief all the agnt contained by this node.
	std::set<const t_SpatialableAgent_3*> contained;
	/// \brief the spatial delimitation of the node
	BoundingBox<Point_3> delimitation;
	unsigned int depth;				///< \brief depth of the node ( 0 if at top )
	const OctreeNode* parent;		///< \brief the parent node
	unsigned int maxNbAgtContained;	///< \brief the maximal number of agent the node can contain
};

#endif // OCTREE_NODE_HH
