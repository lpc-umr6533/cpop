/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef DISCOIDAL_CELL_MESH_SUB_THREAD_HH
#define DISCOIDAL_CELL_MESH_SUB_THREAD_HH

#include "Voronoi2DCellMeshSubThread.hh"
#include <QThread>

using namespace Settings::Geometry::Mesh2D;

//////////////////////////////////////////////////////////////////////////////
/// \brief The thread to refine discoid cell from voronoi cell.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class DiscoidalCellMeshSubThread : public Voronoi2DCellMeshSubThread
{
	Q_OBJECT
public:
	/// \brief constructor
	DiscoidalCellMeshSubThread(
		unsigned int pID, 
		unsigned int pMaxNbSeg, 
		double pDeltaWin,
		const std::map<DiscoidalCell*, std::set<const DiscoidalCell*> >*,
		double pMaxRatioToCellRadius,
		std::vector<DiscoidalCell*> cellsToReffine = std::vector<DiscoidalCell*>());
	virtual ~DiscoidalCellMeshSubThread();

	/// \brief main function called to reffine a specific cell
	virtual bool reffineCell(DiscoidalCell* cell);
protected:
	/// \brief generate the nucleus radius
	bool generateNucleusRadius(DiscoidalCell* cell);
	
	

};

#endif // DISCOIDAL_CELL_MESH_SUB_THREAD_HH