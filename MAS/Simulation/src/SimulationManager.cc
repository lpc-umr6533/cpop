/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "InformationSystemManager.hh"

#include "RandomEngineManager.hh"
#include "Scheduler.hh"
#include "SimulationManager.hh"
#include "SpatialDataStructureManager.hh"
#include "SpatialConflictSolver.hh"
#include "EngineSettings.hh"
#include <limits>

static SimulationManager* simulationManager = 0;

#ifndef NDEBUG
	#define DEBUG_SIMULATION_MANAGER 0
#else
	#define DEBUG_SIMULATION_MANAGER 0 // must stay at 0
#endif

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
/// \brief constructor
//////////////////////////////////////////////////////////////////////////////////
SimulationManager::SimulationManager() :
	maxThreadAgentGroup(INITIAL_MAX_THREAD),
	nextThreadID(0),
	displacementThreshold(-1.),
	numberOfAgentToExecute(1)
{

#ifdef SIMULATION_VALID_AGENT_NEW_POS	
	// add spatial conflict solver
	addConflictSolver( new SpatialConflictSolver<double, Point_2, Vector_2>() );
	addConflictSolver( new SpatialConflictSolver<double, Point_3, Vector_3>() );
#endif

}

//////////////////////////////////////////////////////////////////////////////////
/// \brief destructor
//////////////////////////////////////////////////////////////////////////////////
SimulationManager::~SimulationManager()
{
	// delete conflict solvers
	vector<ConflictSolver*>::iterator itCS;
	for(itCS = conflictSolvers.begin(); itCS != conflictSolvers.end(); ++ itCS)
	{
		if(*itCS)
		{
			delete *itCS;
			*itCS = NULL;
		}
	}

	// delete threads
	map<int, ThreadAgentGroup*>::iterator itAG;
	for(itAG = agentGroups.begin(); itAG != agentGroups.end(); ++itAG)
	{
		delete (itAG->second);
	}
	agentGroups.clear();
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief return the simulation manager.
/// if doesn't exist create a new instance of it.
//////////////////////////////////////////////////////////////////////////////////
SimulationManager* SimulationManager::getInstance()
{
	if(!simulationManager)
	{
		simulationManager = new SimulationManager();
	}
	return simulationManager;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief This will reset all the sytem ( remove threds, reset timers... )
/// if doesn't exist create a new instance of it.
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::reset()
{
	// reset thread agent group
	map<int, ThreadAgentGroup*>::iterator itThread;
	for(itThread = agentGroups.begin(); itThread != agentGroups.end(); ++itThread)
	{
		itThread->second->reset();
	}
	// reset agents
	agentHandler.clear();
	managedAgents.clear();
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief the initalisation procedure
/// \return int : 0 if succes.
//////////////////////////////////////////////////////////////////////////////////
int SimulationManager::init()
{
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Init", "SimlationManager");
	// ini scheduler
	Scheduler::getInstance()->init();
	// update SDS if some agent change of position from the SDS creation
	updateSDS();

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief add an agent on the simulation
/// return 0 if succes.
//////////////////////////////////////////////////////////////////////////////////
int SimulationManager::addAgent(Agent* pAgent)
{
	assert(pAgent);
	if(pAgent == NULL)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't add agent - NULL Pointer", "SimulationManager");
		return 1;	
	}

	/// if agent already registred
	if(agentHandler.find(pAgent) != agentHandler.end() )
	{
		return 0;
	}

	// add to a thread
	ThreadAgentGroup* thread = findThreadToAddAgent();

	/// if find a correct thread
	if(thread!=NULL)
	{
		/// if succed to add the agent
		if(thread->addAgent(pAgent)==0)
		{
			// register the tuple agent / Thread group
			agentHandler.insert(pair<Agent*, ThreadAgentGroup*> ( pAgent, thread));
			managedAgents.push_back(pAgent);
			return 0;			
		}else
		{
			/// failed to add the agent
			return 3;
		}	
	}else
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't add agent - no thread found", "SimulationManager");
		return 2;
	}
}

//////////////////////////////////////////////////////////////////////////////////
/// \param <pLayer> {The layer to set and from which we update SDS}
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::setTopLayer(Layer* pLayer)
{
	assert(pLayer);
	topLayer = pLayer;
}

//////////////////////////////////////////////////////////////////////////////////
/// \return {True if sucess}
//////////////////////////////////////////////////////////////////////////////////
bool SimulationManager::solveConflicts()
{
	vector<Agent*> agents = getAllAgents();

	vector<ConflictSolver*>::const_iterator itCS;
	for(itCS = conflictSolvers.begin(); itCS != conflictSolvers.end(); ++itCS)
	{
		if(!(*itCS)->solveConflict(agents))
		{
			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
/// \return {True if sucess}
//////////////////////////////////////////////////////////////////////////////////
bool SimulationManager::updateAgentState()
{

#ifdef SIMULATION_VALID_AGENT_NEW_POS	
	/// update position of the agent executed.
	set<Agent*>::iterator itAgent;
	for(itAgent = agentExecutedLastStep.begin(); itAgent != agentExecutedLastStep.end(); ++itAgent)
	{
		// try 2D cast
		{
			t_DynamicAgent_2* dymAgent = dynamic_cast<t_DynamicAgent_2*>(*itAgent);
			if(dymAgent)
			{
				dymAgent->validRequiredPos();
			}
		}

		// try 3D cast
		{
			t_DynamicAgent_3* dymAgent = dynamic_cast<t_DynamicAgent_3*>(*itAgent);
			if(dymAgent)
			{
				dymAgent->validRequiredPos();
			}
		}		
	}
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief the run of the simulation
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::run()
{
	init();
	
	/// signal the begining of the simulation
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "StartRun", "SimlationManager");
	
	bool simulationOver = false;

	/// { run each step iteration
	while(!simulationOver)
	{
		/// - compute duration of this simulation step
		double lDurationStep = Scheduler::getInstance()->computeSimulationStepDuration();

		if(lDurationStep <= 0)
		{
			cout << "\n";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Run over", "SimlationManager");
			return;
		}

		/// - process pre actions
		if(!Scheduler::getInstance()->processPreActions())
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail to process pre actions", "SimlationManager");
			return;
		}

		/// - if running the next step failed
		if(!runOneStep())
		{
			return;
		} 

		/// - solve conflicts
		if( !solveConflicts())
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail, unable to solve conflicts", "SimlationManager");
			return;
		}
		/// - set agents state
		updateAgentState();
		/// - update Spatial data structures
		updateSDS();
		/// - process post actions
		if(!Scheduler::getInstance()->processPostActions() )
		{
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail to process post actions", "SimlationManager");
			return;
		}
		// 	- signal we runned a step
		emit si_stepRunned();
	}

	/// stop thread agent group
	map<int, ThreadAgentGroup*>::iterator lItThread;
	for(lItThread = agentGroups.begin(); lItThread != agentGroups.end(); ++lItThread)
	{	
		((*lItThread).second)->stop();
	}

	cout << endl;
}

//////////////////////////////////////////////////////////////////////////////////
/// \param <nbAgent> {The number of agent to pick}
/// \return {The randomly picked agent}
//////////////////////////////////////////////////////////////////////////////////
set<Agent*> SimulationManager::pickRandomlyAgts(unsigned int nbAgent)
{
	if(nbAgent >= getNbAgent())
	{
		return set<Agent*>(managedAgents.begin(), managedAgents.end());
	}

	assert( getNbAgent() > nbAgent);
	set<Agent*> agentsPicked;
	while(agentsPicked.size() < nbAgent)
	{
		Agent* pickAgt = RandomEngineManager::getInstance()->pickRandom(&managedAgents);
		assert(pickAgt);
		agentsPicked.insert( pickAgt );
	}
	return agentsPicked;
}

//////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::updateSDS()
{
	SpatialDataStructureManager::getInstance()->update();
	/// \todo : check why doesn't work. In many case we should update SDS for some agent and not for all as we do actually
	// if( bExecuteAllAgent )
	// {
	// 	SpatialDataStructureManager::getInstance()->update();
	// }else
	// {
	// 	std::set<Agent*>::iterator itAgent;
	// 	for( itAgent = agentExecutedLastStep.begin(); itAgent != agentExecutedLastStep.end(); ++itAgent )
	// 	{
	// 		const t_SpatialableAgent_2* ag_2 = dynamic_cast<const t_SpatialableAgent_2*>(*itAgent);
	// 		if(ag_2)
	// 		{
	// 			SpatialDataStructureManager::getInstance()->update(ag_2);
	// 		}
	// 		const t_SpatialableAgent_3* ag_3 = dynamic_cast<const t_SpatialableAgent_3*>(*itAgent);
	// 		if(ag_3)
	// 		{
	// 			SpatialDataStructureManager::getInstance()->update(ag_3);
	// 		}			
	// 	}
	// }
}

//////////////////////////////////////////////////////////////////////////////////
/// \return {return true if run is a sucess, else false}
//////////////////////////////////////////////////////////////////////////////////
bool SimulationManager::runOneStep()
{	
	if(DEBUG_SIMULATION_MANAGER) 
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "start running a step", "SimlationManager");
	}else
	{
		cout << ".";
		cout.flush();
	}

	QString mess;

	// update agent state to set if to execute or not
	updateAgentToExecute();
	// run them
	runOneStepWithThread();	


	if(DEBUG_SIMULATION_MANAGER) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "end running a step", "SimulationManager");

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
/// \return {True if succes, else false}
//////////////////////////////////////////////////////////////////////////////////
bool SimulationManager::runOneStepWithThread()
{
	/// run all threads
	map<int, ThreadAgentGroup*>::iterator lItThread;
	for(lItThread = agentGroups.begin(); lItThread != agentGroups.end(); ++lItThread)
	{	
		((*lItThread).second)->start(SIMU_THREAD_PRIORITY);
		if(DEBUG_SIMULATION_MANAGER)
		{
			QString mess = "Starting thread with ID : " + QString::number(((*lItThread).second)->getID());
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "SimulationManager");
		}
	}
	
	/// wait until all threads process
	for(lItThread = agentGroups.begin(); lItThread != agentGroups.end(); ++lItThread)
	{
		((*lItThread).second)->wait();
	}		

	/// check if run is a succes or not
	for(lItThread = agentGroups.begin(); lItThread != agentGroups.end(); ++lItThread)
	{
		if(!((*lItThread).second)->hasSucceeded())
		{
			if(DEBUG_SIMULATION_MANAGER) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "running a step failed. Agent error", "SimlationManager");

			return false;
		}
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////////////
/// \details will tag agent to execute, if tagged to true : will be executed next 
/// round else will not be.
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::updateAgentToExecute()
{
	// get the agent to execute
	if(bExecuteAllAgent || getNbAgent() > numberOfAgentToExecute )
	{
		agentExecutedLastStep.clear();
		agentExecutedLastStep.insert(managedAgents.begin(), managedAgents.end());
	}else
	{
		agentExecutedLastStep = pickRandomlyAgts(numberOfAgentToExecute);
	}

	// reset agent execution
	vector<Agent*>::iterator itAgentReset;
	for(itAgentReset = managedAgents.begin(); itAgentReset != managedAgents.end(); ++itAgentReset )
	{
		(*itAgentReset)->setToBeExecute(false);
	}

	// then tag them
	set<Agent*>::iterator itAgentSet;
	for(itAgentSet = agentExecutedLastStep.begin(); itAgentSet != agentExecutedLastStep.end(); ++itAgentSet)
	{
		assert(*itAgentSet);
		(*itAgentSet)->setToBeExecute(true);
	}
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief Return the most appropriate thread to add this agent.
/// \return <> {return the main adapteed thread. If no thread found, return null}
//////////////////////////////////////////////////////////////////////////////////
ThreadAgentGroup* SimulationManager::findThreadToAddAgent()
{
	assert(maxThreadAgentGroup );
	
	// find the lightest thread
	ThreadAgentGroup* bestGroup = NULL;

	map<int, ThreadAgentGroup*>::const_iterator lItThread = agentGroups.begin();

	while(lItThread != agentGroups.end())
	{
		if(lItThread == agentGroups.begin())
		{
			bestGroup = (*lItThread).second;
		}

		// is the best thread ?
		if(((*lItThread).second)->getNbAgent() < bestGroup->getNbAgent())
		{
			bestGroup = (*lItThread).second;
			/// empty thread ?
		}
		if(bestGroup->getNbAgent() == 0)
		{
			return bestGroup;
		}
		++lItThread;
	}

	// can we allocate a new thread ?
	if(agentGroups.size()< (unsigned int) maxThreadAgentGroup)
	{
		int threadID = getNextThreadID();
		if(threadID != 0)
		{
			ThreadAgentGroup* thread = new ThreadAgentGroup(threadID);
			agentGroups.insert(pair<int, ThreadAgentGroup*>(threadID, thread));
	
			return thread;
		}
	}
	return 	bestGroup;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief add a spatial data structure
/// \return return the next available thread ID, if 0 : no ID
//////////////////////////////////////////////////////////////////////////////////
int SimulationManager::getNextThreadID()
{
	if(nextThreadID < (std::numeric_limits<int>::max)())
	{
		nextThreadID++;
		return nextThreadID;
	}else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////
/// \param pConflictSolver The conflict solver to remove
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::removeConflictSolver(ConflictSolver* pConflictSolver)
{
	assert(pConflictSolver);
	vector<ConflictSolver*>::iterator itCS;
	for(itCS = conflictSolvers.begin(); itCS != conflictSolvers.end(); ++itCS)
	{
		if((*itCS)==pConflictSolver)
		{
			conflictSolvers.erase(itCS);
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
/// \return Return all the agents currently registred
//////////////////////////////////////////////////////////////////////////////////
vector<Agent*> SimulationManager::getAllAgents()
{
	return managedAgents;
}
