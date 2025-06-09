#ifndef DELAUNAY_2D_SDS
#define DELAUNAY_2D_SDS

#include "SpatialDataStructure.hh"

#include "AgentSettings.hh"
#include "Mesh2DSettings.hh"

#include <map>
#include <set>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;
using namespace Settings::nAgent;

/// \brief Define a spatial data structure based on a Delaunay triangulation.
/// @author Henri Payno
class Delaunay_2D_SDS: public SpatialDataStructure<double, Point_2, Vector_2> {
public:
	Delaunay_2D_SDS(QString);

	/// \brief function to call to add a spatialable entity
	bool add(const t_SpatialableAgent_2*) override;
	/// \brief function to call to remove a spatialable entity
	void remove(const t_SpatialableAgent_2*) override;
	/// \brief function to call to add a spatialable entity
	bool update(const t_SpatialableAgent_2*) override;
	/// \brief update the all SDS
	int update() override;
	/// \brief return true if the SDS contains this agent
	bool contains(const t_SpatialableAgent_2* agent) override { return _agentToVertex.find(agent) != _agentToVertex.end(); }
	/// \brief return the list of neighbours
	std::set<const t_SpatialableAgent_2*> getNeighbours(const t_SpatialableAgent_2*) const override;
	/// \brief function called when not enoufgt cell existing to generate a triangulation
	std::set<const t_SpatialableAgent_2*> getNeighboursWithoutTriangulation(const t_SpatialableAgent_2* pAgent) const;

	/// \brief return the nearest agent of a given point
	[[nodiscard]] const t_SpatialableAgent_2* getNearestAgent(const Point_2) const;

	/// \brief clean the data
	void clean();

protected:
	RT_2 _delaunayTriangulation;	///< \brief the Delaunay 2D regular triangulation( Weighted Delunay )
	/// \brief the map linking spatialable agent to their vertex in the delaunay triangulation
	std::map<const t_SpatialableAgent_2*, Vertex_2_handle> _agentToVertex;
};

#endif
