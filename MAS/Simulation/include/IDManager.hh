/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ID_MANAGER_HH
#define ID_MANAGER_HH

#include "Agent.hh"

#include <QString>

#include <map>
#include <set>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
/// \brief define a struct of an ID Map
//////////////////////////////////////////////////////////////////////////////
struct IDMap
{
	unsigned long int currentID;			///< the next ID to give from the first itération (make sure we give unreleased IDs first )
											/// \warning we will start giving IDs at currentID +1.
	set< unsigned long int> releasedIDs;	///< \brief the released IDS to give once all unsigned long int have been distributed.
	/// \brief store ID information, free ID and lock IDs
	IDMap(unsigned long int pCurrentIDs = 0, set< unsigned long int> pReleaseID = set< unsigned long int>() )
	{
		currentID = pCurrentIDs;
		releasedIDs = pReleaseID;
	}
};	

//////////////////////////////////////////////////////////////////////////////
/// \brief Defined as a singleton, this will handle ID attribution for all entities 
/// requiring a unique ID.
/// \details The ID '0' correspond to unsetted ID (ID by default : because is an unsigned long int)
/// IDManager return IDs starting at 1.
/// \note For now only attribution, no deletion. But as setted on an unsigned int, this havn't be
/// juged usefull.
//////////////////////////////////////////////////////////////////////////////
class IDManager
{
public:
	/// \brief constructor
	IDManager();
	/// \brief destructor
	~IDManager();
	static IDManager* getInstance();
	/// \brief return the next available ID
	unsigned long int getID()				{ return getID_internal(&agentIDs);};
	/// \brief will set an ID to the given agent
	bool setID(Agent*);
	/// \brief will release/free the given ID to be resed if needed
	void releaseID(unsigned long int pID)	{ releaseID_internal(&agentIDs, pID);};

	/// \brief this will return an Id from a specific ID map ( != agentID). If the map doesn't exists yet, she will be created
	unsigned long int getSpecificIDFor( QString IDMap );
	/// \brief release an ID from a specific map IS
	void releaseSpecificIDFor(QString IDMap, unsigned long int );

private:
	/// \brief deal directly with function giving unsigned long int and the set corresponding.
	unsigned long int getID_internal(IDMap* );
	/// \brief release an ID on the given ID Map
	void releaseID_internal(IDMap*, unsigned long int);
	/// \brief return the IDsMap corresponding to the given name.
	IDMap* getOrCreateIDsMap(QString);

private:
	IDMap agentIDs;							///< \brief the current free ID "pointer" and the corresponding set. Free ID will be setted once max(unsigned long int) reach. Is a set to make sure none affected twice.

	map<QString, IDMap > specificsIDsMap;	///< \brief all the existings specifics map IDs;
};

#endif // ID_MANAGER_HH
