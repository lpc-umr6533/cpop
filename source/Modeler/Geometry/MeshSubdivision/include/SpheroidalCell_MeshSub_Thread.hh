/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef SPHEROIDAL_CELL_MESH_SUBDIVION_THREAD_HH
#define SPHEROIDAL_CELL_MESH_SUBDIVION_THREAD_HH

#include "Voronoi3DCellMeshSubThread.hh"

#include <map>
#include <vector>

#include <QThread>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;

//////////////////////////////////////////////////////////////////////////////
/// \brief The thread to refine spheroid cell from voronoi cell.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class SpheroidalCellMeshSubThread : public Voronoi3DCellMeshSubThread
{
	Q_OBJECT

public:
	/// \brief constructor
	SpheroidalCellMeshSubThread(
		unsigned int, 
		unsigned int, 
		double, 
		const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >*,
		double pMaxRatioToCellRadius,
		std::vector<SpheroidalCell*> cellsToReffine = std::vector<SpheroidalCell*>(),
		double pSpaceBetweenCells = 0);
	/// \brief destructor
	virtual ~SpheroidalCellMeshSubThread();
	
	/// \brief main function called to reffine a specific cell
	virtual bool reffineCell(SpheroidalCell* cell);
	
protected:
	/// \brief generate the nucleus radius
	bool generateNucleusRadius(SpheroidalCell* cell);
};

#endif // SPHEROIDAL_CELL_MESH_THREAD_HH