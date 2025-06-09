#include "ForceFactory.hh"

static ForceFactory* factory = nullptr;

using namespace Settings::nForce;

/// \return <ForceFactory*> {The singleton of the factory.}
ForceFactory* ForceFactory::getInstance() {
	if(!factory)
		factory = new ForceFactory();
	return factory;
}

/// \brief return the force targetted
t_Force_2* ForceFactory::computeForce2D(ForceType, Settings::nCell::t_Cell_2*) {
	return nullptr;
}

/// \brief return the force targetted
t_Force_3* ForceFactory::computeForce3D(ForceType, Settings::nCell::t_Cell_3*) {
	return nullptr;
}
