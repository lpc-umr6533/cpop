#ifndef THREAD_AGENT_GROUP_HH
#define THREAD_AGENT_GROUP_HH

#include "Agent.hh"

#include <QThread>

#include <set>

/// \brief ThreadAgentGroup register a set of agent to be executed. This is the
/// object insuring the multithreded part of the simulation.
/// @author Henri Payno
class ThreadAgentGroup: public QThread {
	Q_OBJECT
	friend class SimulationManager;

public:
	ThreadAgentGroup(int);
	~ThreadAgentGroup() override;

	/// \brief return the run success or not
	bool hasSucceeded() { return runSucced;};

protected:
	/// \brief add the agent on the group
	int addAgent(Agent*);
	/// \brief remove the agent on the group
	int removeAgent(Agent*);
	/// \brief run all the agent of the group
	void run() override;
	/// \brief set the step duration for the simulation
	void setStepDuration(double pDuration) { _stepDuration = pDuration; }
	/// \brief return the number of agent the thread handles
	[[nodiscard]] int getNbAgent() const { return (int) _agents.size(); }
	/// \brief setter for the thread identification
	void setThreadID(int pID) { _ID = pID; }
	/// \brief getter for the thread identification
	[[nodiscard]] int getID()	const { return _ID; }
	/// \brief reset the agent group : remove all agent
	void reset();
	/// \brief stop the threads and agents included.
	void stop();
private:
	/// \brief execute and update agent states
	inline void processAgent(Agent*);

private:
	std::set<Agent*> _agents; ///< \brief  the map of agent the thread agent group handles.
	double _stepDuration;     ///< \brief  simulation step duration ( in s )
	int _ID;                  ///< \brief  the thread ID, set by the simulation manager

	/// \brief  bool update after the run to know if succeeded or not.
	bool runSucced;
};

#endif
