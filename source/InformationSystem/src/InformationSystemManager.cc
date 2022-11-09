/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "InformationSystemManager.hh"
#include <sstream> 

static InformationSystemManager* informationManager = 0;

//////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////
InformationSystemManager::InformationSystemManager():
	on(true)
{

}

//////////////////////////////////////////////////////////////////////////////////
/// 
//////////////////////////////////////////////////////////////////////////////////
InformationSystemManager::~InformationSystemManager()
{
	
}

//////////////////////////////////////////////////////////////////////////////////
/// \return the singleton of InformationSystemManager
//////////////////////////////////////////////////////////////////////////////////
InformationSystemManager* InformationSystemManager::getInstance()
{
	if(!informationManager)
	{
		informationManager = new InformationSystemManager();
	}
	return informationManager;
}

//////////////////////////////////////////////////////////////////////////////////
/// \warning : a call between lock output and unlock output must not contain any Message.
//////////////////////////////////////////////////////////////////////////////////
void InformationSystemManager::lockOutput()
{
	printLock.lock();
}

//////////////////////////////////////////////////////////////////////////////////
/// \warning : a call between lock output and unlock output must not contain any Message.
//////////////////////////////////////////////////////////////////////////////////
void InformationSystemManager::unlockOutput()
{
	printLock.unlock();
}

//////////////////////////////////////////////////////////////////////////////////
/// \param pMessType 	The type of message
/// \param pMessage 	The inforamtion to display
/// \param pSource 		The origin of message
//////////////////////////////////////////////////////////////////////////////////
void InformationSystemManager::Message(MessageType pMessType, std::string pMessage, std::string pSource)
{
	std::stringstream global;
	/// if mute mode : no display. But no message saves.
	if(!on)
	{
		return;
	}

	// printLock.lock();
	/// TODO : color display according to message type
	switch(pMessType)
	{
		case CANT_PROCESS_MES :
		{
			global << "  ---    CAN'T PROCESS   ---";
			break;
		}
		case DEBUG_MES :
		{
#ifndef NDEBUG
			global << "  --- DEBUG INFORMATION  ---"; 
#endif
			break;
		}
		case FATAL_ERROR_MES :
		{
			global << "  ---   FATAl  ERROR     ---";
			break;
		}
		case INFORMATION_MES :
		{
			global << "  ---    INFORMATION     ---";
			break;
		}
		default : // OTHER
		{
			global << "  ---      OTHER         ---";
			break;
		}
	}
	global << " @@@ " << pSource;
	global << " : " << pMessage << std::endl;
	
	std::cout << global.str();
}
