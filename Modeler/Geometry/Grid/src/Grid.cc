/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Grid.hh"
// do not parse under doxygen
/// \cond

//////////////////////////////////////////////////////////////////////::
//		construct grid 2D
/// \brief 2D grid construction
//////////////////////////////////////////////////////////////////////::
template<>
void Grid<double, Point_2, Vector_2>::constructGrid()	
{
	assert(gridDimensions.size() == 2);
	if(gridDimensions.size() != 2)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, 
			"Failed to construct grid. No the good number of dimensions in gridDimension", 
			"Grid");
		return;
	}

	double total_X_Length = gridDimensions[AXIS_X] * elementTraits.getLengthOnAxis(AXIS_X);
	double total_Y_Length = gridDimensions[AXIS_Y] * elementTraits.getLengthOnAxis(AXIS_Y);

	// construct the grid
	// get the minimal value on x and y axis
	double xStartingPoint = center.x() - total_X_Length/2. + elementTraits.getLengthOnAxis(AXIS_X) / 2.;
	double yStartingPoint = center.y() - total_Y_Length/2. + elementTraits.getLengthOnAxis(AXIS_Y) / 2.;
	unsigned int idElmt = 0;
	for(unsigned int uiX = 0; uiX < gridDimensions[AXIS_X]; uiX++)
	{
		double xcoord = xStartingPoint + uiX * elementTraits.getLengthOnAxis(AXIS_X);
		for(unsigned int uiY = 0; uiY < gridDimensions[AXIS_Y]; uiY++)
		{
			double ycoord = yStartingPoint + uiY * elementTraits.getLengthOnAxis(AXIS_Y);
			grid.push_back(new GridElement<double, Point_2>(&elementTraits, Point_2(xcoord, ycoord), idElmt));
			idElmt++;
		}
	}
}

////////////////////////////////////////////////////////////////////////
//		construct grid 3D
/// \brief 3D grid construction
////////////////////////////////////////////////////////////////////////
template<>
void Grid<double, Point_3, Vector_3>::constructGrid()
{
	assert(gridDimensions.size() == 3);
	if(gridDimensions.size() != 3)
	{
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, 
			"Failed to construct grid. No the good number of dimensions in gridDimension", 
			"Grid");
		return;
	}

	double total_X_Length = gridDimensions[AXIS_X] * elementTraits.getLengthOnAxis(AXIS_X);
	double total_Y_Length = gridDimensions[AXIS_Y] * elementTraits.getLengthOnAxis(AXIS_Y);
	double total_Z_Length = gridDimensions[AXIS_Z] * elementTraits.getLengthOnAxis(AXIS_Z);

	// construct the grid
	// get the minimal value on x, y and z axis
	double xStartingPoint = center.x() - total_X_Length/2. + elementTraits.getLengthOnAxis(AXIS_X) / 2.;
	double yStartingPoint = center.y() - total_Y_Length/2. + elementTraits.getLengthOnAxis(AXIS_Y) / 2.;
	double zStartingPoint = center.z() - total_Z_Length/2. + elementTraits.getLengthOnAxis(AXIS_Z) / 2.;
	
	unsigned int idElmt = 0;
	for(unsigned int uiX = 0; uiX < gridDimensions[AXIS_X]; uiX++)
	{
		double xcoord = xStartingPoint + uiX * elementTraits.getLengthOnAxis(AXIS_X);
		for(unsigned int uiY = 0; uiY < gridDimensions[AXIS_Y]; uiY++)
		{
			double ycoord = yStartingPoint + uiY * elementTraits.getLengthOnAxis(AXIS_Y);
			for(unsigned int uiZ = 0; uiZ < gridDimensions[AXIS_Z]; uiZ++ )
			{
				double zcoord = zStartingPoint + uiZ * elementTraits.getLengthOnAxis(AXIS_Z);
				grid.push_back(new GridElement<double, Point_3>(&elementTraits, Point_3(xcoord, ycoord, zcoord), idElmt));
				idElmt++;
			}
		}
	}
}
/// \endcond