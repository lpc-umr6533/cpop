/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
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
/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The agent class : define the structure and the possible call agent can make
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class DynamicAgent: public SpatialableAgent<Kernel, Point, Vector>, public Movable<Kernel, Point, Vector>
{

public:
	/// \brief constructor
	DynamicAgent(Body* pBody, Point p = Point(), Vector v = Vector());
	/// \brief destructor
	virtual ~DynamicAgent();

	/// \brief function called at each iteration of the simulation
	virtual int exec();
	/// \brief drawing the dynamic agent
	virtual void draw() const;
	/// \brief function called before each agent alaunch
	virtual int start();
	/// \brief action called when stopping the agent
	virtual int stop();
	/// \brief agent initailization
	virtual int init() = 0;	
	/// \brief comppute the new requested agent position based on his speed and his direction
	void updatePosition(double);

	/// \brief return the requested position of the agent for next step
	Point getRequestedPosition() const 		{ return requiredNewPos;};		///< \brief required position getter
	/// \brief called to validate the new agent position.
	void validRequiredPos()					{ Spatialable<Kernel, Point, Vector>::position = requiredNewPos; bIsReqNewPos = false;};	
	/// \brief called when the agent require a new position
	void setIsRequiringNewPos(bool b) 		{ bIsReqNewPos = b;};	
	/// \brief true when the agent is requiring a new position
	bool isRequiringNewPos() const			{ return bIsReqNewPos;};

private:
	/// \brief called when we want to set a new position for the agent
	void requireNewPos(Point p)				{ requiredNewPos = p; bIsReqNewPos = true;};

protected:
	bool bIsReqNewPos;			///< \brief True if the agent request a new position for the next step
	Point requiredNewPos;		///< \brief the new required position
};

//////////////////// FUNCTION DEFINITIONS ///////////////////////////////////

//////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
DynamicAgent<Kernel, Point, Vector>::DynamicAgent(Body* pBody, Point pPosition, Vector pOrientation):
	SpatialableAgent<Kernel, Point, Vector>(pBody, pPosition, pOrientation), 
	Movable<Kernel, Point, Vector>(Vector(), Kernel(), Vector())
{

}

//////////////////////////////////////////////////////////////////
/// \brief destructor
//////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
DynamicAgent<Kernel, Point, Vector>::~DynamicAgent()
{

}

//////////////////////////////////////////////////////////////////
/// \brief called at each step of the simulation
//////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::exec()
{
	/// call the movable update position
	updatePosition(Scheduler::getInstance()->getStepDuration());
	return 0;
}

//////////////////////////////////////////////////////////////////
/// \brief called when we want to set the agent in pause
//////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::stop()
{
	return Agent::stop();
}

//////////////////////////////////////////////////////////////////
/// \brief called before launching the agent on the simulation
//////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
int DynamicAgent<Kernel, Point, Vector>::start()
{
	return Agent::start();
}

/////////////////////////////////////////////////////////////////////
/// \brief function drawing the agent according to his body and his 
/// position
/////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
void DynamicAgent<Kernel, Point, Vector>::draw() const
{
	SpatialableAgent<Kernel, Point, Vector>::draw();	
}

////////////////////////////////////////////////////////////////////
/// \brief Will compute the new position from the speed and the deltaT information
/// \param deltaT The duration during steps, in sec
////////////////////////////////////////////////////////////////////
/// standard
template<typename Kernel, typename Point, typename Vector>
void DynamicAgent<Kernel, Point, Vector>::updatePosition(double deltaT)
{
	QString mess = "Failed to update the dynamic agent " + QString::number(Agent::getID()) + " dynamic agent not implemented for this templates parameters";
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Dynamic agent");
}

////////// Update position specialization ///////

////////// Update position specialization ///////

/// 2D
template<>
void DynamicAgent<double, Point_2, Vector_2>::updatePosition(double deltaT);

/// 3D
template<>
void DynamicAgent<double, Point_3, Vector_3>::updatePosition(double deltaT);

////////// End Update position specialization ///////

#endif // DYNAMIC_AGENT_HH
