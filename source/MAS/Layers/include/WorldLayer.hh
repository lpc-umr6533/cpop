#ifndef WORLD_LAYER_HH
#define WORLD_LAYER_HH

#include "Dimensioned_Layer.hh"

#include <set>

/// \brief Define a world layer. A world layer can include agent
/// \details
/// World will delete agent, no worldLayer,
/// World's agents will be set from is initialisation.
/// WorldLayer's agents can be add on the fly before world intialisation
/// World layer will display agent contained, in the contrary of world
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class WorldLayer : public Dimensioned_Layer<Kernel, Point, Vector> {
public:
	/// \warning : the ID (std::string) must be unique in order to avoid issues.
	WorldLayer(std::string pName, Layer* parent);
	/// \brief destructor
	virtual ~WorldLayer();

	/// \brief adding an agent to the world layer
	inline bool addAgent(Agent*);
	/// \brief removing an agent to the world layer
	void removeAgent(Agent*);
	/// \brief display the layer and all the agent included
	virtual void draw() const;
};

///////////////////////// FUNCTION DEFINITIONS ////////////////////////////

#ifndef NDEBUG
	#define DEBUG_WORLD_LAYER 0
#else
	#define DEBUG_WORLD_LAYER 0 // must always stay at 0
#endif

#include <InformationSystemManager.hh>


#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif

#include <GL/gl.h>

#define LIGHT_GRAY 0.25f

template<typename Kernel, typename Point, typename Vector>
WorldLayer<Kernel, Point, Vector>::WorldLayer(std::string pName, Layer* pParent) :
	Dimensioned_Layer<Kernel, Point, Vector>(std::move(pName), pParent)
{
	///< set the color of the layer for display
	Layer::_color = std::vector<float>(3, LIGHT_GRAY);
}

template<typename Kernel, typename Point, typename Vector>
WorldLayer<Kernel, Point, Vector>::~WorldLayer() {
	/// \warning : do not delete agents, this is the world role
	Layer::_agents.clear();
}

/// \brief add an agent on a layer
/// return true if success
template<typename Kernel, typename Point, typename Vector>
bool WorldLayer<Kernel, Point, Vector>::addAgent(Agent* pAgentToAdd) {
	assert(pAgentToAdd);
	if(!pAgentToAdd)
		return false;

	if(Layer::_agents.find(pAgentToAdd) == Layer::_agents.end())
		Layer::_agents.insert(pAgentToAdd);

	return true;
}

/// \brief add an agent on a layer
template<typename Kernel, typename Point, typename Vector>
void WorldLayer<Kernel, Point, Vector>::removeAgent(Agent* pAgentToRemove) {
	assert(pAgentToRemove);
	if(!pAgentToRemove)
		return;

	Layer::_agents.erase(pAgentToRemove);
}

///////////////////////////////////////////////////////////////////////
/// World layer will display agent contained, in the contrary of world
///////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void WorldLayer<Kernel, Point, Vector>::draw() const {
	Layer::draw();

	// set color
	glColor3f(this->getColor(0), this->getColor(1), this->getColor(2));

	/// draw agents
	for(auto it=Layer::_agents.begin(); it != Layer::_agents.end(); ++it)
		(*it)->draw();

	if(DEBUG_WORLD_LAYER) {
		std::string message = "layer " + Layer::_name + " displaying : " + std::to_string(Layer::_agents.size());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, message, "WorldLayer");
	}
}

#endif
