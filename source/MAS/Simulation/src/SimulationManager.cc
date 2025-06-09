#include "InformationSystemManager.hh"

#include "RandomEngineManager.hh"
#include "Scheduler.hh"
#include "SimulationManager.hh"
#include "SpatialDataStructureManager.hh"
#include "EngineSettings.hh"
#include <limits>

static SimulationManager* simulationManager = nullptr;

#ifndef NDEBUG
	#define DEBUG_SIMULATION_MANAGER 0
#else
	#define DEBUG_SIMULATION_MANAGER 0 // must stay at 0
#endif

SimulationManager::SimulationManager() :
	_maxThreadAgentGroup(INITIAL_MAX_THREAD),
	_nextThreadID(0),
	_displacementThreshold(-1.),
	_numberOfAgentToExecute(1)
{

#ifdef SIMULATION_VALID_AGENT_NEW_POS
	// add spatial conflict solver
	addConflictSolver( new SpatialConflictSolver<double, Point_2, Vector_2>());
	addConflictSolver( new SpatialConflictSolver<double, Point_3, Vector_3>());
#endif

}

SimulationManager::~SimulationManager()
{
	// delete conflict solvers
	for(auto*& conflictSolver: _conflictSolvers) {
		delete conflictSolver;
		conflictSolver = nullptr;
	}

	// delete threads
	for(auto const& agentGroup: _agentGroups)
		delete agentGroup.second;

	_agentGroups.clear();
}

/// \brief return the simulation manager.
/// if doesn't exist create a new instance of it.
SimulationManager* SimulationManager::getInstance() {
	if(!simulationManager)
		simulationManager = new SimulationManager();
	return simulationManager;
}

/// \brief This will reset all the sytem ( remove threds, reset timers... )
/// if doesn't exist create a new instance of it.
void SimulationManager::reset() {
	// reset thread agent group
	for(auto& agentGroup: _agentGroups)
		agentGroup.second->reset();

	// reset agents
	_agentHandler.clear();
	_managedAgents.clear();
}

/// \brief the initalisation procedure
/// \return int : 0 if succes.
int SimulationManager::init() {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Init", "SimlationManager");
	// ini scheduler
	Scheduler::getInstance()->init();
	// update SDS if some agent change of position from the SDS creation
	updateSDS();

	return 0;
}

/// \brief add an agent on the simulation
/// return 0 if succes.
int SimulationManager::addAgent(Agent* pAgent) {
	assert(pAgent);
	if(!pAgent) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't add agent - NULL Pointer", "SimulationManager");
		return 1;
	}

	/// if agent already registred
	if(_agentHandler.find(pAgent) != _agentHandler.end())
		return 0;

	// add to a thread
	ThreadAgentGroup* thread = findThreadToAddAgent();

	/// if find a correct thread
	if(thread) {
		/// if succed to add the agent
		if(thread->addAgent(pAgent) == 0) {
			// register the tuple agent / Thread group
			_agentHandler.insert(std::pair<Agent*, ThreadAgentGroup*>(pAgent, thread));
			_managedAgents.push_back(pAgent);
			return 0;
		} else {
			/// failed to add the agent
			return 3;
		}
	} else {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't add agent - no thread found", "SimulationManager");
		return 2;
	}
}

/// \param <pLayer> {The layer to set and from which we update SDS}
void SimulationManager::setTopLayer(Layer* pLayer) {
	assert(pLayer);
	_topLayer = pLayer;
}

/// \return {True if sucess}
bool SimulationManager::solveConflicts() {
	std::vector<Agent*> agents = getAllAgents();

	for(auto conflictSolver: _conflictSolvers) {
		if(!conflictSolver->solveConflict(agents))
			return false;
	}
	return true;
}

/// \return {True if sucess}
bool SimulationManager::updateAgentState() {
#ifdef SIMULATION_VALID_AGENT_NEW_POS
	/// update position of the agent executed.
	for(auto itAgent = agentExecutedLastStep.begin(); itAgent != agentExecutedLastStep.end(); ++itAgent) {
		// try 2D cast
		{
			t_DynamicAgent_2* dymAgent = dynamic_cast<t_DynamicAgent_2*>(*itAgent);
			if(dymAgent)
				dymAgent->validRequiredPos();
		}

		// try 3D cast
		{
			t_DynamicAgent_3* dymAgent = dynamic_cast<t_DynamicAgent_3*>(*itAgent);
			if(dymAgent)
				dymAgent->validRequiredPos();
		}
	}
#endif

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
/// \brief the run of the simulation
//////////////////////////////////////////////////////////////////////////////////
void SimulationManager::run() {
	init();

	/// signal the begining of the simulation
	InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "StartRun", "SimlationManager");

	bool simulationOver = false;

	/// { run each step iteration
	while(!simulationOver) {
		/// - compute duration of this simulation step
		double lDurationStep = Scheduler::getInstance()->computeSimulationStepDuration();

		if(lDurationStep <= 0) {
			std::cout << "\n";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Run over", "SimlationManager");
			return;
		}

		/// - process pre actions
		if(!Scheduler::getInstance()->processPreActions()) {
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail to process pre actions", "SimlationManager");
			return;
		}

		/// - if running the next step failed
		if(!runOneStep())
			return;

		/// - solve conflicts
		if( !solveConflicts()) {
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail, unable to solve conflicts", "SimlationManager");
			return;
		}

		/// - set agents state
		updateAgentState();
		/// - update Spatial data structures
		updateSDS();
		/// - process post actions
		if(!Scheduler::getInstance()->processPostActions() ) {
			InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Fail to process post actions", "SimlationManager");
			return;
		}

		// 	- signal we runned a step
		emit si_stepRunned();
	}

	/// stop thread agent group
	for(auto const& agentGroup : _agentGroups)
		agentGroup.second->stop();

	std::cout << std::endl;
}

/// \param <nbAgent> {The number of agent to pick}
/// \return {The randomly picked agent}
std::set<Agent*> SimulationManager::pickRandomlyAgts(unsigned int nbAgent)
{
	if(nbAgent >= getNbAgent())
		return {_managedAgents.begin(), _managedAgents.end()};

	assert(getNbAgent() > nbAgent);

	std::set<Agent*> agentsPicked;
	while(agentsPicked.size() < nbAgent) {
		Agent* pickAgt = RandomEngineManager::getInstance()->pickRandom(&_managedAgents);
		assert(pickAgt);
		agentsPicked.insert( pickAgt );
	}
	return agentsPicked;
}

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

/// \return {return true if run is a sucess, else false}
bool SimulationManager::runOneStep() {
	if(DEBUG_SIMULATION_MANAGER) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "start running a step", "SimlationManager");
	} else {
		std::cout << ".";
		std::cout.flush();
	}

	QString mess;

	// update agent state to set if to execute or not
	updateAgentToExecute();
	// run them
	runOneStepWithThread();


	if(DEBUG_SIMULATION_MANAGER) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "end running a step", "SimulationManager");

	return true;
}

/// \return {True if succes, else false}
bool SimulationManager::runOneStepWithThread()
{
	/// run all threads
	for(auto const& agentGroup: _agentGroups) {
		agentGroup.second->start(SIMU_THREAD_PRIORITY);
		if(DEBUG_SIMULATION_MANAGER)
		{
			QString mess = "Starting thread with ID : " + QString::number(agentGroup.second->getID());
			InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "SimulationManager");
		}
	}

	/// wait until all threads process
	for(auto & agentGroup: _agentGroups)
		agentGroup.second->wait();

	/// check if run is a succes or not
	for(auto & agentGroup: _agentGroups) {
		if(!agentGroup.second->hasSucceeded()) {
			if(DEBUG_SIMULATION_MANAGER) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "running a step failed. Agent error", "SimlationManager");
			return false;
		}
	}

	return true;
}

/// \details will tag agent to execute, if tagged to true : will be executed next
/// round else will not be.
void SimulationManager::updateAgentToExecute() {
	// get the agent to execute
	if(_bExecuteAllAgent || getNbAgent() > _numberOfAgentToExecute) {
		_agentExecutedLastStep.clear();
		_agentExecutedLastStep.insert(_managedAgents.begin(), _managedAgents.end());
	} else {
		_agentExecutedLastStep = pickRandomlyAgts(_numberOfAgentToExecute);
	}

	// reset agent execution
	for(auto* managedAgent: _managedAgents)
		managedAgent->setToBeExecute(false);

	// then tag them
	std::set<Agent*>::iterator itAgentSet;
	for(auto* agent: _agentExecutedLastStep) {
		assert(agent);
		agent->setToBeExecute(true);
	}
}

/// \brief Return the most appropriate thread to add this agent.
/// \return <> {return the main adapteed thread. If no thread found, return null}
ThreadAgentGroup* SimulationManager::findThreadToAddAgent() {
	assert(_maxThreadAgentGroup);

	// find the lightest thread
	ThreadAgentGroup* bestGroup = nullptr;

	auto lItThread = _agentGroups.begin();

	while(lItThread != _agentGroups.end()) {
		if(lItThread == _agentGroups.begin()) {
			bestGroup = (*lItThread).second;
		}

		// is the best thread ?
		if(((*lItThread).second)->getNbAgent() < bestGroup->getNbAgent()) {
			bestGroup = (*lItThread).second;
			/// empty thread ?
		}

		if(bestGroup->getNbAgent() == 0)
			return bestGroup;

		++lItThread;
	}

	// can we allocate a new thread ?
	if(_agentGroups.size()< (unsigned int) _maxThreadAgentGroup) {
		int threadID = getNextThreadID();
		if(threadID != 0) {
			auto* thread = new ThreadAgentGroup(threadID);
			_agentGroups.insert(std::pair<int, ThreadAgentGroup*>(threadID, thread));

			return thread;
		}
	}

	return bestGroup;
}

/// \brief add a spatial data structure
/// \return return the next available thread ID, if 0 : no ID
int SimulationManager::getNextThreadID() {
	if(_nextThreadID < (std::numeric_limits<int>::max)()) {
		++_nextThreadID;
		return _nextThreadID;
	} else {
		return 0;
	}
}

/// \param pConflictSolver The conflict solver to remove
void SimulationManager::removeConflictSolver(ConflictSolver* pConflictSolver) {
	assert(pConflictSolver);
	std::vector<ConflictSolver*>::iterator itCS;
	for(auto itCS = _conflictSolvers.begin(); itCS != _conflictSolvers.end(); ++itCS) {
		if((*itCS)==pConflictSolver) {
			_conflictSolvers.erase(itCS);
			return;
		}
	}
}

/// \return Return all the agents currently registred
std::vector<Agent*> SimulationManager::getAllAgents() {
	return _managedAgents;
}
