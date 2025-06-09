#ifndef AGENT_HH
#define AGENT_HH

#include "AgentStates.hh"
#include "Body.hh"

#include <CGAL/IO/Color.h>

using namespace AgentStates;
//////////////////////////////////////////////////////////////////////////////////////////
/// \brief Define the structure and the possible call agent can make.
/// Agents are considered as distributed intelligency and can be executed at each step of the simulation.
/// In our case we are running discreet continuous simulation.
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
class Agent {
	friend class MASPlatform;
	friend class IDManager;
	friend class ConflictSolver;

public:
	/// \brief  constructor
	Agent(Body*);
	/// \brief  destructor
	virtual ~Agent();

	/// \brief  the function called before the first run
	virtual int init() = 0;
	/// \brief  return the agent ID
	[[nodiscard]] inline unsigned long int getID() const { return _id; }
	/// \brief  return the agent state
	[[nodiscard]] inline AgentState getState() const { return _state; }

	/// \brief agent state setter.
	bool setState(AgentState);

	/// \brief  the function called at each step. Must defined the agent behavior.
	virtual int exec() = 0;
	/// \brief start the execution of the agent
	virtual int start();
	/// \brief stop the exeution of the agent
	virtual int stop();
	/// \brief draw the agent to the OpenGL world
	virtual void draw() const;
	/// \brief body getter
	[[nodiscard]] Body* getBody() const { return _body; }
	/// \brief will set the ID given
	/// \warning : this shouldn't be used prudently.
	/// no control from the IDManager is made and can generate bad condition !!!
	void forceID(unsigned long int pID) { _id = pID; }
	/// \brief to be executed tag setter
	void setToBeExecute(bool b) { _toBeExecuted = b; }
	/// \brief to be executed tag getter
	[[nodiscard]] bool hasToBeExecuted() const { return _toBeExecuted; }

	/// \brief color getter
	[[nodiscard]] CGAL::Color getColor() const { return _color; }
	/// \brief color setter
	void setColor(CGAL::Color pColor) { _color = pColor; }

protected:
	/// Will move to the agent natural next state
	int goToNextLogicalState();

private:
	/// \brief setter of the ID.
	/// \param pID The new ID
	void setID(unsigned long int pID)	{ _id = pID; }

protected:
	Body* _body; ///< \brief  the physical body of the agent.

private:
	/// \brief  the agent ID, set by the IDManager
	unsigned long int _id;
	/// \brief  the agent state (stopped, running, waitingInit, waitingStart )
	AgentState _state;
	/// \brief tag to know if we have to execute the agent or not
	bool _toBeExecuted;
	CGAL::Color _color; ///< \brief the color to set to the agent
};

#endif
