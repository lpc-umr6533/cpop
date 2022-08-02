/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DynamicAgent.hh"
#include "SimulationManager.hh"

/// Specialization of Dynamic agent templates
// #define SIMULATION_VALID_AGENT_NEW_POS

////////////////////////////////////////////////////////////////////
/// 2D
////////////////////////////////////////////////////////////////////
template<>
void DynamicAgent<double, Point_2, Vector_2>::updatePosition(double deltaT)
{
	assert(deltaT>=0.);
	double maxThreashold = SimulationManager::getInstance()->getDisplacementThreshold();

	if(actingForce.squared_length() == 0.) 
	{
		return; 
	}
	/// make displacement
	Vector_2 movement = actingForce;

	/// check threshold
	if(movement.squared_length() > (maxThreashold*maxThreashold) )
	{
		double lLength = sqrt(movement.squared_length());
		movement = Vector_2( movement.x()/lLength*maxThreashold, movement.y()/lLength*maxThreashold);
	}
	
#ifdef SIMULATION_VALID_AGENT_NEW_POS		
	requireNewPos(K::Point_2(position + movement));
#else
	setPosition(position + movement);
	direction = movement;
	speed = sqrt(direction.squared_length());
#endif
	
	assert(CGAL::is_finite(position.x()));
	assert(CGAL::is_finite(position.y()));

	// reset forces
	Movable<double, Point_2, Vector_2>::resetForce();
}

////////////////////////////////////////////////////////////////////
/// 3D
////////////////////////////////////////////////////////////////////
template<>
void DynamicAgent<double, Point_3, Vector_3>::updatePosition(double deltaT)
{
	assert(deltaT>=0.);
	double maxThreashold = SimulationManager::getInstance()->getDisplacementThreshold();

	if(actingForce.squared_length() == 0.) 
	{
		return; 
	}
	/// make displacement
	Vector_3 movement = actingForce;

	/// check threshold
	if(movement.squared_length() > (maxThreashold*maxThreashold) )
	{
		double lLength = sqrt(movement.squared_length());
		movement = Vector_3( movement.x()/lLength*maxThreashold, movement.y()/lLength*maxThreashold, movement.z()/lLength*maxThreashold );
	}
	
#ifdef SIMULATION_VALID_AGENT_NEW_POS		
	requireNewPos(K::Point_3(position + movement));
#else
	setPosition(position + movement);
	direction = movement;
	speed = sqrt(direction.squared_length());
#endif
	
	assert(CGAL::is_finite(position.x()));
	assert(CGAL::is_finite(position.y()));
	assert(CGAL::is_finite(position.z()));

	// reset forces
	Movable<double, Point_3, Vector_3>::resetForce();
}
