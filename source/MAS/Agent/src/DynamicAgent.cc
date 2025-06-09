#include "DynamicAgent.hh"
#include "SimulationManager.hh"

/// Specialization of Dynamic agent templates
// #define SIMULATION_VALID_AGENT_NEW_POS

/// 2D
template<>
void DynamicAgent<double, Point_2, Vector_2>::updatePosition(double deltaT) {
	assert(deltaT>=0.);
	double maxThreashold = SimulationManager::getInstance()->getDisplacementThreshold();

	if(_actingForce.squared_length() == 0.) return;

	/// make displacement
	Vector_2 movement = _actingForce;

	/// check threshold
	if(movement.squared_length() > (maxThreashold*maxThreashold)) {
		double lLength = sqrt(movement.squared_length());
		movement = Vector_2( movement.x()/lLength*maxThreashold, movement.y()/lLength*maxThreashold);
	}

#ifdef SIMULATION_VALID_AGENT_NEW_POS
	requireNewPos(K::Point_2(position + movement));
#else
	setPosition(_position + movement);
	_direction = movement;
	_speed = sqrt(_direction.squared_length());
#endif

	assert(CGAL::is_finite(_position.x()));
	assert(CGAL::is_finite(_position.y()));

	// reset forces
	Movable<double, Point_2, Vector_2>::resetForce();
}

/// 3D
template<>
void DynamicAgent<double, Point_3, Vector_3>::updatePosition(double deltaT) {
	assert(deltaT>=0.);
	double maxThreashold = SimulationManager::getInstance()->getDisplacementThreshold();

	if(_actingForce.squared_length() == 0.) return;

	/// make displacement
	Vector_3 movement = _actingForce;

	/// check threshold
	if(movement.squared_length() > (maxThreashold*maxThreashold)) {
		double lLength = sqrt(movement.squared_length());
		movement = Vector_3( movement.x()/lLength*maxThreashold, movement.y()/lLength*maxThreashold, movement.z()/lLength*maxThreashold );
	}

#ifdef SIMULATION_VALID_AGENT_NEW_POS
	requireNewPos(K::Point_3(position + movement));
#else
	setPosition(_position + movement);
	_direction = movement;
	_speed = sqrt(_direction.squared_length());
#endif

	assert(CGAL::is_finite(_position.x()));
	assert(CGAL::is_finite(_position.y()));
	assert(CGAL::is_finite(_position.z()));

	// reset forces
	Movable<double, Point_3, Vector_3>::resetForce();
}
