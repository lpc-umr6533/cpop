/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Grid_Utils.hh"
#include "RoundCellProperties.hh"

namespace Utils
{
	namespace nGrid
	{
		/// \param CellProperties the properties of the cell we want the grid for
		/// \param simulatedSubEnv the environment the cell are in and for we want the grid
		/// \param double Optional gridWidth.
		/// \return t_Grif_3* The grid we have compute according toparameters given
		Settings::nGrid::t_Grid_3* getOptimalGrid(const CellProperties* pCellProp, const Settings::nEnvironment::t_SimulatedSubEnv_3* pSimuSubEnv, double pGridWidth)
		{

			G4cout << "\n\n\n Entrée dans Grid_Utils \n\n\n" << G4endl;

			// define grid element size
			double gridElementXLength = 1.;
			double gridElementYLength = 1.;
			double gridElementZLength = 1.;
			// if a size is given set this size
			if(pGridWidth>0.)
			{
				gridElementXLength = pGridWidth;
				gridElementYLength = pGridWidth;
				gridElementZLength = pGridWidth;
			}else
			{
				const RoundCellProperties* lRoundCellProp = dynamic_cast<const RoundCellProperties*>(pCellProp);
				// else : try to find the better size from grid properties
				if( lRoundCellProp )
				{
					// magical formula : length if mean value * 5/6
					double targettedRadius = (lRoundCellProp->getMaxRadius() + lRoundCellProp->getMinRadius())/2. *5./6.;
					gridElementXLength = 2.*targettedRadius;	// *2 to go from radius to diameter
					gridElementYLength = 2.*targettedRadius;	// *2 to go from radius to diameter
					gridElementZLength = 2.*targettedRadius;	// *2 to go from radius to diameter
				}
			}
			vector<double> elementSize;
			elementSize.push_back(gridElementXLength);
			elementSize.push_back(gridElementYLength);
			elementSize.push_back(gridElementZLength);

			// define the grid sizes in the 3 dimensions :
			Point_3 origin = pSimuSubEnv->getSpatialDelimitation()->getOrigin();
			BoundingBox<Point_3> bb = pSimuSubEnv->getSpatialDelimitation()->getBoundingBox();

			double grid_X_size = bb.getTopRight().x() - bb.getBottomLeft().x();
			double grid_Y_size = bb.getTopRight().y() - bb.getBottomLeft().y();
			double grid_Z_size = bb.getTopRight().z() - bb.getBottomLeft().z();

			vector<unsigned int> gridSize;
			gridSize.push_back(grid_X_size / gridElementXLength);
			gridSize.push_back(grid_Y_size / gridElementYLength);
			gridSize.push_back(grid_Z_size / gridElementZLength);

			return new Settings::nGrid::t_Grid_3(
				origin,
				Settings::nGrid::t_GridElementTraits(elementSize),
				gridSize);
		}
	}
}
