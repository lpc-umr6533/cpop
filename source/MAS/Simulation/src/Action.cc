#include "Action.hh"
#include "Scheduler.hh"

Action::Action(ACTION_FREQUENCY pFrequency, double pTime):
	_frequency(pFrequency),
	_actionTime(pTime)
{
	Scheduler::getInstance()->scheduleAction(this);
}
