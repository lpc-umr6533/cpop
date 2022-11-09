/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "OctreeNodeSDSForSpheroidalCell.hh"
#include "SpheroidalCell.hh"

/////////////////////////////////////////////////////////////////////////////////////////////
/// \param pDelimitation the spatial delimitation of the node
/// \param pDepth the depth of the node
/// \param pParent the parent node
/// \param pMaxNbAgent the maximal number of parent of this node ( and children node )
/////////////////////////////////////////////////////////////////////////////////////////////
OctreeNodeSDSForSpheroidalCell::OctreeNodeSDSForSpheroidalCell(
	BoundingBox<Point_3> pDelimitation, 
	unsigned int pDeth, 
	const OctreeNodeSDS* pParent, 
	unsigned int pMaxNbAgt,
	double pExtensionLength):
	OctreeNodeSDS(pDelimitation, pDeth, pParent, pMaxNbAgt, pExtensionLength),
	delaunay("delaunayForSpheroidalCellOctreeSDS")
{

}

/////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////
OctreeNodeSDSForSpheroidalCell::~OctreeNodeSDSForSpheroidalCell()
{
	delaunay.clean();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
OctreeNode* OctreeNodeSDSForSpheroidalCell::newChild(BoundingBox<Point_3> bb)
{
	return new OctreeNodeSDSForSpheroidalCell( 
		bb, 					//< spatial delimitation
		depth+1, 				//< depth of the children
		this, 					//< parent of the new node
		maxNbAgtContained );	//< maximal number of agent 
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// create the delaunay to next access to the nearest neighbors ...
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OctreeNodeSDSForSpheroidalCell::init()
{
	OctreeNode::init();
	// once all spatialables have been added, we can create the delaunay
	set<const t_SpatialableAgent_3*>::iterator itSpa;
	for(itSpa = contained.begin(); itSpa != contained.end(); ++itSpa)
	{
		delaunay.add(*itSpa);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pSpa the spatialable for which we want the neighbours
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
set<const t_SpatialableAgent_3*> OctreeNodeSDSForSpheroidalCell::getNeighbours(const t_SpatialableAgent_3* pSpa) const
{
	// if we are on a closing node use the integrated delaunay.
	if(isClosingNode())
	{
		set<const t_SpatialableAgent_3*> neighbors = delaunay.getNeighbours(pSpa);
		set<const t_SpatialableAgent_3*> result;
		const SpheroidalCell* lCell = dynamic_cast<const SpheroidalCell*>(pSpa);
		if(lCell)
		{
			// remove neighbors which arn't intersecting the spa agt
			for(set<const t_SpatialableAgent_3*>::iterator itNeighbor = neighbors.begin(); itNeighbor != neighbors.end(); ++itNeighbor)
			{
				const SpheroidalCell* lCell2 = dynamic_cast<const SpheroidalCell*>(*itNeighbor);
				if(lCell2)
				{
					if(CGAL::squared_distance(lCell2->getPosition(), lCell->getPosition()) <= std::max(lCell->getSquareRadius(), lCell2->getSquareRadius()))
					{
						result.insert(*itNeighbor);
					}
				}
			}
		}
		return result;

	}else
	{
		// else find on which children the point is part of
		vector<OctreeNode*>::const_iterator itChildNode;
		for(itChildNode = children.begin(); itChildNode != children.end(); ++itChildNode)
		{
			// heaven if the agent is duplicated himself and his neighbors are only stored in one node
			// the one he is contained in.
			if((*itChildNode)->contains(pSpa->getPosition()))
			{
				assert(dynamic_cast<OctreeNodeSDS*>(*itChildNode));
				return (dynamic_cast<OctreeNodeSDS*>(*itChildNode))->getNeighbours(pSpa);
			}
		}
	}
	return set<const t_SpatialableAgent_3*>();
}
