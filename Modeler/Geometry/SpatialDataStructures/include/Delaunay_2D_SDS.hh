/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DELAUNAY_2D_SDS
#define DELAUNAY_2D_SDS

//#include "CellPopulationSDS.hh"
#include "SpatialDataStructure.hh"

#include "AgentSettings.hh"
#include "Mesh2DSettings.hh"

#include <map>
#include <set>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
using namespace Settings::nAgent;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define a spatial data structure based on a Delaunay triangulation.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class Delaunay_2D_SDS: public SpatialDataStructure<double, Point_2, Vector_2> 
{
public:
	/// \brief constructor
	Delaunay_2D_SDS(QString);
	/// \brief destructor
	virtual ~Delaunay_2D_SDS();

	/// \brief function to call to add a spatialable entity
	bool add(const t_SpatialableAgent_2*);
	/// \brief function to call to remove a spatialable entity
	void remove(const t_SpatialableAgent_2*);
	/// \brief function to call to add a spatialable entity
	bool update(const t_SpatialableAgent_2*);
	/// \brief update the all SDS
	int update();
	/// \brief return true if the SDS contains this agent
	bool contains(const t_SpatialableAgent_2* agent)			{return agentToVertex.find(agent) != agentToVertex.end();};
	/// \brief return the list of neighbours
	std::set<const t_SpatialableAgent_2*> getNeighbours(const t_SpatialableAgent_2*) const;
	/// \brief function called when not enoufgt cell existing to generate a triangulation
	std::set<const t_SpatialableAgent_2*> getNeighboursWithoutTriangulation(const t_SpatialableAgent_2* pAgent) const;

	/// \brief return the nearest agent of a given point
	const t_SpatialableAgent_2* getNearestAgent(const Point_2) const;

	/// \brief clean the data
	void clean();
	
protected:
	RT_2 delaunayTriangulation;	///< \brief the Delaunay 2D regular triangulation( Weighted Delunay )
	/// \brief the map linking spatialable agent to their vertex in the delaunay triangulation
	std::map<const t_SpatialableAgent_2*, Vertex_2_handle> agentToVertex;
};

#endif	// Delaunay_2D_SDS