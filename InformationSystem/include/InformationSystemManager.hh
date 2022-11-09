/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef INFORMATION_SYSTEM_MANAGER_HH
#define INFORMATION_SYSTEM_MANAGER_HH

#include <iostream>
#include <string>
#include <QMutex>
//////////////////////////////////////////////////////////////////////////////
/// \brief The informationSystemManager is the manager dealing with messages and
/// making sure they are display on there integrality
/// He is defined as a singleton
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class InformationSystemManager
{
public:
	/// \brief Possible message Type
	/// \details According to the type of the message the display will process or not and
	/// will be display in different way ( color ... ) 
	enum MessageType
	{
		CANT_PROCESS_MES,	///< \brief used if a library can't process a request/function
		DEBUG_MES,			///< \brief information display only on debug mode
		FATAL_ERROR_MES,	///< \brief FATAL error, stopping the execution
		INFORMATION_MES,	///< \brief lambda information
		WARNING_MES,		///< \brief warning message, append if some operation are "dangerous" or unsafe.
		OTHER_MES			///< \brief other type of message
	};

public:
	/// \brief constructor
	InformationSystemManager();
	/// \brief destructor
	~InformationSystemManager();

	/// \brief return the singleton of the Information system manager
	static InformationSystemManager* getInstance();
	/// \brief display a message
	void Message(MessageType, std::string, std::string);

	/// \brief stop mute
	void turnOn()	{on = true;};
	/// \brief mute
	void turnOff()	{on = false;};
	/// \brief lock the output
	void lockOutput();
	/// \brief unlick the output
	void unlockOutput();

private:
	QMutex printLock;	///< \brief used to give write of display to an unick emitter ( used on lock putput/unlock output)
	bool on;			///< \brief is the display is turned off or on ?
};

#endif // INFORMATION_SYSTEM_MANAGER_HH
