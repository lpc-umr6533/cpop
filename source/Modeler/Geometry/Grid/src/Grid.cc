#include "Grid.hh"

// do not parse under doxygen
/// \cond

/// \brief 2D grid construction
template<>
void Grid<double, Point_2, Vector_2>::constructGrid() {
	assert(_gridDimensions.size() == 2);
	if(_gridDimensions.size() != 2) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES,
			"Failed to construct grid. No the good number of dimensions in gridDimension",
			"Grid"
		);
		return;
	}

	double total_X_Length = _gridDimensions[AXIS_X] * _elementTraits.getLengthOnAxis(AXIS_X);
	double total_Y_Length = _gridDimensions[AXIS_Y] * _elementTraits.getLengthOnAxis(AXIS_Y);

	// construct the grid
	// get the minimal value on x and y axis
	double xStartingPoint = _center.x() - total_X_Length/2. + _elementTraits.getLengthOnAxis(AXIS_X) / 2.;
	double yStartingPoint = _center.y() - total_Y_Length/2. + _elementTraits.getLengthOnAxis(AXIS_Y) / 2.;
	unsigned int idElmt = 0;
	for(unsigned int uiX = 0; uiX < _gridDimensions[AXIS_X]; uiX++) {
		double xcoord = xStartingPoint + uiX * _elementTraits.getLengthOnAxis(AXIS_X);
		for(unsigned int uiY = 0; uiY < _gridDimensions[AXIS_Y]; uiY++) {
			double ycoord = yStartingPoint + uiY * _elementTraits.getLengthOnAxis(AXIS_Y);
			grid.push_back(new GridElement<double, Point_2>(&_elementTraits, Point_2(xcoord, ycoord), idElmt));
			idElmt++;
		}
	}
}

/// \brief 3D grid construction
template<>
void Grid<double, Point_3, Vector_3>::constructGrid() {
	assert(_gridDimensions.size() == 3);
	if(_gridDimensions.size() != 3) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES,
			"Failed to construct grid. No the good number of dimensions in gridDimension",
			"Grid"
		);
		return;
	}

	double total_X_Length = _gridDimensions[AXIS_X] * _elementTraits.getLengthOnAxis(AXIS_X);
	double total_Y_Length = _gridDimensions[AXIS_Y] * _elementTraits.getLengthOnAxis(AXIS_Y);
	double total_Z_Length = _gridDimensions[AXIS_Z] * _elementTraits.getLengthOnAxis(AXIS_Z);

	// construct the grid
	// get the minimal value on x, y and z axis
	double xStartingPoint = _center.x() - total_X_Length/2. + _elementTraits.getLengthOnAxis(AXIS_X) / 2.;
	double yStartingPoint = _center.y() - total_Y_Length/2. + _elementTraits.getLengthOnAxis(AXIS_Y) / 2.;
	double zStartingPoint = _center.z() - total_Z_Length/2. + _elementTraits.getLengthOnAxis(AXIS_Z) / 2.;

	unsigned int idElmt = 0;
	for(unsigned int uiX = 0; uiX < _gridDimensions[AXIS_X]; uiX++) {
		double xcoord = xStartingPoint + uiX * _elementTraits.getLengthOnAxis(AXIS_X);
		for(unsigned int uiY = 0; uiY < _gridDimensions[AXIS_Y]; uiY++) {
			double ycoord = yStartingPoint + uiY * _elementTraits.getLengthOnAxis(AXIS_Y);
			for(unsigned int uiZ = 0; uiZ < _gridDimensions[AXIS_Z]; uiZ++) {
				double zcoord = zStartingPoint + uiZ * _elementTraits.getLengthOnAxis(AXIS_Z);
				grid.push_back(new GridElement<double, Point_3>(&_elementTraits, Point_3(xcoord, ycoord, zcoord), idElmt));
				idElmt++;
			}
		}
	}
}
/// \endcond
