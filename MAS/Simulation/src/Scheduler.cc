/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Scheduler.hh"
#include "InformationSystemManager.hh"

static Scheduler* scheduler = 0;

#include <QString>

#ifndef NDEBUG
	#define DEBUG_SCHEDULER 0
#else
	#define DEBUG_SCHEDULER 0 	// must always stay to 0
#endif

#include <iostream>
using namespace std;
//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
Scheduler::Scheduler()
{
	reset();
}

//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
Scheduler::~Scheduler()
{

}

//////////////////////////////////////////////////////////////////////////////////
/// \param pAction The action to schedule
/// \return true if scheduling is a success
//////////////////////////////////////////////////////////////////////////////////
bool Scheduler::scheduleAction(Action* pAction)
{
	assert(pAction);
	switch(pAction->getFrequency())
	{
		case Action::PUNCTUAL_BEFORE_ITERATION :
		{
			if(pAction->getActionTime()< 0.)
			{
				return false;
			}
		}// go throw	
		case Action::EACH_BEGIN_ITERATION:
		{
			preIterationActions.insert(pAction);
			return true;
		}
		case Action::PUNCTUAL_AFTER_ITERATION :
		{
			if(pAction->getActionTime()< 0.)
			{
				return false;
			}
		}// go throw	
		case Action::EACH_END_ITERATION:
		{
			postIterationActions.insert(pAction);
			return true;
		}
		default:	// unknow frequency
		{
			return false;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
/// \param postIteration True if we want to apply the post iteration if we want to apply the pre iteration this will be set as false
/// \return true if the action proceeding ok
//////////////////////////////////////////////////////////////////////////////
bool Scheduler::processActions(bool postIteration)
{
	std::set<Action*, action_timing_comparison>::iterator actionIt;
	// init the iterator
	actionIt = (postIteration ? postIterationActions.begin() : preIterationActions.begin());
	bool lContinue = true;

	//  his execution time is good
	while( lContinue)
	{
		// if no more action to run
		if(actionIt == (postIteration ? postIterationActions.end() : preIterationActions.end()))
		{
			return true; 
		}
		// if to run at each iteration
		if((*actionIt)->getFrequency() == (postIteration? Action::EACH_END_ITERATION : Action::EACH_BEGIN_ITERATION) )
		{
			// if failed to execute the action
			if(!(*actionIt)->exec())
			{
				return false;
			}
			++actionIt;
		}else
		{	
			// if a Ponctual action
			// if the action execution time is over : execute it
			if((*actionIt)->getActionTime()<= getRunningTime())
			{
				// if failed to execute the action
				if(!(*actionIt)->exec())
				{
					return false;
				}
				// then remove this action wich is limited to one
				(postIteration ? postIterationActions : preIterationActions).erase((*actionIt++));

				// !!! DO NOT INCREMENT THE ITERATOR
				// because we juste remove the current action and we want to check the 
				// one just after wich took her place
			}else
			{
				// we want throught all the actions we needed for this simulation time.
				// thanks to the ordonned set of action
				return true;
			}
		}	// end else Ponctual action
	}	// end while (lContinue)
	return true;
}

//////////////////////////////////////////////////////////////////////////////
/// \return the singleton of the scheduler
//////////////////////////////////////////////////////////////////////////////
Scheduler* Scheduler::getInstance()
{
	if(!scheduler)
	{
		scheduler = new Scheduler();
	}
	return scheduler;
}

//////////////////////////////////////////////////////////////////////////////////
/// \return {The duration of the next step}
//////////////////////////////////////////////////////////////////////////////////
double Scheduler::computeSimulationStepDuration()
{
	if(DEBUG_SCHEDULER)
	{
		QString mess = "computing simulation step with : ";
		mess += "\n   - current time : " + QString::number(currentTime);
		mess += "\n   - step duration : " + QString::number(stepDuration);
		mess += "\n   - totalDuration : " + QString::number(totalDuration);
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "SCHEDULER");
	}

	double lNextStepDuration = 0.;
	if((totalDuration - currentTime)/stepDuration > 1 )
	{
		lNextStepDuration = stepDuration;
	}else
	{
		lNextStepDuration = totalDuration - currentTime;
	}
	currentTime += lNextStepDuration;
	return lNextStepDuration;
}

//////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////
void Scheduler::reset()
{
	if(DEBUG_SCHEDULER)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Scheduler reset", "SCHEDULER");
	}
	stepDuration = 0.;
	currentTime = 0.;
	totalDuration = 0.;
}

//////////////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////////////
void Scheduler::init()
{
	if(DEBUG_SCHEDULER)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, "Scheduler initalisation", "SCHEDULER");
	}
	currentTime = 0.;
}