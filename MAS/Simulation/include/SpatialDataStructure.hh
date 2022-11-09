/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPATIAL_DATA_STRUCTURE
#define SPATIAL_DATA_STRUCTURE

#include "Agent.hh"
#include "SpatialableAgent.hh"

#include <set>
#include <vector>

#include <assert.h>

#include <QString>

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define a spatial data structure able to handle 2D or 3D spatialables
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class SpatialDataStructure
{
	typedef SpatialableAgent<Kernel, Point, Vector> t_SpatialableAgent;	///< \brief a spatialable agent depending on SDS dimensions

public:
	/// \brief constructor
	SpatialDataStructure(const QString);
	/// \brief the copy constructor
	SpatialDataStructure(SpatialDataStructure const&);
	/// \brief destructor
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
	QString getName() const	{return name;};
	/// \brief return the contained agent
	std::set<const t_SpatialableAgent*> getContainedSpatialables() const { return containedSpatialables; }

protected:
	bool iterative;												///< \brief Is the stapial data will include agents from layer child
	std::set<const t_SpatialableAgent*> containedSpatialables;	///< \brief the spatialable agent contained on the SDS
	const QString name;											///< \brief name of the SDS
};


//////////////////////////////// TEMPLATE FUNCTIONS DEFINITION ///////////////////////////////////

#include <InformationSystemManager.hh>

#include <QString>

/////////////////////////////////////////////////////////////////////////////////
/// \param pName the name to set to the SDS
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::SpatialDataStructure(const QString pName):
	name(pName)
{
	
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pSDSToCopy the SDS to copy
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::SpatialDataStructure(SpatialDataStructure const& pSDSToCopy):
	iterative(pSDSToCopy.iterative),
	containedSpatialables(pSDSToCopy.containedSpatialables),
	name(pSDSToCopy.name)
{

}

/////////////////////////////////////////////////////////////////////////////////
/// \brief destructor
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialDataStructure<Kernel, Point, Vector>::~SpatialDataStructure()
{
	containedSpatialables.clear();
}

/////////////////////////////////////////////////////////////////////////////////
/// \return update status, 0 is success
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int SpatialDataStructure<Kernel, Point, Vector>::update()
{
	typename std::set<const t_SpatialableAgent*>::const_iterator itSpa;
	for(itSpa = containedSpatialables.begin(); itSpa != containedSpatialables.end(); ++itSpa)
	{
		if(!update(*itSpa)) return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param toAdd The spatialable to add
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::add(const t_SpatialableAgent* toAdd)
{
	assert(toAdd);
	containedSpatialables.insert(toAdd);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param toAdd The set of spatialable to add
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::add(std::set<const t_SpatialableAgent*> toAdd)
{
	typename std::set<const t_SpatialableAgent*>::const_iterator itSpa;
	for(itSpa = toAdd.begin(); itSpa != toAdd.end(); ++itSpa)
	{
		if(!add(*itSpa))
		{
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param toRemove The spatialable to toRemove
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void SpatialDataStructure<Kernel, Point, Vector>::remove(const t_SpatialableAgent* toRemove)
{
	assert(toRemove);
	containedSpatialables.erase(toRemove);
}

/////////////////////////////////////////////////////////////////////////////////
/// \param toRemove The set of spatialable to remove
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void SpatialDataStructure<Kernel, Point, Vector>::remove(std::set<const t_SpatialableAgent*> toRemove)
{
	typename std::set<const t_SpatialableAgent*>::const_iterator itSpa;
	for(itSpa = toRemove.begin(); itSpa != toRemove.end(); ++itSpa)
	{
		remove(*itSpa);
	}
}


/////////////////////////////////////////////////////////////////////////////////
/// \param pSpa The spatialable we want to check.
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool SpatialDataStructure<Kernel, Point, Vector>::contains(const t_SpatialableAgent* pSpa)
{
	return (containedSpatialables.find(pSpa) != containedSpatialables.end());
}

#endif // SPATIAL_DATA_STRUCTURE
