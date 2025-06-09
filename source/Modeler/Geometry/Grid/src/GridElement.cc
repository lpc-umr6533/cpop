#include "GridElement.hh"
#include "RandomEngineManager.hh"

/// \return A random position inside the GridElement
template<>
Point_2 GridElement<double, Point_2>::getRandomPos() const {
	assert(_traits);
	return {
		RandomEngineManager::getInstance()->randd( _center.x() - _traits->getLengthOnAxis(AXIS_X) / 2., _center.x() + _traits->getLengthOnAxis(AXIS_X) / 2.),
		RandomEngineManager::getInstance()->randd( _center.y() - _traits->getLengthOnAxis(AXIS_Y) / 2., _center.y() + _traits->getLengthOnAxis(AXIS_Y) / 2.)
	};
}

/// \return A random position inside the GridElement
template<>
Point_3 GridElement<double, Point_3>::getRandomPos() const {
	assert(_traits);
	return {
		RandomEngineManager::getInstance()->randd( _center.x() - _traits->getLengthOnAxis(AXIS_X) / 2., _center.x() + _traits->getLengthOnAxis(AXIS_X) / 2.),
		RandomEngineManager::getInstance()->randd( _center.y() - _traits->getLengthOnAxis(AXIS_Y) / 2., _center.y() + _traits->getLengthOnAxis(AXIS_Y) / 2.),
		RandomEngineManager::getInstance()->randd( _center.z() - _traits->getLengthOnAxis(AXIS_Z) / 2., _center.z() + _traits->getLengthOnAxis(AXIS_Z) / 2.)
	};
}
