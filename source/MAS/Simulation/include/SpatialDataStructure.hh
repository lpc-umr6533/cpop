#ifndef SPATIAL_DATA_STRUCTURE
#define SPATIAL_DATA_STRUCTURE

#include "SpatialableAgent.hh"

#include <set>
#include <cassert>

/// \brief Define a spatial data structure able to handle 2D or 3D spatialables
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class SpatialDataStructure
{
	using t_SpatialableAgent = SpatialableAgent<Kernel, Point, Vector>;	///< \brief a spatialable agent depending on SDS dimensions

public:
	SpatialDataStructure(std::string);
	SpatialDataStructure(SpatialDataStructure const&);
	virtual ~SpatialDataStructure();

	/// \brief function to call to add a spatialable entity
	virtual bool add(const t_SpatialableAgent*);
	/// \brief function to call to add a set of spatialable entity
	bool add(std::set<const t_SpatialableAgent*>);
	/// \brief function to call to remove a spatialable entity
	virtual void remove(const t_SpatialableAgent*);
	/// \brief function to call to remove a set of spatialable entity
	void remove(std::set<const t_SpatialableAgent*>);
	/// \brief function called when need an update of the structure
	virtual int update();
	/// \brief function to call to add a spatialable entity
	virtual bool update(const t_SpatialableAgent*) = 0;
	/// \brief return true if the agent is registred on the SDS
	virtual bool contains(const t_SpatialableAgent*);
	/// \brief return the list of neighbours
	virtual std::set<const t_SpatialableAgent*> getNeighbours(const t_SpatialableAgent*) const = 0;
	/// \brief name getter
	[[nodiscard]] std::string getName() const	{ return _name; }
	/// \brief return the contained agent
	std::set<const t_SpatialableAgent*> getContainedSpatialables() const { return _containedSpatialables; }

protected:
	bool _iterative; ///< \brief Is the stapial data will include agents from layer child
	std::set<const t_SpatialableAgent*> _containedSpatialables;	///< \brief the spatialable agent contained on the SDS
	const std::string _name; ///< \brief name of the SDS
};

//////////////////////////////// TEMPLATE FUNCTIONS DEFINITION ///////////////////////////////////

#include <InformationSystemManager.hh>

/// \param pName the name to set to the SDS
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::SpatialDataStructure(std::string name):
	_name(std::move(name))
{
}

/// \param pSDSToCopy the SDS to copy
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::SpatialDataStructure(SpatialDataStructure const& pSDSToCopy):
	_iterative(pSDSToCopy._iterative),
	_containedSpatialables(pSDSToCopy._containedSpatialables),
	_name(pSDSToCopy._name)
{
}

/// \brief destructor
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::~SpatialDataStructure() {
	_containedSpatialables.clear();
}

/// \return update status, 0 is success
template<typename Kernel, typename Point, typename Vector>
int SpatialDataStructure<Kernel, Point, Vector>::update() {
	for(auto itSpa = _containedSpatialables.begin(); itSpa != _containedSpatialables.end(); ++itSpa)
		if(!update(*itSpa)) return 1;
	return 0;
}

/// \param toAdd The spatialable to add
/// \return true if success
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::add(const t_SpatialableAgent* toAdd) {
	assert(toAdd);
	_containedSpatialables.insert(toAdd);
	return true;
}

/// \param toAdd The set of spatialable to add
/// \return true if success
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::add(std::set<const t_SpatialableAgent*> toAdd) {
	for(auto itSpa = toAdd.begin(); itSpa != toAdd.end(); ++itSpa)
		if(!add(*itSpa)) return false;
	return true;
}

/// \param toRemove The spatialable to toRemove
template<typename Kernel, typename Point, typename Vector>
void SpatialDataStructure<Kernel, Point, Vector>::remove(const t_SpatialableAgent* toRemove) {
	assert(toRemove);
	_containedSpatialables.erase(toRemove);
}

/// \param toRemove The set of spatialable to remove
template<typename Kernel, typename Point, typename Vector>
void SpatialDataStructure<Kernel, Point, Vector>::remove(std::set<const t_SpatialableAgent*> toRemove) {
	for(auto itSpa = toRemove.begin(); itSpa != toRemove.end(); ++itSpa)
		remove(*itSpa);
}


/// \param pSpa The spatialable we want to check.
/// \return true if success
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::contains(const t_SpatialableAgent* pSpa) {
	return (_containedSpatialables.find(pSpa) != _containedSpatialables.end());
}

#endif
