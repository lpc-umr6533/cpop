/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "SpatialDataStructureManager.hh"

static SpatialDataStructureManager* SDSManager = 0;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpatialDataStructureManager::SpatialDataStructureManager()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpatialDataStructureManager::~SpatialDataStructureManager()
{
	std::set<SpatialDataStructure<double, Point_2, Vector_2>* >::iterator itSDS_2D;
	for( itSDS_2D = spatialDataStructures_2D.begin(); itSDS_2D != spatialDataStructures_2D.end(); ++itSDS_2D)
	{
		delete *itSDS_2D;
	}
	spatialDataStructures_2D.clear();

	std::set<SpatialDataStructure<double, Point_3, Vector_3>* >::iterator itSDS_3;
	for( itSDS_3 = spatialDataStructures_3D.begin(); itSDS_3 != spatialDataStructures_3D.end(); ++itSDS_3)
	{
		delete *itSDS_3;
	}
	spatialDataStructures_3D.clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SpatialDataStructureManager* SpatialDataStructureManager::getInstance()
{
	if(!SDSManager)
	{
		SDSManager = new SpatialDataStructureManager();
	}
	return SDSManager;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::update()
{
	/// update 2D
	std::set<SpatialDataStructure<double, Point_2, Vector_2>* >::iterator itSDS_2D;
	for(itSDS_2D = spatialDataStructures_2D.begin(); itSDS_2D != spatialDataStructures_2D.end(); ++itSDS_2D )
	{
		if(!update(*itSDS_2D)) 
		{
			return false;
		}
	}

	/// update 3D
	std::set<SpatialDataStructure<double, Point_3, Vector_3>* >::iterator itSDS_3D;
	for(itSDS_3D = spatialDataStructures_3D.begin(); itSDS_3D != spatialDataStructures_3D.end(); ++itSDS_3D )
	{
		if(!update(*itSDS_3D)) 
		{
			return false;
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pAgent  the agent we want to update
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::update(const SpatialableAgent<double, Point_2, Vector_2>* pAgent)
{
	assert(pAgent);
	std::set<SpatialDataStructure<double, Point_2, Vector_2>* >::iterator itSDS_2D;
	for(itSDS_2D = spatialDataStructures_2D.begin(); itSDS_2D != spatialDataStructures_2D.end(); ++itSDS_2D )
	{
		if((*itSDS_2D)->contains(pAgent) )
		{
			if(!(*itSDS_2D)->update(pAgent) )
			{
				return false;
			}
		}
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pAgent  the agent we want to update
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::update(const SpatialableAgent<double, Point_3, Vector_3>* pAgent)
{
	assert(pAgent);
	/// update 3D
	std::set<SpatialDataStructure<double, Point_3, Vector_3>* >::iterator itSDS_3D;
	for(itSDS_3D = spatialDataStructures_3D.begin(); itSDS_3D != spatialDataStructures_3D.end(); ++itSDS_3D )
	{
		if(!(*itSDS_3D)->contains(pAgent) )
		{
			if(!(*itSDS_3D)->update(pAgent) )
			{
				return false;
			}
		}
	}
	return true;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D
/// \param pSDS  the spatial data structure we want to update
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::update(SpatialDataStructure<double, Point_2, Vector_2>* pSDS)
{
	if(spatialDataStructures_2D.find(pSDS) == spatialDataStructures_2D.end())
	{
		return false;
	}

	(*spatialDataStructures_2D.find(pSDS))->update();
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 2D
/// \param pSDS  the spatial data structure we want to update
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::update(SpatialDataStructure<double, Point_3, Vector_3>* pSDS)
{
	if(spatialDataStructures_3D.find(pSDS) == spatialDataStructures_3D.end())
	{
		return false;
	}

	(*spatialDataStructures_3D.find(pSDS))->update();
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// make registration - 2D
/// \param pSDS  the spatial data structure we want to register
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::makeRegistration(SpatialDataStructure<double, Point_2, Vector_2>* pSDS)
{
	if(spatialDataStructures_2D.find(pSDS) == spatialDataStructures_2D.end())
	{
		spatialDataStructures_2D.insert(pSDS);
	}
	// no false returned cause if already set the job is done
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// make registration - 3D
/// \param pSDS  the spatial data structure we want to register
/// \return true if success
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::makeRegistration(SpatialDataStructure<double, Point_3, Vector_3>* pSDS)
{
	if(spatialDataStructures_3D.find(pSDS) == spatialDataStructures_3D.end())
	{
		spatialDataStructures_3D.insert(pSDS);
	}
	// no false returned cause if already set the job is done
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// make unregistration - 2D
/// \param pSDS  the spatial data structure we want to unregister
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SpatialDataStructureManager::makeUnregistration(SpatialDataStructure<double, Point_2, Vector_2>* pSDS)
{
	if(spatialDataStructures_2D.find(pSDS) != spatialDataStructures_2D.end())
	{
		spatialDataStructures_2D.erase(pSDS);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// make unregistration - 3D
/// \param pSDS  the spatial data structure we want to unregister
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SpatialDataStructureManager::makeUnregistration(SpatialDataStructure<double, Point_3, Vector_3>* pSDS)
{
	if(spatialDataStructures_3D.find(pSDS) != spatialDataStructures_3D.end())
	{
		spatialDataStructures_3D.erase(pSDS);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// is registred - 2D
/// \param pSDS  the spatial data structure we want to know if registred
/// \return true if registred
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::isRegistred(SpatialDataStructure<double, Point_2, Vector_2>* pSDS) const
{
	if(spatialDataStructures_2D.find(pSDS) == spatialDataStructures_2D.end())
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// is registred - 3D
/// \param pSDS  the spatial data structure we want to know if registred
/// \return true if registred
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SpatialDataStructureManager::isRegistred(SpatialDataStructure<double, Point_3, Vector_3>* pSDS) const
{
	if(spatialDataStructures_3D.find(pSDS) == spatialDataStructures_3D.end())
	{
		return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///   getNeighbours - 2D
/// \param pAgent the agent for which we want the neighbours
/// \return the set of neighbours
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::set<const SpatialableAgent<double, Point_2, Vector_2>*> SpatialDataStructureManager::getNeighbours(const SpatialableAgent<double, Point_2, Vector_2>* pAgent) const
{
	std::set<const SpatialableAgent<double, Point_2, Vector_2>*> results;
	std::set< SpatialDataStructure<double, Point_2, Vector_2>* >::iterator itSDS;
	for(itSDS = spatialDataStructures_2D.begin(); itSDS != spatialDataStructures_2D.end(); ++itSDS )
	{
		std::set<const SpatialableAgent<double, Point_2, Vector_2>*> currentNeighbour = (*itSDS)->getNeighbours(pAgent);
		results.insert(currentNeighbour.begin(), currentNeighbour.end());
	}
	return results;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///   getNeighbours - 3D
/// \param pAgent the agent for which we want the neighbours
/// \return the set of neighbours
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::set<const SpatialableAgent<double, Point_3, Vector_3>*> SpatialDataStructureManager::getNeighbours(const SpatialableAgent<double, Point_3, Vector_3>* pAgent) const
{
	std::set<const SpatialableAgent<double, Point_3, Vector_3>*> results;
	std::set< SpatialDataStructure<double, Point_3, Vector_3>* >::iterator itSDS;
	for(itSDS = spatialDataStructures_3D.begin(); itSDS != spatialDataStructures_3D.end(); ++itSDS )
	{
		std::set<const SpatialableAgent<double, Point_3, Vector_3>*> currentNeighbour = (*itSDS)->getNeighbours(pAgent);
		results.insert(currentNeighbour.begin(), currentNeighbour.end());
	}

	return results;
}