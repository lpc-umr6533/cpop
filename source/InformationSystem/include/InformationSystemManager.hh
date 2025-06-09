#ifndef INFORMATION_SYSTEM_MANAGER_HH
#define INFORMATION_SYSTEM_MANAGER_HH

#include <string>
#include <QMutex>

/// \brief The informationSystemManager is the manager dealing with messages and
/// making sure they are display on there integrality
/// He is defined as a singleton
/// @author Henri Payno
class InformationSystemManager {
public:
	/// \brief Possible message Type
	/// \details According to the type of the message the display will process or not and
	/// will be display in different way ( color ... )
	enum MessageType {
		CANT_PROCESS_MES, ///< \brief used if a library can't process a request/function
		DEBUG_MES,        ///< \brief information display only on debug mode
		FATAL_ERROR_MES,  ///< \brief FATAL error, stopping the execution
		INFORMATION_MES,  ///< \brief lambda information
		WARNING_MES,      ///< \brief warning message, append if some operation are "dangerous" or unsafe.
		OTHER_MES         ///< \brief other type of message
	};

public:
	/// \brief return the singleton of the Information system manager
	static InformationSystemManager* getInstance();
	/// \brief display a message
	void Message(MessageType, std::string, std::string);

	/// \brief stop mute
	void turnOn()	{ _on = true; }
	/// \brief mute
	void turnOff() { _on = false; }
	/// \brief lock the output
	void lockOutput();
	/// \brief unlick the output
	void unlockOutput();

private:
	QMutex _printLock;  ///< \brief used to give write of display to an unick emitter ( used on lock putput/unlock output)
	bool _on = true;    ///< \brief is the display is turned off or on ?
};

#endif
