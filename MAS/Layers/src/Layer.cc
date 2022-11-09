/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
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

//////////////////////////////////////////////////////////////
/// \param pName 		The name to set to the layer ( ID )
/// \param parent 		The parent Layer of this layer
/// \param pLayerType 	The type of layer : root, node, leaf
//////////////////////////////////////////////////////////////
Layer::Layer(QString pName, Layer* parent, LayerType pLayerType) :
	name(pName), alpha(1.f), layerType(pLayerType)
{
	if(DEBUG_LAYER)
	{
		QString mess =  "creating the layer " + name;
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES, mess.toStdString(), "Layer");
	}

	if(parent)
	{
		parent->addChild(this);
	}
}

//////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////
Layer::~Layer()
{
	/// delete all containing layer
	std::map<QString, Layer* >::iterator itL;
	for(itL = childLayers.begin(); itL != childLayers.end(); ++itL)
	{
		delete (itL->second);
		itL->second = NULL;
	}
	childLayers.clear();
}

//////////////////////////////////////////////////////////////
/// \warning Doesn't draw agents for the world heritage
/// in order to draw each only once.
//////////////////////////////////////////////////////////////
void Layer::draw() const
{
	glEnable(GL_BLEND);	
	glBlendFunc(Layer::alpha, Layer::alpha);

	// draw sub layer.
	std::map<QString, Layer*>::const_iterator itLayer;
	for(itLayer = childLayers.begin(); itLayer != childLayers.end(); ++itLayer)
	{
		itLayer->second->draw();
	}
}

/////////////////////////////////////////////////////////////////////
/// \param pLayer The layer to add as a child
/////////////////////////////////////////////////////////////////////
bool Layer::addChild(Layer* pLayer)
{
	assert(pLayer);
	
	childLayers.insert(std::pair<QString, Layer*>(pLayer->getName(), pLayer));
	// the root stay root
	if(layerType==LEAF)
	{
		layerType = NODE;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////
/// \param pLayer The layer to remove as a child
/// \warning Layer will not be delete
/////////////////////////////////////////////////////////////////////
void Layer::removeChild(Layer* pLayer)
{
	assert(pLayer);
	if(childLayers.find(pLayer->getName()) != childLayers.end())
	{
		childLayers.erase(pLayer->getName());
	}
	if(childLayers.size()<1)
	{
		layerType = LEAF;
	}
}

/////////////////////////////////////////////////////////////////////
/// \return The list of unique agents include inside sub layers}
/////////////////////////////////////////////////////////////////////
std::set<Agent*> Layer::getUniqueSubAgents() const
{
	std::set<Agent*> res;
	std::map<QString, Layer*>::const_iterator itLayer;
	for(itLayer = childLayers.begin(); itLayer != childLayers.end(); ++itLayer)
	{
		std::set<Agent*> agentsLvl1 = itLayer->second->getAgents();
		std::set<Agent*>::const_iterator itAgent;
		for(itAgent = agentsLvl1.begin(); itAgent != agentsLvl1.end(); ++itAgent)
		{
			/// the set insure the unicity of an element. If already exists, we not be added
			res.insert((*itAgent));
		}
		std::set<Agent*> agentsLvl2 = itLayer->second->getUniqueSubAgents();
		for(itAgent = agentsLvl2.begin(); itAgent != agentsLvl2.end(); ++itAgent)
		{
			/// the set insure the unicity of an element. If already exists, we not be added
			res.insert((*itAgent));
		}
	}
	return res;
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
void Layer::includeSubLayersAgents()
{
	if(DEBUG_LAYER)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Start layer initialiation", "Layer");
	}

	std::set<Agent*> lSubAgents = getUniqueSubAgents();

	std::set<Agent*>::const_iterator itAgent;
	for(itAgent = lSubAgents.begin(); itAgent != lSubAgents.end(); itAgent++)
	{
		/// the set insure the unicity of an element. If already exists, we not be added
		agents.insert(*itAgent);
	}			
	if(DEBUG_LAYER)
	{
		QString mess = " after initialisation, layer " + name + " contained : " + QString::number(agents.size());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}
}

/////////////////////////////////////////////////////////////////////
/// \return the set of agent include at the level and down ( until LEAF )
/////////////////////////////////////////////////////////////////////
std::set<Agent*> Layer::getUniqueAgentsAndSubAgents() const
{
	std::set<Agent*> res = agents;
	std::set<Agent*> subAgents = getUniqueSubAgents();
	std::set<Agent*>::const_iterator itAgent;
	for(itAgent = subAgents.begin(); itAgent != subAgents.end(); ++itAgent)
	{
		res.insert(*itAgent);
	}
	return res;
}
/////////////////////////////////////////////////////////////////////
/// will reference all unique agent and make sure each his execute only once 
/// per simulation step. 
/// \warning : if this isn't called then no agent will be included 
/// 	inside the world.
/////////////////////////////////////////////////////////////////////
void Layer::init()
{
	if(DEBUG_LAYER)
	{
		QString mess = "initalisation of layer " + name + "with state : ";
		switch(layerType)
		{
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
	std::map<QString, Layer* >::const_iterator itWL;
	for(itWL = childLayers.begin(); itWL != childLayers.end(); ++itWL)
	{
		(itWL)->second->init();
	}

	/// the root will include all sub agents with a unicity control 
	/// in order to execute them once only.
	/// remove ? ==> no because the world has a special status and functions ( getnbAgents... )
	if(layerType == ROOT)
	{
		agents.clear();
		includeSubLayersAgents();	
	}

	if(DEBUG_LAYER)
	{
		QString mess = "initalisation done for layer " + name + "know including" + QString::number(agents.size()) + " agents.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "Layer");
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pLayer The layer we want to check if is part of this layer
/// \return true if the layer contains the layer
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool Layer::contains(Layer* pLayer) const
{
	if(childLayers.find(pLayer->getName()) != childLayers.end())
	{
		return true;
	}
	std::map<QString, Layer*>::const_iterator itLayer;
	for(itLayer = childLayers.begin(); itLayer != childLayers.end(); ++itLayer)
	{
		if(itLayer->second->contains(pLayer))
		{
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// The colors are based on the rgb system
/// \param r The red factor from 0 .to 1.
/// \param g The green factor from 0 .to 1.
/// \param b The blue factor from 0 .to 1.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Layer::setColor(float r, float g, float b)
{
	color[0] = r; 
	color[1] = g; 
	color[2] = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pID The Layer ID
/// \return The requested layer, Null if doesn't exits.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Layer* Layer::getChild(QString pID) const
{
	
	if(childLayers.find(pID)==childLayers.end())
	{
		return NULL;
	}
	return (childLayers.find(pID)->second);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pNbAgentToPick The number of agent requested
/// \return setOfAgent The result of the request
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::set<Agent*> Layer::getNRandomAgent(unsigned int pNbAgentToPick) const
{
	assert(pNbAgentToPick <= getNbAgent());
	if(pNbAgentToPick >= getNbAgent())
	{
		return agents;
	}

	std::set<Agent*> requestedAgts;
	while(requestedAgts.size() < pNbAgentToPick)
	{
		// the set<Agent*> structure insure the fact that an agent is at most set once.
		requestedAgts.insert(getOneRandomAgent());
	}
	return requestedAgts;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return Agent* The agent picked randomly
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Agent* Layer::getOneRandomAgent() const
{
	assert(getNbAgent() > 0);
	if(getNbAgent() == 0 )
	{
		return NULL;
	}
	return *RandomEngineManager::getInstance()->pickRandom(agents.begin(), agents.end());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return Layer* fisrst child of the layer in the alphabetical order, Null if doesn't exits.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Layer* Layer::getFirstChild() const
{
	if(childLayers.size() > 0 )
	{
		return childLayers.begin()->second;
	}else
	{
		return NULL;
	}
}
