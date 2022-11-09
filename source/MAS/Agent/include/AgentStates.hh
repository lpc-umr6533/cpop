/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef AGENT_STATE
#define AGENT_STATE

/// \brief Agent States
namespace AgentStates
{
	/// \brief list the potential agent states
	enum AgentState
	{
		RUNNING,			///< The agent is running
		WAITING_FOR_INIT,	///< The agent is waiting for initialization
		WAITING_FOR_START,	///< The agent is waiting to start
		STOPPED,			///< THe agent is stopped
		DEAD,				///< The agent is dead
		NUMBER_OF_STATE		///< must be the last referenced 
	};
}

#endif // AGENT_STATE