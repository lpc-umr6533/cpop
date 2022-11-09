/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SIMULATION_MANAGER_HH
#define SIMULATION_MANAGER_HH

#include "ConflictSolver.hh"
#include "Layer.hh"
#include "ThreadAgentGroup.hh"

#include <QThread>

#include <map>
#include <vector>
#include <set>

#include <assert.h>

using namespace std;
//////////////////////////////////////////////////////////////////////////////
/// \brief The simulation manager handles agent allocation to ThreadAgentGroup in order to make sure
/// each agent are at top execute once (multithreading). 
/// \details The simulation is defined as a singleton.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class SimulationManager : public QThread
{
	Q_OBJECT	
	friend class MASPlatform;

public:
	/// \brief constructor
	SimulationManager();
	/// \brief destructor
	~SimulationManager();
	/// \brief start the run based on registred paramters
	void run();
	/// \brief return the instance of the simulation manager
	static SimulationManager* getInstance();

	/// \brief return the number of agent on the simulation
	unsigned int getNbAgent() const {return (int)agentHandler.size();};

	/// \brief reset the manager.
	void reset();
	/// \brief displacementThreshold getter
	double getDisplacementThreshold() const						{ return displacementThreshold; };

protected:
	/// \brief add the agent on the simulation
	int addAgent(Agent*);
	/// \brief the initialization of the simulation manager.
	int init(); 
	/// \brief conflict solver adder
	void addConflictSolver(ConflictSolver* pConflictSolver)		{assert(pConflictSolver); conflictSolvers.push_back( pConflictSolver);};
	/// \brief conflict solver remover
	void removeConflictSolver(ConflictSolver* pConflictSolver);

	/// \brief displacementThreshold setter
	void setDisplacementThreshold(double pThreshold)			{ displacementThreshold = pThreshold; };

	/// \brief we will execute randomly a limited number of agent 
	void limiteNbAgentToSimulate(unsigned int i )				{numberOfAgentToExecute = i;};
	/// \brief avoid limitation of agent, execute all agent
	void unlimiteNbAgentToSimulate(bool b)						{bExecuteAllAgent = b;};

private:
	/// \brief return the best trhad to set this agent on.
	ThreadAgentGroup* findThreadToAddAgent();

	/// \brief compute the next step duration
	double computeSimulationStepDuration();
	/// \brief return the next thread ID available
	int getNextThreadID();
	/// \brief run the next step of the simulation
	bool runOneStep();
	/// \brief run one step by the intermediary thread agent group 
	bool runOneStepWithThread();
	/// \brief pick randomly agent from the one to simulate
	set<Agent*> pickRandomlyAgts(unsigned int);
	/// \brief setter  of the maximal number of thread
	void setMaxNumberOfThread(int nb) {maxThreadAgentGroup = nb;};
	/// \brief top layer setter, needed to know SDS to update.
	void setTopLayer(Layer*);
	/// \brief return all the agents running
	vector<Agent*> getAllAgents();
	/// \brief update spatial data structures
	void updateSDS();
	/// \brief run all conflict manager
	bool solveConflicts();
	/// \brief update agent states ( include his position )
	bool updateAgentState();
	/// \brief tag agent to execute during the next simulation step
	void updateAgentToExecute();
	
private:
	/// \brief the map of agent group. The key is the trehad ID
	map<int, ThreadAgentGroup*> agentGroups; 
	/// \brief the map of the agent handlers
	map<Agent*, ThreadAgentGroup*> agentHandler;
	/// \brief store all agent manage
	vector<Agent*> managedAgents;

	/// \brief the set of threads the simulation is waiting for to process next state
	set<int> threadRunning;

	/// \brief the maximal number of thread we enable to launch.
	int maxThreadAgentGroup;
	/// \brief the next thread ID.
	int nextThreadID;
	/// \brief The conflict solver which wiil permit agent to run without contradictions
	vector<ConflictSolver*> conflictSolvers;
	/// \brief the top layer we are running the simulation for, to make SDS updates
	Layer* topLayer;
	/// \brief the displacement threshold for each step simulation. If negative none
	double displacementThreshold;

	/// \brief do we execute all agent
	bool bExecuteAllAgent;
	/// \brief the number of agent to exexute if we want to execute a limited number of them.
	unsigned int numberOfAgentToExecute;
	/// \brief the set of agent executed last step
	set<Agent*> agentExecutedLastStep;

signals:
	/// \brief the signal of the step has end run
	void si_stepRunned();
};

#endif
