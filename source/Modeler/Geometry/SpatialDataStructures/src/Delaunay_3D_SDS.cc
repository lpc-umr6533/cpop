/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Delaunay_3D_SDS.hh"

#include "Round_Shape.hh"

#ifndef NDEBUG
 	#define DEBUG_DELAUNAY_3D_SDS 0
#else
	#define DEBUG_DELAUNAY_3D_SDS 0	// must always stay at 0
#endif

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
Delaunay_3D_SDS::Delaunay_3D_SDS(QString pName):
	SpatialDataStructure<double, Point_3, Vector_3>(pName)
{

}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
Delaunay_3D_SDS::~Delaunay_3D_SDS()
{

}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
Delaunay_3D_SDS::Delaunay_3D_SDS(const Delaunay_3D_SDS& pSDS):
	SpatialDataStructure<double, Point_3, Vector_3>(pSDS),
	delaunay(pSDS.delaunay),
	agentToVertex(pSDS.agentToVertex)
{

}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
void Delaunay_3D_SDS::clean()
{
	agentToVertex.clear();
	delaunay.clear();
	assert(delaunay.is_valid());
}

//////////////////////////////////////////////////////////////////////////////
/// \param <pSpaAgt The spatialable agent to add to the SDS.
/// \return true if add is a success
//////////////////////////////////////////////////////////////////////////////
bool Delaunay_3D_SDS::add(const t_SpatialableAgent_3* pSpaAgt)
{
	/// dissable to do nothing cause not 
	assert(pSpaAgt);

	Round_Shape<double, Point_3, Vector_3>* shape = dynamic_cast<Round_Shape<double, Point_3, Vector_3>*>(pSpaAgt->getBody()); 
	if(!shape)
	{
		QString mess = "unable to add the agent, the body isn't disc shape.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Weighted Delaunay 3D - SDS");
		return false;
	}

	assert(shape->getRadius() > 0.);
	if(! SpatialDataStructure<double, Point_3, Vector_3>::add(pSpaAgt) )
	{
		return false;
	}

	auto v = delaunay.insert(Weighted_point_3(pSpaAgt->getPosition(), shape->getRadius()));
	if(v != NULL)
	{
		agentToVertex.insert(std::make_pair(pSpaAgt, v));
		v->info() = pSpaAgt;
		assert(v->info());
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pSpaAgt The spatialable agent to remove
//////////////////////////////////////////////////////////////////////////////
void Delaunay_3D_SDS::remove(const t_SpatialableAgent_3* pSpaAgt)
{
	assert(pSpaAgt);
	if(agentToVertex.find(pSpaAgt) == agentToVertex.end())
	{
		return;
	}
	Vertex_3_handle v = agentToVertex[pSpaAgt];
	delaunay.remove(v);
	agentToVertex.erase(pSpaAgt);
}

//////////////////////////////////////////////////////////////////////////////
/// \param pAgent The agent we want the neighbour for
/// \return The neighbours of the given agent
/// \warning this is used only if we are not able to generate a triangulation
/// \details will simply link each agent with all the other agents
//////////////////////////////////////////////////////////////////////////////
std::set<const t_SpatialableAgent_3*> Delaunay_3D_SDS::getNeighboursWithoutTriangulation(const t_SpatialableAgent_3* pAgent) const
{
	assert(delaunay.is_valid());
	if(DEBUG_DELAUNAY_3D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : enter", "Delaunay_3D_SDS");
	
	std::set<const t_SpatialableAgent_3*> results;
	std::set<const t_SpatialableAgent_3*>::const_iterator itCell;
	for(itCell = containedSpatialables.begin(); itCell != containedSpatialables.end(); ++itCell)
	{
		if(*itCell != pAgent)
		{
			results.insert(*itCell);
		}
	}
	if(DEBUG_DELAUNAY_3D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : exit", "Delaunay_3D_SDS");
	return results;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pAgent The agent we want the neighbour for
/// \return The neighbours of the agent
//////////////////////////////////////////////////////////////////////////////
inline std::set<const t_SpatialableAgent_3*> Delaunay_3D_SDS::getNeighbours(const t_SpatialableAgent_3* pAgent) const
{
	assert(pAgent);
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	if(delaunay.number_of_vertices() < 4)
	{
		return getNeighboursWithoutTriangulation(pAgent);
	} 
	
	// check the agent is on the Spatial data structure
	if(agentToVertex.find(pAgent) == agentToVertex.end())
	{
		if(DEBUG_DELAUNAY_3D_SDS)
		{		
			QString mess = "unable to give neighbours for agent " + QString::number(pAgent->getID()) + ", not set on the Spatial Data structure";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Delaunay_3D_SDS");
		}
		return std::set<const t_SpatialableAgent_3*>();
	}

	Vertex_3_handle vertexChecked = agentToVertex.find(pAgent)->second;
	std::vector<Vertex_3_handle> adjacentsVertices;
	delaunay.adjacent_vertices(vertexChecked, std::back_inserter(adjacentsVertices));
	std::set<const t_SpatialableAgent_3*> result;
	for(std::vector<Vertex_3_handle>::iterator itVertex = adjacentsVertices.begin(); itVertex != adjacentsVertices.end(); ++itVertex)
	{
        if(!delaunay.is_infinite(*itVertex))        // needed cause CGAL include the finite vertex.
       	{
			assert((*itVertex)->info());			
			result.insert( (*itVertex)->info());
		}
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////////
/// \param pSpaAgt The agent for which we want to update his position
/// \return bool true if update succeded
//////////////////////////////////////////////////////////////////////////////
bool Delaunay_3D_SDS::update(const t_SpatialableAgent_3* pSpaAgt) 
{
	assert(pSpaAgt);

	if(agentToVertex.find(pSpaAgt) == agentToVertex.end())
	{
		return true;
	}

	assert(agentToVertex[pSpaAgt] != NULL);
	return true;	// if a collision return the vertex already at is position.
}

//////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////
int Delaunay_3D_SDS::update()
{
	assert(delaunay.is_valid());
	std::set<const t_SpatialableAgent_3*> agts;
	/// because it is a delaunay it is faster to regenerate it all from scratch
	std::map<const t_SpatialableAgent_3*, Vertex_3_handle>::iterator itAgts;
	for(itAgts = agentToVertex.begin(); itAgts != agentToVertex.end(); ++itAgts)
	{
		agts.insert(itAgts->first);
	}
	clean();

	std::set<const t_SpatialableAgent_3*>::const_iterator itSpa;
	for(itSpa = agts.begin(); itSpa != agts.end(); ++itSpa)
	{
		add(*itSpa);
	}
	assert(delaunay.is_valid());
	return 0;
}


//////////////////////////////////////////////////////////////////////////////
/// \param pPt the point we want to localize the nearest vertex ( and so agent )
/// \return the nearest agent localize
//////////////////////////////////////////////////////////////////////////////
const t_SpatialableAgent_3* Delaunay_3D_SDS::getNearestAgent(const Point_3 pPt) const
{
	Vertex_3_handle v = delaunay.nearest_power_vertex(pPt);

	if(v != NULL)
	{
		assert(v->info());
		return v->info();
	}else
	{
		return NULL;
	}
}
