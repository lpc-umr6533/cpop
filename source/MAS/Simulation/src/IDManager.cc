#include "IDManager.hh"
#include "InformationSystemManager.hh"

#include <cassert>
#include <limits>

static IDManager* idManager = nullptr;

/// \return the ID manager singleton.
IDManager* IDManager::getInstance() {
	if(!idManager) idManager = new IDManager();
	return idManager;
}

/// \param pIDsMap The pair (IDsMap) we want to opere on
/// \return the next available id. If none empty will return the limist::max<double> value
unsigned long int IDManager::getID_internal(IDMap* pIDsMap) {
	if(pIDsMap->currentID < std::numeric_limits<unsigned long int>::max()) {
		pIDsMap->currentID ++;
		return pIDsMap->currentID;
	} else {
		// look at free ID
		if(pIDsMap->releasedIDs.size()==0) {
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "No more available ID", "IDManager");
			return 0;
		} else {
			unsigned long int lID = *(pIDsMap->releasedIDs.begin());
			pIDsMap->releasedIDs.erase(lID);
			return lID;
		}
	}

	return pIDsMap->currentID;
}

/// \param pAgt The agent we want to set the ID for
/// \return  True if set is a success.
bool IDManager::setID(Agent* pAgt) {
	assert(pAgt);
	unsigned long int lID = this->getID_internal(&_agentIDs);
	std::cout << "Setting id: " << lID << std::endl;
	if(lID == 0) {
		return false;
	} else {
		pAgt->setID(lID);
		return true;
	}
}

/// \param pIDsMap The pair (IDsMap) we want to opere on
/// \param pID The ID to release
void IDManager::releaseID_internal(IDMap* pIDsMap, unsigned long int pID) {
	pIDsMap->releasedIDs.insert(pID);
}

/// \param pName The name of the targetted IDs map
IDMap* IDManager::getOrCreateIDsMap(QString pName) {
	// if not existing
	if(_specificsIDsMap.find(pName) == _specificsIDsMap.end())
		_specificsIDsMap.insert(std::make_pair(pName, IDMap()));
	return &_specificsIDsMap[pName];
}

/// \param pIDMap The map to apply the request
/// \return the id requested
unsigned long int IDManager::getSpecificIDFor(QString pIDMap) {
	return getID_internal( getOrCreateIDsMap(pIDMap));
}

/// \param pIDMap The map to apply the request
/// \param pID The id to release
void IDManager::releaseSpecificIDFor(QString pIDMap, unsigned long int pID) {
	releaseID_internal(getOrCreateIDsMap(pIDMap), pID);
}

void IDManager::reset() {
	_agentIDs = IDMap(); // Resets the stored ID map
	_specificsIDsMap.clear(); // Clears all specific ID maps
}

void IDManager::destroyInstance() {
	delete idManager;
	idManager = nullptr;
}
