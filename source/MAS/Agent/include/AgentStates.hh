#ifndef AGENT_STATE
#define AGENT_STATE

/// \brief Agent States
namespace AgentStates {

/// \brief list the potential agent states
enum AgentState {
	RUNNING,            ///< The agent is running
	WAITING_FOR_INIT,   ///< The agent is waiting for initialization
	WAITING_FOR_START,  ///< The agent is waiting to start
	STOPPED,            ///< THe agent is stopped
	DEAD,               ///< The agent is dead
	NUMBER_OF_STATE     ///< must be the last referenced 
};

}

#endif
