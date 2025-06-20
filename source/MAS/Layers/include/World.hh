#ifndef WORLD_HH
#define WORLD_HH

#include "Dimensioned_Layer.hh"
#include "ViewerUpdater.hh"

/// \brief The world calss, defining the agent's world.
/// A world is composed of an infinite number of layer.
/// A layer include agent.
/// An agent can be included on 0..n Layer.
/// Layers are use to display world and to assure relationship between simulation actors.
///	Heritate from Layer
/// World will delete agent, no worldLayer, He will not display agent in the contrary of WorldLayer
/// World's agents will be set from is initialisation.
/// WorldLayer's agents can be add on the fly before world intialisation
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class World: public ViewerUpdater, public Dimensioned_Layer<Kernel, Point, Vector> {
public:
	World(std::string const& name);
	~World() override;
};

///////////////////////////// FUNCTIONS DEFINITION /////////////////////////////////////

#include <InformationSystemManager.hh>

#ifndef NDEBUG
	#define DEBUG_WORLD 0
#else
	#define DEBUG_WORLD 0	// must always be set to 0
#endif

template<typename Kernel, typename Point, typename Vector>
World<Kernel, Point, Vector>::World(std::string const& name):
	ViewerUpdater(),
	Dimensioned_Layer<Kernel, Point, Vector>(name, nullptr, Layer::ROOT)
{
}

template<typename Kernel, typename Point, typename Vector>
World<Kernel, Point, Vector>::~World() {
	std::set<Agent*> const& lAllAgents = Layer::getUniqueAgentsAndSubAgents();
	for(auto const& agent: lAllAgents)
			delete agent;
}

#endif
