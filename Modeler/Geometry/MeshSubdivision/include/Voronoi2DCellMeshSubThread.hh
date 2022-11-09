/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef VORONOI_2D_CELL_MESH_SUBDIVION_THREAD_HH
#define VORONOI_2D_CELL_MESH_SUBDIVION_THREAD_HH

#include "DiscoidalCell.hh"
#include "RefinementThread.hh"
#include "CPOP_Segment_2.hh"
#include "Mesh2DSettings.hh"

#include <map>
#include <vector>
#include <QThread>


using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh2D;

//////////////////////////////////////////////////////////////////////////////
/// \brief The thread to refine spheroid cell from voronoi cell.
/// ideally to SpheroidalCell must be Sphere_3. But to win dev time we heritate from 
/// SpheroidalCellMeshSubThread and do the same action but without nucleus.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class Voronoi2DCellMeshSubThread : public RefinementThread
{
	Q_OBJECT

public:
	/// \brief constructor
	Voronoi2DCellMeshSubThread(unsigned int, unsigned int, double, 
		const std::map<DiscoidalCell*, std::set<const DiscoidalCell*> >*,					
		std::vector<DiscoidalCell*> cellsToReffine = std::vector<DiscoidalCell*>());
	/// \brief destructor
	virtual ~Voronoi2DCellMeshSubThread();
	
	/// \brief add a spheroidal cell struct to reffine
	void addCell(DiscoidalCell*);
	/// \brief run of the thread.
	void run();
	/// \brief main function called to reffine a specific cell
	virtual bool reffineCell(DiscoidalCell* cell);

protected:
	/// \brief generate the intersections planes for the given cell
	bool generateIntersectionLine(DiscoidalCell* cell);
	/// \brief gnerate the smallest membrane mesh possible for the cell
	bool generateBasicCellMembraneMesh(DiscoidalCell* cell);
	/// \brief subdivided the membrane mesh according to the delta and maxNumberFacet parameters.
	bool subdivseCellMembraneMesh(DiscoidalCell* cell);
	/// \brief clean data structure and pointer after process
	void clean();	
	/// \brief check if both points are on the same intersection line.
	bool ptsAreOnInterLine(Point_2 pt1, Point_2 pt2);
	/// \brief update point accoridng to intersection line
	void adjustPointFromIntersections(Point_2& ptToCheck, DiscoidalCell* cell);
	/// \brief check points from a list of intersections
	void adjustPointFromIntersections(Point_2& ptToCheck, std::vector<const Line_2*>* intersections, DiscoidalCell* );
	/// \brief return the point to ad for the given segment
	Point_2 getPointToAdd(CPOP_Segment_2 seg, Point_2 circleCenter, double circleRadius);

protected:
	std::vector<DiscoidalCell*> cellsToReffine;											///< \brief the list of cell to process
	const std::map<DiscoidalCell*, std::set<const DiscoidalCell* > >* neighbourCells;	///< \brief the list of neighbour and there weight.
	
	std::vector<Line_2*> intersections; 												///< \brief computed intersections for the current cell the thread is working on
	std::map<Line_2*, int> intersectionIDMap;											///< \brief map from intersection to intersection ID
};

#endif // SPHEROIDAL_2D_CELL_MESH_THREAD_HH