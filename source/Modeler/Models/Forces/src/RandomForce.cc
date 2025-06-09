#include "RandomForce.hh"
#include "RandomEngineManager.hh"
#include "CGAL_Utils.hh"

using namespace Utils::myCGAL;

template<>
Vector_2 RandomForce<double, Point_2, Vector_2>::computeForce() const {
	return _intensity* normalize(
		Vector_2(
			RandomEngineManager::getInstance()->randd(-1., 1.),
			RandomEngineManager::getInstance()->randd(-1., 1.)
		)
	);
}

template<>
Vector_3 RandomForce<double, Point_3, Vector_3>::computeForce() const {
	return _intensity* normalize(
		Vector_3(
			RandomEngineManager::getInstance()->randd(-1., 1.),
			RandomEngineManager::getInstance()->randd(-1., 1.),
			RandomEngineManager::getInstance()->randd(-1., 1.)
		)
	);
}
