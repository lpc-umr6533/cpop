/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CUTTER_HH
#define CUTTER_HH

#include "CellSettings.hh"
#include "Direction.hh"

using namespace Settings::Geometry;
using namespace Settings::nCell;
using namespace std;


//////////////////////////////////////////////////////////////////////////////
/// \brief Cutter_3 class enable user to select some 3D spatialables by removing 3D spatialables 
/// not included on defined region.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Cutter_3
{
public:
	/// \brief constructor
	Cutter_3(vector<t_SpatialableAgent_3*>);
	Cutter_3(set<t_SpatialableAgent_3*>);
	/// \brief destructor
	~Cutter_3();

	/// \brief cut all 3D spatialables on the given direction from the given val
	void cut(Direction, double val, double bufferWidth=0.);
	/// \brief uncuted 3D spatialables getter
	vector<t_SpatialableAgent_3*> getUncutCells() const 	{ return uncutCells;}
	/// \brief uncuted 3D spatialables getter
	vector<t_SpatialableAgent_3*> getCutCells() const 	{ return cutCells;}
	/// \brief uncuted 3D spatialables getter
	vector<t_SpatialableAgent_3*> getCellsOnBuffer() const { return cellsOnBuffer;}
	/// \brief will set all cell membrane of the cell included on the buffer to the requested color
	void setColorToCellOnBuffer(CGAL::Color );
	/// \brief will set the given color to all the cell uncuted
	void setColorToUncuttedCell(CGAL::Color);
	/// \brief will set the given color to the cutted cell
	void setColorToCuttedCell(CGAL::Color);

private:
	/// \brief will cut 3D spatialables on to cut trhought the given direction at the given value.
	void processCut(Direction, double, double, vector<t_SpatialableAgent_3*>* toCut, vector<t_SpatialableAgent_3*>* buffer = NULL, vector<t_SpatialableAgent_3*>* cutted = NULL, bool cutBuffer = false );

private:
	vector<t_SpatialableAgent_3*> uncutCells;		///< \brief the cell uncuted : the one targetted by the user 
	vector<t_SpatialableAgent_3*> cutCells;			///< \brief the removed 3D spatialables
	vector<t_SpatialableAgent_3*> cellsOnBuffer;	///< \brief the cell stored on the buffer accross the different cut
};

#endif // CUTTER_HH