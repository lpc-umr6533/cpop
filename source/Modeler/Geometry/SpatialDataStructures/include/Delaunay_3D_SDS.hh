#ifndef DELAUNAY_3D_SDS
#define DELAUNAY_3D_SDS

#include "SpatialDataStructure.hh"

#include <map>

#include "AgentSettings.hh"
#include "Mesh3DSettings.hh"

using namespace Settings::nAgent;
using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

/// \brief Define a spatial data structure based on a Delaunay triangulation.
/// @author Henri Payno
class Delaunay_3D_SDS : public SpatialDataStructure<double, Point_3, Vector_3> {
public:
	Delaunay_3D_SDS(QString);

	/// \brief function to call to add a spatialable entity
	bool add(const t_SpatialableAgent_3*) override;
	/// \brief function to call to remove a spatialable entity
	void remove(const t_SpatialableAgent_3*) override;
	/// \brief function to call to add a spatialable entity
	bool update(const t_SpatialableAgent_3*) override;
	/// \brief update the total triangulation
	int update() override;
	/// \brief return true if the SDS contains this agent
	bool contains(const t_SpatialableAgent_3* agent) override	{return _agentToVertex.find(agent) != _agentToVertex.end();};
	/// \brief return the list of neighbours
	std::set<const t_SpatialableAgent_3*> getNeighbours(const t_SpatialableAgent_3*) const override;
	/// \brief return neighbour only if not enought spatialable to generate a tetragulation
	std::set<const t_SpatialableAgent_3*> getNeighboursWithoutTriangulation(const t_SpatialableAgent_3* pAgent) const;
	/// \brief return the nearest agent of a given point
	const t_SpatialableAgent_3* getNearestAgent(const Point_3) const;

	/// \brief clean all
	virtual void clean();

protected:
	RT_3 _delaunay;	///< \brief the delaunay regular triangulation ( weighted Delunay )
	/// \brief the map linking agent to his dealunay regular triangulation Vertex
	std::map<const t_SpatialableAgent_3*, Vertex_3_handle> _agentToVertex;
};

#endif
