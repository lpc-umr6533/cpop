#include "ThreadAgentGroup.hh"
#include "InformationSystemManager.hh"

#include <cassert>

#ifndef NDEBUG
	#define DEBUG_THREAD_AGENT_GROUP 0
#else
	#define DEBUG_THREAD_AGENT_GROUP 0	// must always stay at 0
#endif

ThreadAgentGroup::ThreadAgentGroup(int pID) :
	_ID(pID)
{
}

ThreadAgentGroup::~ThreadAgentGroup() {
	_agents.clear();
}

/// \param <agentToAdd> { The agent to add}
/// \return
///	0 : success
///	1 : agent == NULL
///	2 : agent already added previously
int ThreadAgentGroup::addAgent(Agent* agentToAdd) {
	assert(agentToAdd);
	if(!agentToAdd) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unvalid agent, NULL pointer", "ThreadAgentGroup");
		return 1;
	}

	/// if already contains the agent
	if(_agents.find(agentToAdd) != _agents.end()) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "unable to add an agent twice", "ThreadAgentGroup");
		return 2;
	} else {
		_agents.insert(agentToAdd);
		return 0;
	}
}

int ThreadAgentGroup::removeAgent(Agent* agentToRemove) {
	if(_agents.find(agentToRemove) != _agents.end())
		_agents.erase(agentToRemove);

	return 0;
}

void ThreadAgentGroup::run() {
	if(DEBUG_THREAD_AGENT_GROUP) {
		std::string mess = "thread " + std::to_string(_ID) + "will process " + std::to_string(_agents.size()) + " agents";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess, "ThreadAgentGroup");
	}

	/// process all agent contained
	for(auto* agent : _agents) {
		assert(agent);
		if(agent->hasToBeExecuted())
			processAgent(static_cast<Agent*>(agent));
	}

	runSucced = true;

	if(DEBUG_THREAD_AGENT_GROUP) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "running thread group over", "ThreadAgentGroup");
}

void ThreadAgentGroup::processAgent(Agent* pAgent) {
	assert(pAgent);
	if(DEBUG_THREAD_AGENT_GROUP) InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "startexecuting an agent", "ThreadAgentGroup");

	switch(pAgent->getState()) {
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

void ThreadAgentGroup::stop() {
	/// stop all agents
	for(auto agent : _agents)
		agent->stop();
}

void ThreadAgentGroup::reset() {
	_agents.clear();
	_stepDuration = 0.;
}
