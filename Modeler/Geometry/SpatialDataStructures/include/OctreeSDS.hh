/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef OCTREE_SDS_HH
#define OCTREE_SDS_HH

#include <AgentSettings.hh>
#include <SpatialDataStructure.hh>
#include <SpheroidalCell.hh>
#include <Octree.hh>

using namespace Settings::nAgent;

//////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief take the same concept as the Octree but extends his behavior to the SDS behavior.
/// We add a new condition for Nodes in order to make sure each touching neighboring cells is included in the same node 
/// ( so make sure each neighbors are registred)
///
/// in order to complete this we have to define a larger outlying area of inclusion (Node extension). And add the cells which can be crossing this area.
/// This larger if based on the largest spheroidalCellRadius contained by the node. 
/// But to speed up this process we will use the same radius for all node. Oterwise we might be forced to process way many time the association process. so : 
/// \warning if we add a new Cell with an higher radius of all previous cells, we have to recompute the entire tree. So the add function must be used carrefully and can be time consuming.
/// \details Octree heritates froml OctreeNode because he is the first node of the tree
//////////////////////////////////////////////////////////////////////////////////////////////////
template<typename TNodeSDS>
class OctreeSDS : public SpatialDataStructure<double, Point_3, Vector_3>, public Octree<TNodeSDS>
{
public:
	/// \brief constructor
	OctreeSDS(QString, const vector<const t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt );
	/// \brief constructor
	OctreeSDS(QString, const set<const t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt );
	/// \brief destructor
	virtual ~OctreeSDS();
	
	/// \brief add a spatialable to this node
	virtual bool add(const t_SpatialableAgent_3* pSpa);

	/// \brief function called when need an update of the structure
	virtual int update();
	/// \brief function to call to add a spatialable entity
	virtual bool update(const t_SpatialableAgent_3*);
	/// \brief return the list of neighbours. Here the node is not only a .
	virtual set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3* pSpa) const { return Octree<TNodeSDS>::topNode.getNeighbours(pSpa);}

protected:
	/// \brief the extension length on which we should add the cell crossing
	double extensionLength;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNodeSDS>
OctreeSDS<TNodeSDS>::OctreeSDS(
	QString pName,
	const vector<const t_SpatialableAgent_3*>* pSpatialables, 
	unsigned int pMaxNbAgt):
	SpatialDataStructure<double, Point_3, Vector_3>(pName),
	Octree<TNodeSDS>(pMaxNbAgt)
{
	extensionLength = 0;
	vector<const t_SpatialableAgent_3*>::const_iterator itSpa;
	for(itSpa = pSpatialables->begin(); itSpa != pSpatialables->end(); ++itSpa)
	{
		SpheroidalCell* lCell = dynamic_cast<SpheroidalCell*>(*itSpa);
		if(lCell)
		{
			extensionLength = max(extensionLength, lCell->getRadius());
		}
	}
	Octree<TNodeSDS>::topNode.setExtensionLength(extensionLength);

	Octree<TNodeSDS>::construct(pSpatialables->begin(), pSpatialables->end());
	Octree<TNodeSDS>::initialize();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNodeSDS>
OctreeSDS<TNodeSDS>::OctreeSDS(
	QString pName,
	const set<const t_SpatialableAgent_3*>* pSpatialables, 
	unsigned int pMaxNbAgt):
	SpatialDataStructure<double, Point_3, Vector_3>(pName),
	Octree<TNodeSDS>(pMaxNbAgt)
{
	extensionLength = 0;
	set<const t_SpatialableAgent_3*>::const_iterator itSpa;
	for(itSpa = pSpatialables->begin(); itSpa != pSpatialables->end(); ++itSpa)
	{
		const SpheroidalCell* lCell = dynamic_cast<const SpheroidalCell*>(*itSpa);
		if(lCell)
		{
			extensionLength = max(extensionLength, lCell->getRadius());
		}
	}
	Octree<TNodeSDS>::topNode.setExtensionLength(extensionLength);

	Octree<TNodeSDS>::construct(pSpatialables->begin(), pSpatialables->end());
	Octree<TNodeSDS>::initialize();	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNodeSDS>
bool OctreeSDS<TNodeSDS>::add(const t_SpatialableAgent_3* pSpa)
{
	const SpheroidalCell* lCell = dynamic_cast<const SpheroidalCell*>(pSpa);
	if(lCell)
	{
		double radSpa = lCell->getRadius();
		if(radSpa > extensionLength)
		{
			// update higher extension length
			extensionLength = radSpa;
			// update ocTree with this new value
			update();
			// redo the add
			return add(pSpa);
		}
	}
	// otherwise let the Octree deal with the add
	return Octree<TNodeSDS>::topNode.add(pSpa);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNodeSDS>
int OctreeSDS<TNodeSDS>::update()
{
	set<const t_SpatialableAgent*> containedSpas;
	Octree<TNodeSDS>::getContainedAgents(containedSpas);
	Octree<TNodeSDS>::topNode.clear();
	set<const t_SpatialableAgent*>::iterator itSpa;

	// TODO : recompute bounding box to make sur this is ok.
	Octree<TNodeSDS>::construct(containedSpas.begin(), containedSpas.end());
	Octree<TNodeSDS>::initialize();
	containedSpas.clear();
	Octree<TNodeSDS>::getContainedAgents(containedSpas);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename TNodeSDS>
bool OctreeSDS<TNodeSDS>::update(const t_SpatialableAgent* pSpa)
{
	assert(pSpa);
	Octree<TNodeSDS>::topNode.remove(pSpa);
	return Octree<TNodeSDS>::topNode.add(pSpa);
}

#endif // OCTREE_SDS_HH