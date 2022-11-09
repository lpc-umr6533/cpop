/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef OCTREE_NODE_HH
#define OCTREE_NODE_HH

#include "AgentSettings.hh"
#include "GeometrySettings.hh"
#include "Mesh3DSettings.hh"

#include <unordered_set>
#include <vector>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;
using namespace Settings::nAgent;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief define an octree node which is part of an octree, associate a list of spatialables
//////////////////////////////////////////////////////////////////////////////////////////////////
class OctreeNode
{
public:
	/// \brief constructor
	OctreeNode(BoundingBox<Point_3>, unsigned int pDeth, const OctreeNode* pParent, unsigned int pMaxNbAgt);
	/// \brief destructor
	virtual ~OctreeNode();
	/// \brief add a spatialable to this node
	bool add(const t_SpatialableAgent_3* pSpa);
	/// \brief add a spatialable to this node
	void remove(const t_SpatialableAgent_3* pSpa);
	/// \brief return the nearest spatialable agent
	virtual const t_SpatialableAgent_3* getNearestSpatialableAgent(Point_3) const;
	/// \brief used to know if this node has children or not 
	/// \return true if the node is a clossing node
	bool isClosingNode() const 			{ return children.size() == 0;} 
	/// \brief used to know if a point is included on the node
	bool contains(Point_3 pt) const;
	/// \brief init the node, once the construction of the complete tree is done
	virtual void init();
	/// \brief return the boundary of the node
	const BoundingBox<Point_3>* getBoundary() const { return &delimitation; }
	/// \brief clear the node and the children node
	void clear();
	/// \brief return contained agent on this node and the children nodes
	void getContainedAgents(set<const t_SpatialableAgent_3*>& pStruct) const;

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

	vector<OctreeNode*> children;	///< \brief the children node of this node. value is 8 or 0 if is a clossing/bottom node ( no children) 
	/// \brief all the agnt contained by this node.
	set<const t_SpatialableAgent_3*> contained;
	/// \brief the spatial delimitation of the node
	BoundingBox<Point_3> delimitation;
	unsigned int depth;				///< \brief depth of the node ( 0 if at top )
	const OctreeNode* parent;		///< \brief the parent node
	unsigned int maxNbAgtContained;	///< \brief the maximal number of agent the node can contain
};

#endif // OCTREE_NODE_HH
