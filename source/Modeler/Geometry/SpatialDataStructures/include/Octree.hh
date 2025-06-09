#ifndef OCTREE_HH
#define OCTREE_HH

#include <vector>
#include <set>

#include <QString>

#include "InformationSystemManager.hh"
#include "AgentSettings.hh"

/// \brief a simple octree class to store spatialables.
/// \details Octree heritates froml OctreeNode because he is the first node of the tree
template<typename TNode>
class Octree {
public:
	Octree(BoundingBox<Point_3>, unsigned int pMaxNbAgt);
	Octree(BoundingBox<Point_3>, const std::vector<const Settings::nAgent::t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt);
	Octree(BoundingBox<Point_3>, const std::set<const Settings::nAgent::t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt);
	virtual ~Octree() = default;

	/// \brief return the nearest spatialable agent
	[[nodiscard]] virtual const Settings::nAgent::t_SpatialableAgent_3* getNearestSpatialableAgent(Point_3 pPt) const { return topNode.getNearestSpatialableAgent(pPt); }
	/// \brief clear the octree
	void clear()	{ topNode.clear();}
	/// \brief return all contained agent
	void getContainedAgents(std::set<const Settings::nAgent::t_SpatialableAgent_3*>& pStruc) const { return topNode.getContainedAgents(pStruc); }

protected:
	/// \brief will generate the octree construction
	template<typename TIt>
	void construct(TIt, TIt);
	/// \brief will be called juste after the octree construction
	void initialize();
	/// the first / main node of the octree. Containing all other node
	TNode topNode;
};

/// \param pDelimitation the spatial delimitation of the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
template <typename TNode>
Octree<TNode>::Octree(BoundingBox<Point_3> pBB, unsigned int pMaxNbAgt):
	topNode(
		pBB,
		0,
		NULL,
		pMaxNbAgt
	)
{
}

/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
template <typename TNode>
Octree<TNode>::Octree(BoundingBox<Point_3> pBB, const std::vector<const Settings::nAgent::t_SpatialableAgent_3*>* pSpatialables, unsigned int pMaxNbAgt):
	topNode(
		pBB,
		0,
		NULL,
		pMaxNbAgt
	)
{
	assert(pSpatialables);
	construct(pSpatialables->begin(), pSpatialables->end());
	initialize();
}

/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
template <typename TNode>
Octree<TNode>::Octree(BoundingBox<Point_3> pBB, const std::set<const Settings::nAgent::t_SpatialableAgent_3*>* pSpatialables, unsigned int pMaxNbAgt):
	topNode(
		pBB,
		0,
		NULL,
		pMaxNbAgt
	)
{
	assert(pSpatialables);
	construct(pSpatialables->begin(), pSpatialables->end());
	initialize();
}

template <typename TNode>
template<typename TIt>
void Octree<TNode>::construct(TIt begin, TIt end) {
	assert(begin != end);

	TIt itSpa;
	for(itSpa = begin; itSpa != end; ++itSpa) {
		if(!topNode.add(*itSpa)) {
			QString mess = "unable to add the spatialable : " + QString::number((*itSpa)->getID())
				+ " @ (" + QString::number( (*itSpa)->getPosition().x() ) + ", " + QString::number( (*itSpa)->getPosition().y() ) + ", " + QString::number( (*itSpa)->getPosition().z() ) + ")";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Octree:construtor");
		}
	}
}

template <typename TNode>
void Octree<TNode>::initialize() {
	// init the main node which will init children
	topNode.init();
}

#endif
