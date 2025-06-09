#ifndef SPATIAL_DATA_STRUCTURE_MANAGER
#define SPATIAL_DATA_STRUCTURE_MANAGER

#include "Agent.hh"
#include "InformationSystemManager.hh"
#include "Layer.hh"
#include "SpatialDataStructure.hh"
#include "AgentSettings.hh"

#include <set>

using namespace Settings::Geometry;
using namespace Settings::nAgent;

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief  The spatial data structure manager register all SDS. Communicate with the SimulationManager
/// to make all registred SDS is updated when needed
/// \details Because we have to deal with n dimensions, SDS we have specify n request funtion.
/// Defined as a singleton.
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
class SpatialDataStructureManager
{

public:
	~SpatialDataStructureManager();
	/// \brief return the singleton of the spatialDataStructureMaanger
	static SpatialDataStructureManager* getInstance();

	/// \brief return the list of unique neighbours from a set of layer
	template<typename Kernel, typename Point, typename Vector>
	std::set<const SpatialableAgent<Kernel, Point, Vector>*> 		getNeighbours(const SpatialableAgent<Kernel, Point, Vector>* pAgent) const;
	/// \brief return the list of unique neighbours from a set of layer - 2D Specification
	std::set<const SpatialableAgent<double, Point_2, Vector_2>*> 	getNeighbours(const SpatialableAgent<double, Point_2, Vector_2>* pAgent) const;
	/// \brief return the list of unique neighbours from a set of layer - 3D Specification
	std::set<const SpatialableAgent<double, Point_3, Vector_3>*> 	getNeighbours(const SpatialableAgent<double, Point_3, Vector_3>* pAgent) const;

	/// \brief update all data structures
	template<typename Kernel, typename Point, typename Vector>
	bool update(SpatialDataStructure<Kernel, Point, Vector>* );
	/// \brief update all data structures - 2D Specification
	bool update(SpatialDataStructure<double, Point_2, Vector_2>* );
	/// \brief update all data structures - 3D Specification
	bool update(SpatialDataStructure<double, Point_3, Vector_3>* );

	/// \brief update all SDS
	bool update();
	/// \brief update a specific spatialable - 2D specification
	bool update(const SpatialableAgent<double, Point_2, Vector_2>* );
	/// \brief update a specific spatialable - 3D specification
	bool update(const SpatialableAgent<double, Point_3, Vector_3>* );


	/// \brief sptatial data structure registration
	template<typename Kernel, typename Point, typename Vector>
	bool makeRegistration(SpatialDataStructure<Kernel, Point, Vector>*);
	/// \brief sptatial data structure registration - 2D specification
	bool makeRegistration(SpatialDataStructure<double, Point_2, Vector_2>*);
	/// \brief sptatial data structure registration - 3D specification
	bool makeRegistration(SpatialDataStructure<double, Point_3, Vector_3>*);
	/// \brief sptatial data structure unregistration
	template<typename Kernel, typename Point, typename Vector>
	void makeUnregistration(SpatialDataStructure<Kernel, Point, Vector>*);
	/// \brief sptatial data structure unregistration - 2D specification
	void makeUnregistration(SpatialDataStructure<double, Point_2, Vector_2>*);
	/// \brief sptatial data structure unregistration - 3D specification
	void makeUnregistration(SpatialDataStructure<double, Point_3, Vector_3>*);

	/// \brief function to know if a staial data structure is registred
	template<typename Kernel, typename Point, typename Vector>
	bool isRegistred(SpatialDataStructure<Kernel, Point, Vector>*) const;
	/// \brief function to know if a staial data structure is registred - 2D specification
	bool isRegistred(SpatialDataStructure<double, Point_2, Vector_2>*) const;
	/// \brief function to know if a staial data structure is registred - 3D specification
	bool isRegistred(SpatialDataStructure<double, Point_3, Vector_3>*) const;

private:
	std::set<SpatialDataStructure<double, Point_2, Vector_2>* > spatialDataStructures_2D;	///< \brief the 2D set of spatial data structures
	std::set<SpatialDataStructure<double, Point_3, Vector_3>* > spatialDataStructures_3D;	///< \brief the 3D set of spatial data structures

};

///////////////////// template function definitions ////////

///   getNeighbours
/// \param pAgent the agent for which we want the neighbours
/// \return the set of neighbours
template<typename Kernel, typename Point, typename Vector>
std::set<const SpatialableAgent<Kernel, Point, Vector>* > SpatialDataStructureManager::getNeighbours(
	const SpatialableAgent<Kernel, Point, Vector>* pAgent
) const
{
	InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "unable to deal with this template parameters", "SpatialDataStructureManager");
	return std::set<const Spatialable<Kernel, Point, Vector>*>();
}

///   update
/// \param pSDS  the spatial data structure we want to update
/// \return true if success
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructureManager::update(SpatialDataStructure<Kernel, Point, Vector>* pSDS) {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "update - unable to deal with this template parameters", "SpatialDataStructureManager");
	return false;
}

// make registration
/// \param pSDS  the spatial data structure we want to register
/// \return true if success
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructureManager::makeRegistration(SpatialDataStructure<Kernel, Point, Vector>* pSDS) {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "make registration - unable to deal with this template parameters", "SpatialDataStructureManager");

	return false;
}

// make unregistration
/// \param pSDS  the spatial data structure we want to unregister
template<typename Kernel, typename Point, typename Vector>
void SpatialDataStructureManager::makeUnregistration(SpatialDataStructure<Kernel, Point, Vector>* pSDS) {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "make unregistration - unable to deal with this template parameters", "SpatialDataStructureManager");

}

// is registred
/// \param pSDS  the spatial data structure we want to know if registred
/// \return true if registred
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructureManager::isRegistred(SpatialDataStructure<Kernel, Point, Vector>* pSDS) const {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "is registred - unable to deal with this template parameters", "SpatialDataStructureManager");
	return false;
}

#endif
