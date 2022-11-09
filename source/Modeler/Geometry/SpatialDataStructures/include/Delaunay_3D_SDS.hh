/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DELAUNAY_3D_SDS
#define DELAUNAY_3D_SDS

#include "SpatialDataStructure.hh"
#include "SpatialableAgent.hh"

#include <map>

#include "AgentSettings.hh"
#include "Mesh3DSettings.hh"

using namespace Settings::nAgent;
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

//////////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define a spatial data structure based on a Delaunay triangulation.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////////////////////
class Delaunay_3D_SDS : public SpatialDataStructure<double, Point_3, Vector_3>
{

public:
	/// \brief constructor
	Delaunay_3D_SDS(QString);
		/// \brief constructor
	Delaunay_3D_SDS(const Delaunay_3D_SDS&);
	///\brief destructor
	virtual ~Delaunay_3D_SDS();

	/// \brief function to call to add a spatialable entity
	bool add(const t_SpatialableAgent_3*);
	/// \brief function to call to remove a spatialable entity
	void remove(const t_SpatialableAgent_3*);
	/// \brief function to call to add a spatialable entity
	bool update(const t_SpatialableAgent_3*);	
	/// \brief update the total triangulation
	int update();
	/// \brief return true if the SDS contains this agent
	bool contains(const t_SpatialableAgent_3* agent)	{return agentToVertex.find(agent) != agentToVertex.end();};
	/// \brief return the list of neighbours
	std::set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3*) const;
	/// \brief return neighbour only if not enought spatialable to generate a tetragulation
	std::set<const t_SpatialableAgent_3*> getNeighboursWithoutTriangulation(const t_SpatialableAgent_3* pAgent) const;
	/// \brief return the nearest agent of a given point
	const t_SpatialableAgent_3* getNearestAgent(const Point_3) const;

	/// \brief clean all
	virtual void clean();

protected:
	RT_3 delaunay;	///< \brief the delaunay regular triangulation ( weighted Delunay )
	/// \brief the map linking agent to his dealunay regular triangulation Vertex
	std::map<const t_SpatialableAgent_3*, Vertex_3_handle> agentToVertex;

};

#endif // DELAUNAY_3D_SDS
