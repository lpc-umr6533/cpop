/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SCHEDULER_HH
#define SCHEDULER_HH

#include "Action.hh"

#include <assert.h>
#include <set>

//////////////////////////////////////////////////////////////////////////////
/// \brief Handles scheduling of action and the timer. Set as a singleton
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Scheduler
{
	friend class MASPlatform;

	/// \brief used to served as the comparaison operation between actions
	/// \todo : make a unitary test to check construction is on the right order
	/// redefining the set structure in order to have it on this order : 
	/// 1. EACH BEGIN ITERATION
	/// 2. punctual action ordered by increasing action's time
	struct action_timing_comparison
	{
		/// \brief operator used to order actions
		bool operator() (const Action* act1, const Action* act2)
		{
			if(act1->getFrequency() == Action::EACH_BEGIN_ITERATION ||
				act1->getFrequency() ==	Action::EACH_END_ITERATION)
				return true;

			if(act2->getFrequency() == Action::EACH_BEGIN_ITERATION ||
				act2->getFrequency() ==	Action::EACH_END_ITERATION)
				return false;

			return act1->getActionTime() < act2->getActionTime();
		}
	};

public:
	/// \brief constructor
	Scheduler();
	/// \brief destructor
	~Scheduler();

	/// \brief schedule a given action to process
	bool scheduleAction(Action*);	

	/// \brief return the simulation time
	inline double getRunningTime() const	{return currentTime;};

	/// \brief return the duration of the current simulated step
	inline double getStepDuration() const	{return stepDuration;};
	/// \brief return the singleton of the instance
	static Scheduler* getInstance();
	/// \brief reset all scheduler parameters, timers and action scheduled
	void reset();
	/// \brief Compute the simulation step duration
	double computeSimulationStepDuration();
	/// \brief intialize the scheduler and timers before a new run
	void init();
	/// process post simulation step actions requested
	bool processPostActions()				{ return processActions(true);};
	/// process pre simulation step actions requested
	bool processPreActions()				{ return processActions(false);};

protected:
	/// \brief  define the duration of the simulation
	void setDuration(double pDuration) 		{totalDuration = pDuration;};
	/// \brief  define the duration of a step
	void setStepDuration(double pDuration) 	{stepDuration = pDuration;};

private:
	/// \brief will run each requested actions
	bool processActions(bool postIteration);

	/// \brief duration of a step on the simulation time
	double stepDuration; 	///< in s
	/// \brief the simulation current time.
	double currentTime; 		///< in s
	/// \brief duration of the simulation;
	double totalDuration; 	//< in s

	// The vector structure enable to process actions in the same order they have been registred.
	// \todo : find a better data structure to speed up this process.
	std::set<Action*, action_timing_comparison> preIterationActions;		///< The pre iteration actions
	std::set<Action*, action_timing_comparison> postIterationActions;	///< The post iteration actions
};

#endif