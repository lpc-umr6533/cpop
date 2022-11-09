/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "ThreadAgentGroup.hh"
#include "InformationSystemManager.hh"

#include <assert.h>

#ifndef NDEBUG
	#define DEBUG_THREAD_AGENT_GROUP 0
#else
	#define DEBUG_THREAD_AGENT_GROUP 0	// must always stay at 0
#endif

/////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////
ThreadAgentGroup::ThreadAgentGroup(int pID) :
	ID(pID)
{
	
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
ThreadAgentGroup::~ThreadAgentGroup()
{
	agents.clear();
}

/////////////////////////////////////////////////////////////////////
/// \param <agentToAdd> { The agent to add}
/// \return
///	0 : success
///	1 : agent == NULL
///	2 : agent already added previously
/////////////////////////////////////////////////////////////////////
int ThreadAgentGroup::addAgent(Agent* agentToAdd)
{
	assert(agentToAdd);
	if(agentToAdd==NULL)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unvalid agent, NULL pointer", "ThreadAgentGroup");
		return 1;
	}

	/// if already contains the agent
	if(agents.find(agentToAdd) != agents.end())
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unable to add an agent twice", "ThreadAgentGroup");
		return 2;
	}else
	{
		agents.insert(agentToAdd);
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////
int ThreadAgentGroup::removeAgent(Agent* agentToRemove)
{
	if(agents.find(agentToRemove) != agents.end())
	{
		agents.erase(agentToRemove);
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////
void ThreadAgentGroup::run()
{
	if(DEBUG_THREAD_AGENT_GROUP)
	{
		QString mess = "thread " + QString::number(ID) + "will process " + QString::number(agents.size()) + " agents" ;
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "ThreadAgentGroup");
	}
	/// process all agent contained
	std::set<Agent*>::iterator it;
	for(it=agents.begin(); it!=agents.end(); ++it)
	{
		assert(*it);
		if((*it)->hasToBeExecuted())
		{
			processAgent((static_cast<Agent*>(*it)));
		}
	}

	runSucced = true;

	if(DEBUG_THREAD_AGENT_GROUP) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "running thread group over", "ThreadAgentGroup");
}

/////////////////////////////////////////////////////////////////////
/// 
/////////////////////////////////////////////////////////////////////
void ThreadAgentGroup::processAgent(Agent* pAgent)
{
	assert(pAgent);
	if(DEBUG_THREAD_AGENT_GROUP) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "startexecuting an agent", "ThreadAgentGroup");

	switch(pAgent->getState())
	{
		case AgentStates::WAITING_FOR_INIT:
			pAgent->init();
			pAgent->setState(AgentStates::WAITING_FOR_START);
		// no break : go throw
		case AgentStates::WAITING_FOR_START:
		case AgentStates::STOPPED:
			pAgent->start();
			pAgent->setState(AgentStates::RUNNING);
		// no break : go throw
		case AgentStates::RUNNING:
			pAgent->exec();
			break;
		default:
			break;
	}
	if(DEBUG_THREAD_AGENT_GROUP) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "en executing an agent", "ThreadAgentGroup");
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
void ThreadAgentGroup::stop()
{
	/// stop all agents
	std::set<Agent*>::iterator itAgent;
	for(itAgent=agents.begin(); itAgent!=agents.end(); ++itAgent)
	{
		(*itAgent)->stop();
	}
}

/////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////
void ThreadAgentGroup::reset()
{
	agents.clear();
	stepDuration = 0.;
}
