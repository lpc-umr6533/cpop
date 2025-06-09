#include "Delaunay_2D_SDS.hh"
#include "Round_Shape.hh"
#include "CellMeshSettings.hh"

#include <CGAL/range_search_delaunay_2.h>
#include <iterator>     // std::back_inserter
#ifndef NDEBUG
 	#define DEBUG_DELAUNAY_2D_SDS 0
#else
	#define DEBUG_DELAUNAY_2D_SDS 0	// must always stay at 0
#endif

#define DEBUG_NEIGHBOUR

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;

/// \param pName the name to give to the agent
Delaunay_2D_SDS::Delaunay_2D_SDS(QString pName):
	SpatialDataStructure<double, Point_2, Vector_2>(pName)
{
}

void Delaunay_2D_SDS::clean() {
	_agentToVertex.clear();
	_delaunayTriangulation.clear();
	assert(_delaunayTriangulation.is_valid());
}

/// \param pSpaAgt The spatialable agent to add
/// \return true if insertion of the agent is a success
bool Delaunay_2D_SDS::add(const t_SpatialableAgent_2* pSpaAgt) {
	assert(pSpaAgt);
	// check if agent get a round shape
	auto* shape = dynamic_cast<Round_Shape<double, Point_2, Vector_2>*>(pSpaAgt->getBody());
	if(!shape) {
		QString mess = "unable to add the agent, the body isn't disc shape.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Weighted Delaunay 2D - SDS");
		return false;
	}

	assert(shape->getRadius() > 0.);
	if(!SpatialDataStructure<double, Point_2, Vector_2>::add(pSpaAgt))
		return false;

	// add the agent to the triangulation
	Vertex_2_handle v = _delaunayTriangulation.insert(Weighted_point_2(pSpaAgt->getPosition(), shape->getRadius()));
	if(v != nullptr) {
		_agentToVertex.insert(std::pair<const t_SpatialableAgent_2*, Vertex_2_handle> (pSpaAgt, v));
		v->info() = pSpaAgt;
		assert(v->info());
	}

	return true;
}

/// \param pSpaAgt The spatialable agent to remove
void Delaunay_2D_SDS::remove(const t_SpatialableAgent_2* pSpaAgt) {
	assert(pSpaAgt);
	if(_agentToVertex.find(pSpaAgt) == _agentToVertex.end())
		return;

	Vertex_2_handle v = _agentToVertex[pSpaAgt];
	_delaunayTriangulation.remove(v);
	_agentToVertex.erase(pSpaAgt);
}

/// \param pAgent The agent we want the neighbour for
/// \return The neighbours
/// \warning this is used only if we are not able to generate a triangulation
std::set<const t_SpatialableAgent_2*> Delaunay_2D_SDS::getNeighboursWithoutTriangulation(const t_SpatialableAgent_2* pAgent) const {
	if(DEBUG_DELAUNAY_2D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : enter", "Delaunay_2D_SDS");

	assert(_delaunayTriangulation.number_of_vertices() < 3);
	std::set<const t_SpatialableAgent_2*> results;
	for(auto const& containedSpatialable : _containedSpatialables) {
		if(containedSpatialable != pAgent)
			results.insert(containedSpatialable);
	}

	if(DEBUG_DELAUNAY_2D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : exit", "Delaunay_2D_SDS");
	return results;
}

/// \param pAgent The agent we want the neighbour for
/// \return The neighbours
inline std::set<const t_SpatialableAgent_2*> Delaunay_2D_SDS::getNeighbours(const t_SpatialableAgent_2* pAgent) const {
	assert(pAgent);

	if(_delaunayTriangulation.number_of_vertices() < 3)
		return getNeighboursWithoutTriangulation(pAgent);

	// check the agent is on the Spatial data structure
	if(_agentToVertex.find(pAgent) == _agentToVertex.end()) {
		QString mess = "unable to give neighbours for agent " + QString::number(pAgent->getID()) + ", not set on the Spatial Data structure";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Weighted Delaunay 2D - SDS");
		return {};
	}

	Vertex_2_handle v = _agentToVertex.find(pAgent)->second;
	// add all connected spatialables
	std::set<const t_SpatialableAgent_2*> neighbours;

	auto* vShape = dynamic_cast<Round_Shape<double, Point_2, Vector_2>*>(v->info()->getBody());

	if(!v->is_hidden()) {
		RT_2::Vertex_circulator vIncident = _delaunayTriangulation.incident_vertices(v);
		RT_2::Vertex_circulator vInit = vIncident;
		do {
			if(/*(!vIncident->is_hidden()) &&*/ (!_delaunayTriangulation.is_infinite(vIncident))) {
				assert(vIncident->info());
				neighbours.insert(vIncident->info());
			}
		} while(++vIncident != vInit);

#ifdef DEBUG_NEIGHBOUR /// TMP PATCH : add some missing intersections
		for(auto itVertex = _delaunayTriangulation.finite_vertices_begin(); itVertex != _delaunayTriangulation.finite_vertices_end(); ++itVertex) {
			if(v->info() != itVertex->info()) {
				if(! itVertex->is_hidden()) {
					/// if had a connection
					double minDist = vShape->getRadius() +  dynamic_cast<Round_Shape<double, Point_2, Vector_2>*>(itVertex->info()->getBody())->getRadius();
					// TODO optimisation
					if(sqrt(CGAL::squared_distance( v->info()->getPosition(), itVertex->info()->getPosition()) ) < minDist) {
						// if connection not referenced
						if(neighbours.find(itVertex->info()) == neighbours.end()) {
							// std::cout << " ONE is missing !!!!" << std::endl;
							neighbours.insert(itVertex->info());
						}
					}
				}
			}
		}
#endif	// TMP fin patch
	}

	return neighbours;
}

/// \param pSpaAgt The agent to update
/// \return true if succeded
inline bool Delaunay_2D_SDS::update(const t_SpatialableAgent_2* pSpaAgt) {
	assert(pSpaAgt);

	if(_agentToVertex.find(pSpaAgt) == _agentToVertex.end())
		return true;

	assert(_agentToVertex[pSpaAgt] != NULL);
	return true; // if a collision return the vertex already at is position.
}

/// \return 0 is succeded
int Delaunay_2D_SDS::update() {
	assert(_delaunayTriangulation.is_valid());
	std::set<const t_SpatialableAgent_2*> agts;
	/// because it is a delaunay it is faster to regenerate it all from scratch
	for(auto const& itAgts : _agentToVertex)
		agts.insert(itAgts.first);
	clean();

	for(auto const& agt : agts)
		add(agt);

	assert(_delaunayTriangulation.is_valid());
	return 0;
}

/// \param pPt the point we want to localize the nearest vertex ( and so agent )
/// \return t_SpatialableAgent the nearest agent localize
const t_SpatialableAgent_2* Delaunay_2D_SDS::getNearestAgent(const Point_2 pPt) const {
	Vertex_2_handle v = _delaunayTriangulation.nearest_power_vertex(pPt);

	if(v != nullptr) {
		assert(v->info());
		return v->info();
	} else {
		return nullptr;
	}
}
