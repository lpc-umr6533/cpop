#include <utility>

#include "Voronoi2DCellMeshSubThread.hh"
#include "CPOP_Segment_2.hh"
#include "Geometry_Utils_Circle.hh"

/// \param pID The ID of the thread to reffine.
/// \param pMaximalNumberOfFacets When this value is reach we will stop facet subdivision
/// \param pDeltaWin The minimal subdivision win we are interesting to subdivide for
/// \param pNeighbourCells The map of all the neighbouring.
///< Warning pNeighbourgCells must stay a read only struct for threads
/// \param pCellsToReffine The set of cell the thread must execute
Voronoi2DCellMeshSubThread::Voronoi2DCellMeshSubThread(
	unsigned int pID,
	unsigned int pMaximalNumberOfFacets,
	double pDeltaWin,
	const std::map<DiscoidalCell*, std::set<const DiscoidalCell*> >* pNeighbourCells,
	std::vector<DiscoidalCell*> pCellsToReffine
):
	RefinementThread(pID, pMaximalNumberOfFacets, pDeltaWin),
	cellsToReffine(std::move(pCellsToReffine)),
	neighbourCells(pNeighbourCells)
{
}

/// \param cell The cell to reffine
/// \return true if success
bool Voronoi2DCellMeshSubThread::reffineCell(DiscoidalCell* cell) {
	assert(cell);
	/// warning the function called here are link one to another so do no change the order of execution
	{
		clean();
		if(!generateIntersectionLine(cell)) 		return false;
		if(!generateBasicCellMembraneMesh(cell))	return false;
		// if(!subdivseCellMembraneMesh(cell))			return false;
	}
	return true;
}

/// \param cell The cell to add to the list of cell to refine
void Voronoi2DCellMeshSubThread::addCell(DiscoidalCell* cell) {
	assert(cell);
	cellsToReffine.push_back(cell);
}

void Voronoi2DCellMeshSubThread::run() {
	// bool sucess = true;
	/// TODO : send error if not a sucess
	for(auto const& cell : cellsToReffine) {
		if(!reffineCell(cell))
			return;
	}
}

void Voronoi2DCellMeshSubThread::clean() {
	intersectionIDMap.clear();
	// delete intersection
	for(auto* intersection : intersections)
		delete intersection;
	intersections.clear();
}

///	\param cell The cell to reffine.
///	\return True if sucess.
bool Voronoi2DCellMeshSubThread::generateBasicCellMembraneMesh(DiscoidalCell* cell) {
	assert(cell);

	/// TODO : remove RefinementThread::numberOfUnitaryEntityPerCell
	std::vector<Point_2> polyInitPts = Utils::Geometry::Circle::getNPointOnCircle(cell->getOrigin(), cell->getRadius(), RefinementThread::_numberOfUnitaryEntityPerCell);	// TODO : MIN_PT
	// update standard point from intersections
	for(unsigned int iPt = 0; iPt < polyInitPts.size(); iPt++) {
		adjustPointFromIntersections(polyInitPts[iPt], cell);
		// remove the point if undef. CGAL issue ?
		if(is_nan(polyInitPts[iPt].x()) || is_nan(polyInitPts[iPt].y())) {
			polyInitPts.erase(polyInitPts.begin() + iPt);
			--iPt;
		}
	}

	// create corresponding segment
	for(unsigned int iPt = 0; iPt < polyInitPts.size(); iPt++) {
		unsigned int iPt2 = iPt + 1;
		if(iPt2 >= polyInitPts.size()) iPt2 = 0;

		cell->add_to_shape(Segment_2(polyInitPts[iPt], polyInitPts[iPt2]));
	}

	return true;
}

/// \param ptToCheck the point to check
/// \param intersections the intersections relative to the given cell
/// \param cell The cell containing the mesh we are working in
void Voronoi2DCellMeshSubThread::adjustPointFromIntersections(Point_2& ptToCheck, std::vector<const Line_2*>* lIntersections, DiscoidalCell* cell) {
	for(unsigned int iLine = 0; iLine < lIntersections->size(); iLine++) {
		assert(lIntersections->at(iLine));
		// if the point is outside the shape
		if(lIntersections->at(iLine)->oriented_side(ptToCheck) == CGAL::ON_NEGATIVE_SIDE) {
			// project it on the intersection line
			Point_2 newPt = lIntersections->at(iLine)->projection(ptToCheck);
			// remove the point if undef. CGAL issue ?
			if( (!is_nan(newPt.x())) || (!is_nan(newPt.y()))) {
				// TODO optimisation
				if(sqrt(CGAL::squared_distance(newPt, cell->getOrigin() )) < sqrt(CGAL::squared_distance(ptToCheck, cell->getOrigin())))
					ptToCheck = newPt;
			}

			lIntersections->erase(lIntersections->begin() + iLine);
			adjustPointFromIntersections(ptToCheck, lIntersections, cell);
			return;
		}
	}
}

/// \param ptToCheck The point to adjust.
/// \param cell The cell containing the mesh we are working in
void Voronoi2DCellMeshSubThread::adjustPointFromIntersections(Point_2& ptToCheck, DiscoidalCell* cell) {
	/// \warning : must copy the vector of intersection because modify by the funciton
	std::vector<const Line_2*> lIntersections;
	for(auto const& intersection : intersections) // TODO const_cast ==?
		lIntersections.push_back(const_cast<const Line_2*>(intersection));
	return adjustPointFromIntersections(ptToCheck, static_cast<std::vector<const Line_2*>*> ( &lIntersections), cell);
}

/// \param seg The segment part of the discoidal cell mesh to reffine
/// \param circleCenter The center of discoidal cell mesh
/// \param circleRadius The theorical radius of the cell mesh.
/// \return The point to add
Point_2 Voronoi2DCellMeshSubThread::getPointToAdd(CPOP_Segment_2 seg, Point_2 circleCenter, double circleRadius) {
	double x = seg.getSegment().source().x() + seg.getSegment().target().x();
	double y = seg.getSegment().source().y() + seg.getSegment().target().y();
	Point_2 mediumPt(x / 2., y / 2.);
	mediumPt = Utils::Geometry::Circle::getProjectionOnCircle(mediumPt, circleCenter, circleRadius);

	/// TODO : to reset
	// adjustPointFromIntersections(mediumPt);
	return mediumPt;
}

/// \param pt1 The first point to check
/// \param pt2 The second point to check
/// \return true if both point are on the same intersection line.
bool Voronoi2DCellMeshSubThread::ptsAreOnInterLine(Point_2, Point_2) {
	return true;
	// return ((ptsOnLineInter[pt1] == ptsOnLineInter[pt2]) &&	// both on the same
	// 		(ptsOnLineInter[pt1] > -1 ) );					// and are in an intersection plane
}

/// \param cell The cell containing the mesh we are working in
///	\return True if sucess.
bool Voronoi2DCellMeshSubThread::subdivseCellMembraneMesh(DiscoidalCell* cell) {
	assert(cell);
	if(!generateBasicCellMembraneMesh(cell))
		return false;

	// TODO : redo - but not called for now because we are directly picking the number of point we want to have

	// std::set<CPOP_Segment_2> segs;
	// std::vector<Segment_2>::iterator itSeg;
	// for(itSeg = cell->shape.begin(); itSeg != cell->shape.end(); ++itSeg )
	// {
	// 	segs.insert(CPOP_Segment_2(*itSeg, !ptsAreOnInterLine(itSeg->source(), itSeg->target() )));
	// }

	// /// generate the stack of segments
	// while( 	(segs.size() < RefinementThread::numberOfUnitaryEntityPerCell ) &&
	// 		(segs.begin()->getPotentialWin() < deltaReffinement) )
	// {
	// 	CPOP_Segment_2 seg = *segs.begin();
	// 	segs.erase(segs.begin());

	// 	Point_2 pt = getPointToAdd(seg, cell->origin, cell->radius);
	// 	segs.insert( 	CPOP_Segment_2( Segment_2(seg.source(), pt),
	// 					!ptsAreOnInterLine(seg.source(), pt) )
	// 				);

	// 	segs.insert( 	CPOP_Segment_2( Segment_2(pt, seg.target()),
	// 				 	!ptsAreOnInterLine(pt, seg.target()) )
	// 				 );
	// }

	/// TODO : compute area error
	// formule arc de cercle http://www.utc.fr/~tthomass/Themes/Unites/unites/infos/aires_volumes/Surfaces%20et%20volumes.pdf
	return true;
}

/// \param cell The cell containing the mesh we are working in
///	\return True if sucess.
bool Voronoi2DCellMeshSubThread::generateIntersectionLine(DiscoidalCell* cell) {
	assert(cell);
	assert(neighbourCells->find(cell) != neighbourCells->end());
	int intersectionID = 0;

	std::set<const DiscoidalCell*> cellNeighbour = neighbourCells->find(cell)->second;
	std::vector<Point_2> polyInitPts;

	std::set<const DiscoidalCell*>::iterator itNeighbour;
	for(itNeighbour = cellNeighbour.begin(); itNeighbour != cellNeighbour.end(); ++itNeighbour) {
		// TODO : remove sqrt
		double distance = sqrt(squared_distance((*itNeighbour)->getOrigin(), cell->getOrigin())) - cell->getRadius() - (*itNeighbour)->getRadius();
		// if a point intersection
		if(distance == 0) {
			polyInitPts.push_back(
				Utils::Geometry::Circle::getProjectionOnCircle((*itNeighbour)->getOrigin(), cell->getOrigin(), cell->getRadius())
			);
		}

		// if a plane intersection
		if(distance < 0.) {
			// no need to calculate the sphere, we didn't need it.
			Line_2 interLine = K::Construct_radical_axis_2()(
				Weighted_point_2( (*itNeighbour)->getOrigin(), (*itNeighbour)->getSquareRadius()),
				Weighted_point_2( cell->getOrigin(), cell->getSquareRadius())
			);

			Line_2* line = nullptr;
			if(interLine.oriented_side(cell->getOrigin()) == CGAL::ON_NEGATIVE_SIDE) {
				line = new Line_2(interLine.opposite());
			} else {
				line = new Line_2(interLine);
			}

			intersections.push_back( line );
			intersectionIDMap.insert(std::pair<Line_2*, int>(line, intersectionID));
			intersectionID++;
		}
	}

	return true;
}
