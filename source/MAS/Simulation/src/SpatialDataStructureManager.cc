#include "SpatialDataStructureManager.hh"

static SpatialDataStructureManager* SDSManager = nullptr;

SpatialDataStructureManager::~SpatialDataStructureManager() {
	for(auto* sds2D: spatialDataStructures_2D)
		delete sds2D;
	spatialDataStructures_2D.clear();

	for(auto sds3: spatialDataStructures_3D)
		delete sds3;

	spatialDataStructures_3D.clear();
}

SpatialDataStructureManager* SpatialDataStructureManager::getInstance() {
	if(!SDSManager)
		SDSManager = new SpatialDataStructureManager();
	return SDSManager;
}

bool SpatialDataStructureManager::update() {
	/// update 2D
	for(auto sds2D : spatialDataStructures_2D) {
		if(!update(sds2D))
			return false;
	}

	/// update 3D
	for(auto sds3D : spatialDataStructures_3D) {
		if(!update(sds3D))
			return false;
	}
	return true;
}

/// \param pAgent  the agent we want to update
/// \return true if success
bool SpatialDataStructureManager::update(const SpatialableAgent<double, Point_2, Vector_2>* pAgent) {
	assert(pAgent);
	for(auto sds2D : spatialDataStructures_2D) {
		if(sds2D->contains(pAgent)) {
			if(!sds2D->update(pAgent))
				return false;
		}
	}

	return true;
}

/// \param pAgent  the agent we want to update
/// \return true if success
bool SpatialDataStructureManager::update(const SpatialableAgent<double, Point_3, Vector_3>* pAgent) {
	assert(pAgent);
	/// update 3D
	for(auto sds3D : spatialDataStructures_3D) {
		if(!sds3D->contains(pAgent)) {
			if(!sds3D->update(pAgent))
				return false;
		}
	}

	return true;
}

// 2D
/// \param pSDS  the spatial data structure we want to update
/// \return true if success
bool SpatialDataStructureManager::update(SpatialDataStructure<double, Point_2, Vector_2>* pSDS) {
	if(spatialDataStructures_2D.find(pSDS) == spatialDataStructures_2D.end())
		return false;

	(*spatialDataStructures_2D.find(pSDS))->update();
	return true;
}

// 2D
/// \param pSDS  the spatial data structure we want to update
/// \return true if success
bool SpatialDataStructureManager::update(SpatialDataStructure<double, Point_3, Vector_3>* pSDS) {
	if(spatialDataStructures_3D.find(pSDS) == spatialDataStructures_3D.end())
		return false;

	(*spatialDataStructures_3D.find(pSDS))->update();
	return true;
}

// make registration - 2D
/// \param pSDS  the spatial data structure we want to register
/// \return true if success
bool SpatialDataStructureManager::makeRegistration(SpatialDataStructure<double, Point_2, Vector_2>* pSDS) {
	if(spatialDataStructures_2D.find(pSDS) == spatialDataStructures_2D.end())
		spatialDataStructures_2D.insert(pSDS);

	// no false returned cause if already set the job is done
	return true;
}

// make registration - 3D
/// \param pSDS  the spatial data structure we want to register
/// \return true if success
bool SpatialDataStructureManager::makeRegistration(SpatialDataStructure<double, Point_3, Vector_3>* pSDS) {
	if(spatialDataStructures_3D.find(pSDS) == spatialDataStructures_3D.end())
		spatialDataStructures_3D.insert(pSDS);

	// no false returned cause if already set the job is done
	return true;
}

// make unregistration - 2D
/// \param pSDS  the spatial data structure we want to unregister
void SpatialDataStructureManager::makeUnregistration(SpatialDataStructure<double, Point_2, Vector_2>* pSDS) {
	if(spatialDataStructures_2D.find(pSDS) != spatialDataStructures_2D.end())
		spatialDataStructures_2D.erase(pSDS);
}

// make unregistration - 3D
/// \param pSDS  the spatial data structure we want to unregister
void SpatialDataStructureManager::makeUnregistration(SpatialDataStructure<double, Point_3, Vector_3>* pSDS) {
	if(spatialDataStructures_3D.find(pSDS) != spatialDataStructures_3D.end())
		spatialDataStructures_3D.erase(pSDS);
}

// is registred - 2D
/// \param pSDS  the spatial data structure we want to know if registred
/// \return true if registred
bool SpatialDataStructureManager::isRegistred(SpatialDataStructure<double, Point_2, Vector_2>* pSDS) const {
	return spatialDataStructures_2D.find(pSDS) != spatialDataStructures_2D.end();
}

// is registred - 3D
/// \param pSDS  the spatial data structure we want to know if registred
/// \return true if registred
bool SpatialDataStructureManager::isRegistred(SpatialDataStructure<double, Point_3, Vector_3>* pSDS) const {
	return spatialDataStructures_3D.find(pSDS) != spatialDataStructures_3D.end();
}

///   getNeighbours - 2D
/// \param pAgent the agent for which we want the neighbours
/// \return the set of neighbours
std::set<const SpatialableAgent<double, Point_2, Vector_2>*> SpatialDataStructureManager::getNeighbours(const SpatialableAgent<double, Point_2, Vector_2>* pAgent) const {
	std::set<const SpatialableAgent<double, Point_2, Vector_2>*> results;
	for(auto sds : spatialDataStructures_2D) {
		auto currentNeighbour = sds->getNeighbours(pAgent);
		results.insert(currentNeighbour.begin(), currentNeighbour.end());
	}
	return results;
}

///   getNeighbours - 3D
/// \param pAgent the agent for which we want the neighbours
/// \return the set of neighbours
std::set<const SpatialableAgent<double, Point_3, Vector_3>*> SpatialDataStructureManager::getNeighbours(const SpatialableAgent<double, Point_3, Vector_3>* pAgent) const {
	std::set<const SpatialableAgent<double, Point_3, Vector_3>*> results;
	std::set< SpatialDataStructure<double, Point_3, Vector_3>* >::iterator itSDS;
	for(auto sds : spatialDataStructures_3D) {
		auto currentNeighbour = sds->getNeighbours(pAgent);
		results.insert(currentNeighbour.begin(), currentNeighbour.end());
	}

	return results;
}
