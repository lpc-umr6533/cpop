#include "Layer.hh"

#include <assert.h>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif
#include <GL/gl.h>

#include <InformationSystemManager.hh>
#include <RandomEngineManager.hh>

#ifndef NDEBUG
	#define DEBUG_LAYER 0
#else
	#define DEBUG_LAYER 0	// must always be set to 0
#endif

/// \param pName 		The name to set to the layer ( ID )
/// \param parent 		The parent Layer of this layer
/// \param pLayerType 	The type of layer : root, node, leaf
Layer::Layer(QString pName, Layer* parent, LayerType pLayerType) :
	_name(pName), _alpha(1.f), _layerType(pLayerType)
{
	if(DEBUG_LAYER) {
		QString mess =  "creating the layer " + _name;
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Layer");
	}

	if(parent)
		parent->addChild(this);
}

Layer::~Layer() {
	for(auto itL = _childLayers.begin(); itL != _childLayers.end(); ++itL) {
		delete itL->second;
		itL->second = nullptr;
	}

	_childLayers.clear();
}

/// \warning Doesn't draw agents for the world heritage
/// in order to draw each only once.
void Layer::draw() const {
	glEnable(GL_BLEND);
	glBlendFunc(Layer::_alpha, Layer::_alpha);

	// draw sub layer.
	for(auto itLayer = _childLayers.begin(); itLayer != _childLayers.end(); ++itLayer)
		itLayer->second->draw();
}

/// \param pLayer The layer to add as a child
bool Layer::addChild(Layer* pLayer) {
	assert(pLayer);

	_childLayers.insert(std::pair<QString, Layer*>(pLayer->getName(), pLayer));
	// the root stay root
	if(_layerType == LEAF)
		_layerType = NODE;

	return true;
}

/// \param pLayer The layer to remove as a child
/// \warning Layer will not be delete
void Layer::removeChild(Layer* pLayer) {
	assert(pLayer);
	if(_childLayers.find(pLayer->getName()) != _childLayers.end())
		_childLayers.erase(pLayer->getName());

	if(_childLayers.size() < 1)
		_layerType = LEAF;
}

/// \return The list of unique agents include inside sub layers}
std::set<Agent*> Layer::getUniqueSubAgents() const {
	std::set<Agent*> res;

	for(auto itLayer = _childLayers.begin(); itLayer != _childLayers.end(); ++itLayer) {
		std::set<Agent*> agentsLvl1 = itLayer->second->getAgents();

		for(auto itAgent = agentsLvl1.begin(); itAgent != agentsLvl1.end(); ++itAgent) {
			/// the set insure the unicity of an element. If already exists, we not be added
			res.insert((*itAgent));
		}

		std::set<Agent*> agentsLvl2 = itLayer->second->getUniqueSubAgents();
		for(auto itAgent = agentsLvl2.begin(); itAgent != agentsLvl2.end(); ++itAgent) {
			/// the set insure the unicity of an element. If already exists, we not be added
			res.insert((*itAgent));
		}
	}

	return res;
}

void Layer::includeSubLayersAgents() {
	if(DEBUG_LAYER)
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Start layer initialiation", "Layer");

	std::set<Agent*> lSubAgents = getUniqueSubAgents();

	for(auto itAgent = lSubAgents.begin(); itAgent != lSubAgents.end(); itAgent++) {
		/// the set insure the unicity of an element. If already exists, we not be added
		_agents.insert(*itAgent);
	}

	if(DEBUG_LAYER) {
		QString mess = " after initialisation, layer " + _name + " contained : " + QString::number(_agents.size());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}
}

/// \return the set of agent include at the level and down ( until LEAF )
std::set<Agent*> Layer::getUniqueAgentsAndSubAgents() const {
	std::set<Agent*> res = _agents;
	std::set<Agent*> subAgents = getUniqueSubAgents();

	for(auto itAgent = subAgents.begin(); itAgent != subAgents.end(); ++itAgent)
		res.insert(*itAgent);

	return res;
}

/// will reference all unique agent and make sure each his execute only once
/// per simulation step.
/// \warning : if this isn't called then no agent will be included
/// 	inside the world.
void Layer::init() {
	if(DEBUG_LAYER) {
		QString mess = "initalisation of layer " + _name + "with state : ";
		switch(_layerType) {
			case ROOT :
			{
				mess += "ROOT";
				break;
			}
			case NODE :
			{
				mess += "NODE";
				break;
			}case LEAF :
			{
				mess += "LEAF";
				break;
			}
			default:
				mess += "UNKNOW";
		}
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}

	/// init child layers
	for(auto itWL = _childLayers.begin(); itWL != _childLayers.end(); ++itWL)
		(itWL)->second->init();

	/// the root will include all sub agents with a unicity control
	/// in order to execute them once only.
	/// remove ? ==> no because the world has a special status and functions ( getnbAgents... )
	if(_layerType == ROOT) {
		_agents.clear();
		includeSubLayersAgents();
	}

	if(DEBUG_LAYER) {
		QString mess = "initalisation done for layer " + _name + "know including" + QString::number(_agents.size()) + " agents.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}
}

/// \param pLayer The layer we want to check if is part of this layer
/// \return true if the layer contains the layer
inline bool Layer::contains(Layer* pLayer) const {
	if(_childLayers.find(pLayer->getName()) != _childLayers.end())
		return true;

	for(auto itLayer = _childLayers.begin(); itLayer != _childLayers.end(); ++itLayer) {
		if(itLayer->second->contains(pLayer))
			return true;
	}

	return false;
}

/// The colors are based on the rgb system
/// \param r The red factor from 0 .to 1.
/// \param g The green factor from 0 .to 1.
/// \param b The blue factor from 0 .to 1.
void Layer::setColor(float r, float g, float b) {
	_color[0] = r;
	_color[1] = g;
	_color[2] = b;
}

/// \param pID The Layer ID
/// \return The requested layer, Null if doesn't exits.
Layer* Layer::getChild(QString pID) const {
	if(_childLayers.find(pID)==_childLayers.end())
		return nullptr;

	return (_childLayers.find(pID)->second);
}

/// \param pNbAgentToPick The number of agent requested
/// \return setOfAgent The result of the request
std::set<Agent*> Layer::getNRandomAgent(unsigned int pNbAgentToPick) const {
	assert(pNbAgentToPick <= getNbAgent());
	if(pNbAgentToPick >= getNbAgent())
		return _agents;

	std::set<Agent*> requestedAgts;
	while(requestedAgts.size() < pNbAgentToPick) {
		// the set<Agent*> structure insure the fact that an agent is at most set once.
		requestedAgts.insert(getOneRandomAgent());
	}

	return requestedAgts;
}

/// \return Agent* The agent picked randomly
Agent* Layer::getOneRandomAgent() const {
	assert(getNbAgent() > 0);
	if(getNbAgent() == 0)
		return nullptr;

	return *RandomEngineManager::getInstance()->pickRandom(_agents.begin(), _agents.end());
}

/// \return Layer* fisrst child of the layer in the alphabetical order, Null if doesn't exits.
Layer* Layer::getFirstChild() const {
	if(_childLayers.size() > 0)
		return _childLayers.begin()->second;
	else
		return nullptr;
}
