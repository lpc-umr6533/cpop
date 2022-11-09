/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPATIALABLE_AGENT_HH
#define SPATIALABLE_AGENT_HH

#include "Agent.hh"
#include "BoundingBox.hh"
#include "GeometrySettings.hh"
#include "Spatialable.hh"

#include <cstddef>

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#include <windows.h>
#endif

#include <GL/gl.h>

using namespace Settings::Geometry;

//////////////////////////////////////////////////////////////////////////////
/// \brief The agent class : define a spatialable agent
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class SpatialableAgent : public Agent, public Spatialable<Kernel, Point, Vector>
{
	friend class ConflictSolver;
	
public:
	/////////////////////////////////////////
	/// \brief constructor
	/// \param pAnimat the animation/ behavior of the Agent
	/// \param pBody the body ( physical entity ) of the agent
	/// \param pPosition The intiial position of the agent
	/// \param pOrientation The initial orientation of the agent
	/////////////////////////////////////////
	SpatialableAgent(Body* pBody, Point pPosition = Point(), Vector pOrientation = Vector()):
		Agent(pBody), 
		Spatialable<Kernel, Point, Vector>(pPosition, pOrientation)
	{

	}
	
	/////////////////////////////////////////
	/// \brief destructor
	/////////////////////////////////////////
	virtual ~SpatialableAgent()
	{

	}

	/// \brief The function called to draw the agent
	virtual void draw() const;
	
	/// \brief the process called at each simulation iteration
	inline virtual int exec() 			{ return 0;};	
	/// \brief the process called when launching the agent on the simulation
	virtual int start()
	{
		return Agent::start();
	}

	/// \brief the function called when we want to set the agent in pause	
	virtual int stop()
	{
		return Agent::stop();
	}

	/// \brief function called before launching the agent on the simulation
	virtual int init() { return 0;};	

	/// \brief return true if the spatialable cross the bounding box
	virtual bool cross(const BoundingBox<Point>*) const;
	/// \brief return true if the point is in the bounding box
	virtual bool isIn(const BoundingBox<Point>*) const;

};

//////////////////////////// DRAW FUNCTION ////////////////////////
template <typename Kernel, typename Point, typename Vector>
void SpatialableAgent<Kernel, Point, Vector>::draw() const
{

}

//// 2D
template<>
void SpatialableAgent<double, Point_2, Vector_2>::draw() const;

/// 3D
template<>
void SpatialableAgent<double, Point_3, Vector_3>::draw() const;

///////////////////////////////////////////////////////////////////////
// cross
template <typename Kernel, typename Point, typename Vector>
bool SpatialableAgent<Kernel, Point, Vector>::cross(const BoundingBox<Point>*) const
{
	return false;
}

template<>
bool SpatialableAgent<double, Point_2, Vector_2>::cross(const BoundingBox<Point_2>*) const;

template<>
bool SpatialableAgent<double, Point_3, Vector_3>::cross(const BoundingBox<Point_3>*) const;

/////////////////////////////////////////////////////////////////////////
// isIn
template <typename Kernel, typename Point, typename Vector>
bool SpatialableAgent<Kernel, Point, Vector>::isIn(const BoundingBox<Point>*) const
{
	return false;
}

template<>
bool SpatialableAgent<double, Point_2, Vector_2>::isIn(const BoundingBox<Point_2>*) const;

template<>
bool SpatialableAgent<double, Point_3, Vector_3>::isIn(const BoundingBox<Point_3>*) const;


#endif