#ifndef SIMULATION_MANAGER_HH
#define SIMULATION_MANAGER_HH

#include "ConflictSolver.hh"
#include "Layer.hh"
#include "ThreadAgentGroup.hh"

#include <QThread>

#include <map>
#include <vector>
#include <set>

#include <cassert>

/// \brief The simulation manager handles agent allocation to ThreadAgentGroup in order to make sure
/// each agent are at top execute once (multithreading).
/// \details The simulation is defined as a singleton.
/// @author Henri Payno
class SimulationManager: public QThread {
	Q_OBJECT
	friend class MASPlatform;

public:
	SimulationManager();
	~SimulationManager() override;

	/// \brief start the run based on registred paramters
	void run() override;
	/// \brief return the instance of the simulation manager
	static SimulationManager* getInstance();

	/// \brief return the number of agent on the simulation
	[[nodiscard]] unsigned int getNbAgent() const { return (int)_agentHandler.size(); }

	/// \brief reset the manager.
	void reset();
	/// \brief displacementThreshold getter
	[[nodiscard]] double getDisplacementThreshold() const { return _displacementThreshold; };

protected:
	/// \brief add the agent on the simulation
	int addAgent(Agent*);
	/// \brief the initialization of the simulation manager.
	int init();
	/// \brief conflict solver adder
	void addConflictSolver(ConflictSolver* pConflictSolver) {
		assert(pConflictSolver);
		_conflictSolvers.push_back(pConflictSolver);
	}
	/// \brief conflict solver remover
	void removeConflictSolver(ConflictSolver* pConflictSolver);

	/// \brief displacementThreshold setter
	void setDisplacementThreshold(double pThreshold) { _displacementThreshold = pThreshold; }

	/// \brief we will execute randomly a limited number of agent
	void limiteNbAgentToSimulate(unsigned int i) { _numberOfAgentToExecute = i; }
	/// \brief avoid limitation of agent, execute all agent
	void unlimiteNbAgentToSimulate(bool b) { _bExecuteAllAgent = b; }

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
	std::set<Agent*> pickRandomlyAgts(unsigned int);
	/// \brief setter  of the maximal number of thread
	void setMaxNumberOfThread(int nb) { _maxThreadAgentGroup = nb; }
	/// \brief top layer setter, needed to know SDS to update.
	void setTopLayer(Layer*);
	/// \brief return all the agents running
	std::vector<Agent*> getAllAgents();
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
	std::map<int, ThreadAgentGroup*> _agentGroups;
	/// \brief the map of the agent handlers
	std::map<Agent*, ThreadAgentGroup*> _agentHandler;
	/// \brief store all agent manage
	std::vector<Agent*> _managedAgents;

	/// \brief the set of threads the simulation is waiting for to process next state
	std::set<int> _threadRunning;

	/// \brief the maximal number of thread we enable to launch.
	int _maxThreadAgentGroup;
	/// \brief the next thread ID.
	int _nextThreadID;
	/// \brief The conflict solver which wiil permit agent to run without contradictions
	std::vector<ConflictSolver*> _conflictSolvers;
	/// \brief the top layer we are running the simulation for, to make SDS updates
	Layer* _topLayer;
	/// \brief the displacement threshold for each step simulation. If negative none
	double _displacementThreshold;

	/// \brief do we execute all agent
	bool _bExecuteAllAgent;
	/// \brief the number of agent to exexute if we want to execute a limited number of them.
	unsigned int _numberOfAgentToExecute;
	/// \brief the set of agent executed last step
	std::set<Agent*> _agentExecutedLastStep;

signals:
	/// \brief the signal of the step has end run
	void si_stepRunned();
};

#endif
