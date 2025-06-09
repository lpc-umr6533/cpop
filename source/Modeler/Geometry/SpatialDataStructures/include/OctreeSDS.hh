#ifndef OCTREE_SDS_HH
#define OCTREE_SDS_HH

#include <AgentSettings.hh>
#include <SpatialDataStructure.hh>
#include <SpheroidalCell.hh>
#include <Octree.hh>

using namespace Settings::nAgent;

/// \brief take the same concept as the Octree but extends his behavior to the SDS behavior.
/// We add a new condition for Nodes in order to make sure each touching neighboring cells is included in the same node
/// (so make sure each neighbors are registred)
///
/// in order to complete this we have to define a larger outlying area of inclusion (Node extension). And add the cells which can be crossing this area.
/// This larger if based on the largest spheroidalCellRadius contained by the node.
/// But to speed up this process we will use the same radius for all node. Oterwise we might be forced to process way many time the association process. so :
/// \warning if we add a new Cell with an higher radius of all previous cells, we have to recompute the entire tree. So the add function must be used carrefully and can be time consuming.
/// \details Octree heritates froml OctreeNode because he is the first node of the tree
template<typename TNodeSDS>
class OctreeSDS : public SpatialDataStructure<double, Point_3, Vector_3>, public Octree<TNodeSDS> {
public:
	OctreeSDS(QString, const std::vector<const t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt);
	OctreeSDS(QString, const std::set<const t_SpatialableAgent_3*>*, unsigned int pMaxNbAgt);
	~OctreeSDS() override;

	/// \brief add a spatialable to this node
	bool add(const t_SpatialableAgent_3* pSpa) override;

	/// \brief function called when need an update of the structure
	int update() override;
	/// \brief function to call to add a spatialable entity
	bool update(const t_SpatialableAgent_3*) override;
	/// \brief return the list of neighbours. Here the node is not only a .
	std::set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3* pSpa) const override {
		return Octree<TNodeSDS>::topNode.getNeighbours(pSpa);
	}

protected:
	/// \brief the extension length on which we should add the cell crossing
	double _extensionLength;
};

/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
template <typename TNodeSDS>
OctreeSDS<TNodeSDS>::OctreeSDS(
	QString pName,
	const std::vector<const t_SpatialableAgent_3*>* pSpatialables,
	unsigned int pMaxNbAgt):
	SpatialDataStructure<double, Point_3, Vector_3>(pName),
	Octree<TNodeSDS>(pMaxNbAgt)
{
	_extensionLength = 0;
	for(auto const& pSpatialable : *pSpatialables) {
		auto const* lCell = dynamic_cast<SpheroidalCell const*>(pSpatialable);
		if(lCell)
			_extensionLength = std::max(_extensionLength, lCell->getRadius());
	}

	Octree<TNodeSDS>::topNode.setExtensionLength(_extensionLength);

	Octree<TNodeSDS>::construct(pSpatialables->begin(), pSpatialables->end());
	Octree<TNodeSDS>::initialize();
}

/// \param pDelimitation the spatial delimitation of the octree
/// \param pSpatialables the spatialables agents contained on the octree
/// \param pMaxNbAgent the maximal number of agent contained on a node
template <typename TNodeSDS>
OctreeSDS<TNodeSDS>::OctreeSDS(
	QString pName,
	const std::set<const t_SpatialableAgent_3*>* pSpatialables,
	unsigned int pMaxNbAgt):
	SpatialDataStructure<double, Point_3, Vector_3>(pName),
	Octree<TNodeSDS>(pMaxNbAgt)
{
	_extensionLength = 0;
	for(auto const& pSpatialable : *pSpatialables) {
		const auto* lCell = dynamic_cast<const SpheroidalCell*>(pSpatialable);
		if(lCell)
			_extensionLength = std::max(_extensionLength, lCell->getRadius());
	}
	Octree<TNodeSDS>::topNode.setExtensionLength(_extensionLength);

	Octree<TNodeSDS>::construct(pSpatialables->begin(), pSpatialables->end());
	Octree<TNodeSDS>::initialize();
}

template <typename TNodeSDS>
bool OctreeSDS<TNodeSDS>::add(const t_SpatialableAgent_3* pSpa) {
	const auto* lCell = dynamic_cast<const SpheroidalCell*>(pSpa);
	if(lCell) {
		double radSpa = lCell->getRadius();
		if(radSpa > _extensionLength) {
			// update higher extension length
			_extensionLength = radSpa;
			// update ocTree with this new value
			update();
			// redo the add
			return add(pSpa);
		}
	}

	// otherwise let the Octree deal with the add
	return Octree<TNodeSDS>::topNode.add(pSpa);
}

template <typename TNodeSDS>
int OctreeSDS<TNodeSDS>::update() {
	std::set<const t_SpatialableAgent*> containedSpas;
	Octree<TNodeSDS>::getContainedAgents(containedSpas);
	Octree<TNodeSDS>::topNode.clear();
	std::set<const t_SpatialableAgent*>::iterator itSpa;

	// TODO : recompute bounding box to make sur this is ok.
	Octree<TNodeSDS>::construct(containedSpas.begin(), containedSpas.end());
	Octree<TNodeSDS>::initialize();
	containedSpas.clear();
	Octree<TNodeSDS>::getContainedAgents(containedSpas);

	return 0;
}

template <typename TNodeSDS>
bool OctreeSDS<TNodeSDS>::update(const t_SpatialableAgent* pSpa) {
	assert(pSpa);
	Octree<TNodeSDS>::topNode.remove(pSpa);
	return Octree<TNodeSDS>::topNode.add(pSpa);
}

#endif
