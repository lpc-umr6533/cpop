/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Agent.hh"
#include "InformationSystemManager.hh"
#include "IDManager.hh"

#include <QString>
/////////////////////////////////////////////////////////////////////
/// \param pAnimat the animation/ behavior of the Agent
/// \param pBody the body ( physical entity ) of the agent
///
/////////////////////////////////////////////////////////////////////
Agent::Agent(Body* pBody) :
	body(pBody),
	toBeExecuted(true)
{
	state = WAITING_FOR_INIT;
	IDManager::getInstance()->setID(this);
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
Agent::~Agent()
{
	delete body;
	body = NULL;
	IDManager::getInstance()->releaseID(this->getID());
}

/////////////////////////////////////////////////////////////////////
/// \brief set the new agent state "newState"
/// \param newState The new state to give to the agent
/// \return true if succcess
/////////////////////////////////////////////////////////////////////
bool Agent::setState(AgentState newState)
{
	if(newState >= NUMBER_OF_STATE)
	{
		return false;
	}

	state = newState;		
	return true;
}

/////////////////////////////////////////////////////////////////////
/// \brief This function permits to go automatically to
/// the next logical step. Steps are on this roder : WAITING_FOR_INIT ==> WAITING_FOR_START ==> RUNNING
/// \return int 0 if success 
/// 	- 0 : ok
///	- 1 : no other next state
///	- 2 : the agent is actually on an unknow state.
/////////////////////////////////////////////////////////////////////
int Agent::goToNextLogicalState()
{
	switch(state)
	{
		case WAITING_FOR_INIT :
		{
			state = WAITING_FOR_START;
			return 0;
		}
		case WAITING_FOR_START :
		case STOPPED : 
		{
			state = RUNNING;
			return 0;
		}
		case RUNNING :
		{
			state = DEAD;
		}
		case DEAD:
		{
			QString mess = "Agent - " + QString::number(ID) + " - has no next logical state, he already reachs the Dead state";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Agent");
			return 1;
		}
		default:
		{
			QString mess = "Agent - " + QString::number(ID) + " has an unknow state"; 
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Agent");
			return 2;
		}
	}
}

/////////////////////////////////////////////////////////////////////
/// \brief function called when the agent his included on the simulation
/////////////////////////////////////////////////////////////////////
int Agent::start()
{
	return 0;
}

/////////////////////////////////////////////////////////////////////
/// \brief function called when we stop an agent.
/////////////////////////////////////////////////////////////////////
int Agent::stop()
{
	state = STOPPED;
	return 0;
}

/////////////////////////////////////////////////////////////////////
/// \brief function called when we want to draw the agent
/////////////////////////////////////////////////////////////////////
void Agent::draw() const
{

}

