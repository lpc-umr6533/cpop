#ifndef DYNAMIC_AGENT_HH
#define DYNAMIC_AGENT_HH

#include "Movable.hh"
#include "Scheduler.hh"
#include "SpatialableAgent.hh"
#include "GeometrySettings.hh"

#include <InformationSystemManager.hh>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <QString>

#define DEBUG_DYNAMIC_AGENT 0

using namespace Settings::Geometry;

/// \brief The agent class : define the structure and the possible call agent can make
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class DynamicAgent: public SpatialableAgent<Kernel, Point, Vector>, public Movable<Kernel, Point, Vector> {
public:
	DynamicAgent(Body* pBody, Point p = Point(), Vector v = Vector());

	/// \brief function called at each iteration of the simulation
	int exec() override;
	/// \brief drawing the dynamic agent
	void draw() const override;
	/// \brief function called before each agent alaunch
	int start() override;
	/// \brief action called when stopping the agent
	int stop() override;
	/// \brief comppute the new requested agent position based on his speed and his direction
	void updatePosition(double) override;

	/// \brief return the requested position of the agent for next step
	[[nodiscard]] Point getRequestedPosition() const { return _requiredNewPos; }   ///< \brief required position getter
	/// \brief called to validate the new agent position.
	void validRequiredPos() {
		Spatialable<Kernel, Point, Vector>::position = _requiredNewPos;
		_bIsReqNewPos = false;
	}
	/// \brief called when the agent require a new position
	void setIsRequiringNewPos(bool b) { _bIsReqNewPos = b; }
	/// \brief true when the agent is requiring a new position
	[[nodiscard]] bool isRequiringNewPos() const { return _bIsReqNewPos; }

private:
	/// \brief called when we want to set a new position for the agent
	void requireNewPos(Point p) {
		_requiredNewPos = p;
		_bIsReqNewPos = true;
	}

protected:
	bool _bIsReqNewPos;			///< \brief True if the agent request a new position for the next step
	Point _requiredNewPos;		///< \brief the new required position
};

//////////////////// FUNCTION DEFINITIONS ///////////////////////////////////

template<typename Kernel, typename Point, typename Vector>
DynamicAgent<Kernel, Point, Vector>::DynamicAgent(Body* pBody, Point pPosition, Vector pOrientation):
	SpatialableAgent<Kernel, Point, Vector>(pBody, pPosition, pOrientation),
	Movable<Kernel, Point, Vector>(Vector(), Kernel(), Vector())
{
}

/// \brief called at each step of the simulation
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::exec() {
	/// call the movable update position
	updatePosition(Scheduler::getInstance()->getStepDuration());
	return 0;
}

/// \brief called when we want to set the agent in pause
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::stop() {
	return Agent::stop();
}

/// \brief called before launching the agent on the simulation
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::start() {
	return Agent::start();
}

/// \brief function drawing the agent according to his body and his
template<typename Kernel, typename Point, typename Vector>
void DynamicAgent<Kernel, Point, Vector>::draw() const {
	SpatialableAgent<Kernel, Point, Vector>::draw();
}

/// \brief Will compute the new position from the speed and the deltaT information
/// \param deltaT The duration during steps, in sec
template<typename Kernel, typename Point, typename Vector>
void DynamicAgent<Kernel, Point, Vector>::updatePosition(double deltaT) {
	QString mess = "Failed to update the dynamic agent " + QString::number(Agent::getID()) + " dynamic agent not implemented for this templates parameters";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Dynamic agent");
}

////////// Update position specialization ///////

/// 2D
template<>
void DynamicAgent<double, Point_2, Vector_2>::updatePosition(double deltaT);

/// 3D
template<>
void DynamicAgent<double, Point_3, Vector_3>::updatePosition(double deltaT);

////////// End Update position specialization ///////

#endif
