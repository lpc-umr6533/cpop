/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "MASPlatform.hh"

#include "IDManager.hh"
#include "InformationSystemManager.hh"
#include "Scheduler.hh"
#include "SpatialDataStructureManager.hh"

#ifndef NDEBUG
	#define DEBUG_MAS_PLATFORM 0
#else
	#define DEBUG_MAS_PLATFORM 0	// must always set to 0
#endif

using namespace std;
/////////////////////////////////////////////////////////////////////////////////
/// \param pAgents The list of participants of the simulation
/////////////////////////////////////////////////////////////////////////////////
MASPlatform::MASPlatform(QList<Agent*> pAgents)
{
	for(int iAgent = 0; iAgent < pAgents.size(); iAgent++)
	{
		SimulationManager::getInstance()->addAgent(pAgents[iAgent]);
	}
}

/////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////
MASPlatform::~MASPlatform()
{
	/// delete potential scheduler, simulation manager, SDS manager.
	delete SimulationManager::getInstance();
	delete Scheduler::getInstance();
	delete SpatialDataStructureManager::getInstance();
}

/////////////////////////////////////////////////////////////////////////////////
/// \return true id the agent initialisation proceed well
/////////////////////////////////////////////////////////////////////////////////
bool MASPlatform::initAgentToSimulate()
{
	assert(layerToSimulate);
	layerToSimulate->init();

	// // impact change on the Simulation manager
	SimulationManager::getInstance()->reset();
	// // is 2D or is 3D ?
	SimulationManager::getInstance()->setTopLayer(layerToSimulate);
	set<Agent*> agentsFromLayer = layerToSimulate->getUniqueAgentsAndSubAgents();
	set<Agent*>::const_iterator itAgent;
	/// for all agents in the layer : add them to the simulation
	for(itAgent = agentsFromLayer.begin(); itAgent != agentsFromLayer.end(); itAgent++)
	{
		assert(*itAgent);
		if(!addParticipant(*itAgent))	return false;
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////
/// \return 0 if sucess, else : 
///						- 1 : no world
///						- 2 : no layer
///						- 3 : no agent
/////////////////////////////////////////////////////////////////////////////////
int MASPlatform::startSimulation()
{
	// check we have a layer to simulate
	if(!layerToSimulate)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't launch simulation, no layer to simulate defined", "MASPlatform");
		return 2;
	}

	// if we can start the simulation
	set<Agent*> agentsFromLayer = layerToSimulate->getUniqueAgentsAndSubAgents();
	cout << " number of agent to simulate : " << agentsFromLayer.size() << endl;
	
	// check as some agent to simulate
	if(agentsFromLayer.size() < 1 )
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't launch simulation, no participant", "MASPlatform");
		return 3;
	}

	if(!initAgentToSimulate())
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "can't launch simulation, erreur on adding agent to simulate. NULL agent", "MASPlatform");
		return 3;
	}
	agentsFromLayer = layerToSimulate->getUniqueAgentsAndSubAgents();
	std::cout << " number of agent to simulate : " << agentsFromLayer.size() << std::endl;
	// then run the simulation
	SimulationManager::getInstance()->run();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pAgent The agent to add on the simulation
/// \return error code {return 0 if succes, 1 if invalid agent, 2 if unvalid ID}
/////////////////////////////////////////////////////////////////////////////////
int MASPlatform::addParticipant(Agent* pAgent)
{
	assert(pAgent);
	// check agent
	if(!pAgent)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unvalid agent - refuse adding it", "MASPlatform");
		return 1;
	}

	// agent ID are setted automatically on their creation

	if(DEBUG_MAS_PLATFORM)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "adding a potential participant to the simulation", "MASPlatform");
	}
	return (SimulationManager::getInstance()->addAgent(pAgent) == 0);
}

/////////////////////////////////////////////////////////////////////////////////
/// Must be defined p, order to connect it with the viewer.
/// \param pUpdater the updater to called for view update
/// \return 0 if success
/////////////////////////////////////////////////////////////////////////////////
bool MASPlatform::connectViewerUpdater(ViewerUpdater* pUpdater)
{
	assert(pUpdater);
	QObject::connect(SimulationManager::getInstance(), SIGNAL(si_stepRunned()), pUpdater, SLOT(sl_updated()));
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pDuration The simulation duration ( in s )
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::setDuration(double pDuration)
{
	assert(pDuration > 0.);
	if(pDuration > 0.)
	{
		Scheduler::getInstance()->setDuration(pDuration);
	}
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pStepDuration duration of a step ( in s)
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::setStepDuration(double pStepDuration)
{
	assert(pStepDuration > 0.);
	if(pStepDuration > 0.)
	{
		Scheduler::getInstance()->setStepDuration(pStepDuration);
	}
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pNbThread duration of a step ( in s)
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::setNbMaxThreads(int pNbThread)
{
	assert(pNbThread > 0);
	if(pNbThread > 0 )
	{
		SimulationManager::getInstance()->setMaxNumberOfThread(pNbThread);
	}
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pConflictSolver The conflict solver we want to set
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::addConflictSolver(ConflictSolver* pConflictSolver)
{
	assert(pConflictSolver);
	SimulationManager::getInstance()->addConflictSolver(pConflictSolver);
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pConflictSolver The conflict solver we want to set
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::removeConflictSolver(ConflictSolver* pConflictSolver)
{
	assert(pConflictSolver);
	SimulationManager::getInstance()->removeConflictSolver(pConflictSolver);
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pLayer The layer to simulate during the next simulation
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::setLayerToSimulate(Layer* pLayer)
{
	assert(pLayer);
	layerToSimulate = pLayer;
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pThreshold the thresold to set
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::setDisplacementThreshold(double pThreshold) const
{
	SimulationManager::getInstance()->setDisplacementThreshold(pThreshold);
}

/////////////////////////////////////////////////////////////////////////////////
/// \return The threshold setted
/////////////////////////////////////////////////////////////////////////////////
double MASPlatform::getDisplacementThreshold() const
{
	return SimulationManager::getInstance()->getDisplacementThreshold();
}

/////////////////////////////////////////////////////////////////////////////////
/// \param pNbAgent The number of agent to simulate at each step.
/// \warning this function will avoid multithreading to simulate agent.
/// \warning agent will be picked randomly at each iteration
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::limiteNbAgentToSimulate(unsigned int pNbAgent)
{
	assert(pNbAgent > 0);
	SimulationManager::getInstance()->limiteNbAgentToSimulate(pNbAgent);
}

/////////////////////////////////////////////////////////////////////////////////
/// \param b If true we stop to simulate agent, if false we will simulate 
///	the limited number of agent defined 
/////////////////////////////////////////////////////////////////////////////////
void MASPlatform::unlimiteNbAgentToSimulate(bool b)
{
	SimulationManager::getInstance()->unlimiteNbAgentToSimulate(b);
}
