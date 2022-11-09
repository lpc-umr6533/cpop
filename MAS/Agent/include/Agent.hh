/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
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
class Agent
{
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
	inline unsigned long int getID() const 	{return ID;}; 
	/// \brief  return the agent state
	inline AgentState getState() const 		{return state;};
	
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
	Body* getBody() const					{ return body;};
	/// \brief will set the ID given
	/// \warning : this shouldn't be used prudently.
	/// no control from the IDManager is made and can generate bad condition !!!
	void forceID(unsigned long int pID)		{ID = pID;};
	/// \brief to be executed tag setter
	void setToBeExecute(bool b)				{ toBeExecuted = b;}
	/// \brief to be executed tag getter
	bool hasToBeExecuted()					{ return toBeExecuted;}

	/// \brief color getter
	CGAL::Color getColor() const 		{ return color; }
	/// \brief color setter
	void setColor(CGAL::Color pColor) 	{ color = pColor;}

protected:
	/// Will move to the agent natural next state
	int goToNextLogicalState();
private:
	/// \brief setter of the ID.
	/// \param pID The new ID
	void setID(unsigned long int pID)	{ID = pID;};	
	
protected:	
	Body* body;			///< \brief  the physical body of the agent.
	
private:
	/// \brief  the agent ID, set by the IDManager
	unsigned long int ID;
	/// \brief  the agent state (stopped, running, waitingInit, waitingStart )
	AgentState state;
	/// \brief tag to know if we have to execute the agent or not
	bool toBeExecuted;
	CGAL::Color color;						///< \brief the color to set to the agent

};

#endif // AGENT_HH
