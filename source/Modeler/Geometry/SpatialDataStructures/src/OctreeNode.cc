#include "OctreeNode.hh"

static const unsigned int OCTREE_NODE_DEPTH_MAX = 40;

/// \param pDelimitation the spatial delimitation (bounding box) of the octree
/// \param pDeth the depth of the node
/// \param pParent the parent node. Null if is the top node
/// \ûparam pMaxNbAgent the maximal number of agent the node can include
OctreeNode::OctreeNode(BoundingBox<Point_3> pDelimitation, unsigned int pDeth, const OctreeNode* pParent, unsigned int pMaxNbAgt):
	delimitation(pDelimitation),
	depth(pDeth),
	parent(pParent),
	maxNbAgtContained(pMaxNbAgt)
{
	assert(pDelimitation.getBottomLeft() != pDelimitation.getTopRight());
}

bool OctreeNode::shouldBeAdd(const t_SpatialableAgent_3* pSpa) const {
	return (pSpa->isIn(&delimitation) || pSpa->cross(&delimitation));
}

/// \param pSpa the agent to add to the node
/// \return true if add is a sucess
bool OctreeNode::add(const t_SpatialableAgent_3* pSpa) {
	// if the agent is not part of the bounding box, refuse add
	if(!shouldBeAdd(pSpa))
		return false;

	// if is a closing node : dela with the agent
	if(isClosingNode()) {
		contained.insert(pSpa);
		// if the node became to heavy : subdivise her
		if((depth < OCTREE_NODE_DEPTH_MAX) && (contained.size() > maxNbAgtContained))
			subdivide();

		return true;
	} else {
		// else let children node deal with it
		bool sucess = false;
		for(auto const& itChildNode : children)
			sucess += itChildNode->add(pSpa);

		return sucess;
	}
}

void OctreeNode::getContainedAgents(std::set<const t_SpatialableAgent_3*>& pStruct) const {
	// if is a closing node : dela with the agent
	if(isClosingNode()) {
		pStruct.insert(contained.begin(), contained.end());
	} else {
		for(auto const& itChildNode : children)
			itChildNode->getContainedAgents(pStruct);
	}
}

/// \param pSpa the agent to remove from the node
void OctreeNode::remove(const t_SpatialableAgent_3* pSpa) {
	// if is a closing node : dela with the agent
	if(isClosingNode()) {
		contained.erase(pSpa);
	} else {
		for(auto const& itChildNode : children)
			itChildNode->remove(pSpa);
	}
}

OctreeNode* OctreeNode::newChild(BoundingBox<Point_3> bb) {
	return new OctreeNode(
		bb, 					//< spatial delimitation
		depth+1, 				//< depth of the children
		this, 					//< parent of the new node
		maxNbAgtContained	//< maximal number of agent
	);
}

/// \details there is two policy of subdivision for object contained one many node.
/// First one is keep the object on the parent node. Second one is duplicate this object
/// on children node. We will go for the second policy. Because for the moment we want
/// to create a dedicated spatial data structure ( Delaunay ) for each node. And the second option
/// will be simpler to speed up this process.
void OctreeNode::subdivide() {
	assert(children.size() == 0);
	Point_3 max = delimitation.getTopRight();
	Point_3 min = delimitation.getBottomLeft();

	double xMed = (max.x() + min.x())/2.;
	double yMed = (max.y() + min.y())/2.;
	double zMed = (max.z() + min.z())/2.;

	Point_3 barycenter(xMed, yMed, zMed);

	/// bottom node	( on y up landmark)
	children.push_back(newChild(
		BoundingBox<Point_3>(
			Point_3(min.x(), min.y(), min.z()), Point_3(xMed, yMed, zMed) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(xMed, min.y(), min.z()), Point_3(max.x(), yMed, zMed) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(min.x(), min.y(), zMed), Point_3(xMed, yMed, max.z()) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(xMed, min.y(), zMed), Point_3(max.x(), yMed, max.z()) )
		)
	);

	/// up node ( on y up landmark)
	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(min.x(), yMed, min.z()), Point_3(xMed, max.y(), zMed) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(xMed, yMed, min.z()), Point_3(max.x(), max.y(), zMed) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(min.x(), yMed, zMed), Point_3(xMed, max.y(), max.z()) )
		)
	);

	children.push_back( newChild(
		BoundingBox<Point_3>(
			Point_3(xMed, yMed, zMed), Point_3(max.x(), max.y(), max.z()) )
		)
	);

	// move all points to the children
	for(auto const& itSpa : contained)
		moveToChildrens(itSpa);

	contained.clear();
}

/// \param pSpa the agent to move to children node
void OctreeNode::moveToChildrens(const t_SpatialableAgent_3* pSpa) {
	assert(children.size() == 8);
	for(auto const& itChildNode : children) {
		// if not on the node will simply refuse the add
		itChildNode->add(pSpa);
	}
}

/// \param pPoint the point for which we want to have the nearest spatialable
/// \return the nearest agent to the given point. Null if none found
const t_SpatialableAgent_3* OctreeNode::getNearestSpatialableAgent(Point_3 pPoint) const {
	// if we are on a closing node use the integrated delaunay.
	if(isClosingNode()) {
		double minDist = std::numeric_limits<double>::max();
		const t_SpatialableAgent_3* nearest = nullptr;
		for(auto const& itSpa : contained) {
			double dist = CGAL::squared_distance(pPoint, itSpa->getPosition());
			if(dist < minDist) {
				minDist = dist;
				nearest = itSpa;
			}
		}
		return nearest;
	} else {
		// else find on which children the point is part of
		for(auto const& itChildNode : children) {
			if(itChildNode->contains(pPoint))
				return itChildNode->getNearestSpatialableAgent(pPoint);
		}
	}

	// if none found
	return nullptr;
}

void OctreeNode::init() {
	// init children
	for(auto& itChildNode : children)
		itChildNode->init();
}

/// \param pt the point we want to know if is inside this node
/// \return true if the point is inside the node
bool OctreeNode::contains(Point_3 pt) const {
	Point_3 max = delimitation.getTopRight();
	Point_3 min = delimitation.getBottomLeft();
	return (
		(pt.x() <= max.x()) && (pt.x() >= min.x()) &&
		(pt.y() <= max.y()) && (pt.y() >= min.y()) &&
		(pt.z() <= max.z()) && (pt.z() >= min.z())
	);
}

void OctreeNode::clear() {
	if(isClosingNode()) {
		contained.clear();
	} else {
		// clear and remove children
		for(auto& itChildNode : children) {
			itChildNode->clear();
			delete itChildNode;
		}

		children.clear();
	}
}
