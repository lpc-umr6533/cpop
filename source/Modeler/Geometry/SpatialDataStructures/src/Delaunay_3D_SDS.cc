#include "Delaunay_3D_SDS.hh"

#include "Round_Shape.hh"

#ifndef NDEBUG
 	#define DEBUG_DELAUNAY_3D_SDS 0
#else
	#define DEBUG_DELAUNAY_3D_SDS 0	// must always stay at 0
#endif

Delaunay_3D_SDS::Delaunay_3D_SDS(std::string const& name):
	SpatialDataStructure<double, Point_3, Vector_3>(name)
{
}

void Delaunay_3D_SDS::clean() {
	_agentToVertex.clear();
	_delaunay.clear();
	assert(_delaunay.is_valid());
}

/// \param <pSpaAgt The spatialable agent to add to the SDS.
/// \return true if add is a success
bool Delaunay_3D_SDS::add(const t_SpatialableAgent_3* pSpaAgt) {
	/// dissable to do nothing cause not
	assert(pSpaAgt);

	auto* shape = dynamic_cast<Round_Shape<double, Point_3, Vector_3>*>(pSpaAgt->getBody());
	if(!shape) {
		std::string mess = "unable to add the agent, the body isn't disc shape.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Weighted Delaunay 3D - SDS");
		return false;
	}

	assert(shape->getRadius() > 0.);
	if(!SpatialDataStructure<double, Point_3, Vector_3>::add(pSpaAgt))
		return false;

	auto v = _delaunay.insert(Weighted_point_3(pSpaAgt->getPosition(), shape->getRadius()));
	if(v != nullptr) {
		_agentToVertex.insert(std::make_pair(pSpaAgt, v));
		v->info() = pSpaAgt;
		assert(v->info());
	}

	return true;
}

/// \param pSpaAgt The spatialable agent to remove
void Delaunay_3D_SDS::remove(const t_SpatialableAgent_3* pSpaAgt) {
	assert(pSpaAgt);
	if(_agentToVertex.find(pSpaAgt) == _agentToVertex.end())
		return;

	Vertex_3_handle v = _agentToVertex[pSpaAgt];
	_delaunay.remove(v);
	_agentToVertex.erase(pSpaAgt);
}

/// \param pAgent The agent we want the neighbour for
/// \return The neighbours of the given agent
/// \warning this is used only if we are not able to generate a triangulation
/// \details will simply link each agent with all the other agents
std::set<const t_SpatialableAgent_3*> Delaunay_3D_SDS::getNeighboursWithoutTriangulation(const t_SpatialableAgent_3* pAgent) const {
	assert(_delaunay.is_valid());
	if(DEBUG_DELAUNAY_3D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : enter", "Delaunay_3D_SDS");

	std::set<const t_SpatialableAgent_3*> results;
	for(auto const& containedSpatialable : _containedSpatialables) {
		if(containedSpatialable != pAgent)
			results.insert(containedSpatialable);
	}

	if(DEBUG_DELAUNAY_3D_SDS) InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "SDS : exit", "Delaunay_3D_SDS");
	return results;
}

/// \param pAgent The agent we want the neighbour for
/// \return The neighbours of the agent
inline std::set<const t_SpatialableAgent_3*> Delaunay_3D_SDS::getNeighbours(const t_SpatialableAgent_3* pAgent) const {
	assert(pAgent);
	/// by default no hidden point, CGAL triangulation by default set the weight of this kind of point to 0
	if(_delaunay.number_of_vertices() < 4)
		return getNeighboursWithoutTriangulation(pAgent);

	// check the agent is on the Spatial data structure
	if(_agentToVertex.find(pAgent) == _agentToVertex.end()) {
		if(DEBUG_DELAUNAY_3D_SDS) {
			std::string mess = "unable to give neighbours for agent " + std::to_string(pAgent->getID()) + ", not set on the Spatial Data structure";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "Delaunay_3D_SDS");
		}

		return {};
	}

	Vertex_3_handle vertexChecked = _agentToVertex.find(pAgent)->second;
	std::vector<Vertex_3_handle> adjacentsVertices;
	_delaunay.adjacent_vertices(vertexChecked, std::back_inserter(adjacentsVertices));
	std::set<const t_SpatialableAgent_3*> result;
	for(auto const& adjacentsVertice : adjacentsVertices) {
		if(!_delaunay.is_infinite(adjacentsVertice)) {      // needed cause CGAL include the finite vertex.
			assert(adjacentsVertice->info());
			result.insert(adjacentsVertice->info());
		}
	}

	return result;
}

/// \param pSpaAgt The agent for which we want to update his position
/// \return bool true if update succeded
bool Delaunay_3D_SDS::update(const t_SpatialableAgent_3* pSpaAgt) {
	assert(pSpaAgt);

	if(_agentToVertex.find(pSpaAgt) == _agentToVertex.end())
		return true;

	assert(_agentToVertex[pSpaAgt] != NULL);
	return true;	// if a collision return the vertex already at is position.
}

int Delaunay_3D_SDS::update() {
	assert(_delaunay.is_valid());
	std::set<const t_SpatialableAgent_3*> agts;
	/// because it is a delaunay it is faster to regenerate it all from scratch
	for(auto const& itAgts : _agentToVertex)
		agts.insert(itAgts.first);
	clean();

	for(auto const& agt : agts)
		add(agt);

	assert(_delaunay.is_valid());
	return 0;
}

/// \param pPt the point we want to localize the nearest vertex ( and so agent )
/// \return the nearest agent localize
const t_SpatialableAgent_3* Delaunay_3D_SDS::getNearestAgent(const Point_3 pPt) const {
	Vertex_3_handle v = _delaunay.nearest_power_vertex(pPt);

	if(v != nullptr) {
		assert(v->info());
		return v->info();
	} else {
		return nullptr;
	}
}
