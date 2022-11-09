#include "OctreeNodeForSpheroidalCell.hh"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \param pDelimitation the spatial delimitation of the node
/// \param pDepth the depth of the node
/// \param pParent the parent node
/// \param pMaxNbAgent the maximal number of parent of this node ( and children node )
/////////////////////////////////////////////////////////////////////////////////////////////
OctreeNodeForSpheroidalCell::OctreeNodeForSpheroidalCell(
	BoundingBox<Point_3> pDelimitation, 
	unsigned int pDeth, 
	const OctreeNode* pParent, 
	unsigned int pMaxNbAgt):
	OctreeNode(pDelimitation, pDeth, pParent, pMaxNbAgt),
	delaunay("delaunayForSpheroidalCellOctree")
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////
OctreeNodeForSpheroidalCell::~OctreeNodeForSpheroidalCell()
{
	delaunay.clean();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OctreeNode* OctreeNodeForSpheroidalCell::newChild(BoundingBox<Point_3> bb)
{
	return new OctreeNode( 
		bb, 					//< spatial delimitation
		depth+1, 				//< depth of the children
		this, 					//< parent of the new node
		maxNbAgtContained );	//< maximal number of agent 
}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////
const t_SpatialableAgent_3* OctreeNodeForSpheroidalCell::getNearestSpatialableAgent(Point_3 pPoint) const
{
	// if we are on a closing node use the integrated delaunay.
	if(isClosingNode())
	{
		// do the same as OctreeNode but based on a delaunay ( CGAL quick distance point verification)
		return delaunay.getNearestAgent(pPoint);
	}else
	{
		// else find on which children the point is part of
		vector<OctreeNode*>::const_iterator itChildNode;
		for(itChildNode = children.begin(); itChildNode != children.end(); ++itChildNode)
		{
			if((*itChildNode)->contains(pPoint))
			{
				return (*itChildNode)->getNearestSpatialableAgent(pPoint);
			}
		}
	}

	// if none found
	return NULL;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OctreeNodeForSpheroidalCell::init()
{
	OctreeNode::init();
	// once all spatialables have been added, we can create the delaunay
	set<const t_SpatialableAgent_3*>::iterator itSpa;
	for(itSpa = contained.begin(); itSpa != contained.end(); ++itSpa)
	{
		delaunay.add(*itSpa);
	}
}
