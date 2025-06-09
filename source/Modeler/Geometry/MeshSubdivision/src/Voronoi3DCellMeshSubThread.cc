#include "Voronoi3DCellMeshSubThread.hh"

#include "Geometry_Utils_Sphere.hh"
#include "CellMeshSettings.hh"
#include "UnitSystemManager.hh"

#include <CGAL/convex_hull_3.h>
#include <CGAL/intersections.h>

#ifndef NDEBUG
	#define VORONOI_3D_MESH_SUBDIVISION_DEBUG 0
#else
	#define VORONOI_3D_MESH_SUBDIVISION_DEBUG 0	// must always stay at 0
#endif

using namespace Utils::Geometry;
using namespace Utils::Geometry::Sphere;

// patch for refinnement
static const double minDistPts = 0.00001;
// end refinement patch

/// \param pID 						The ID of the thread to reffine.
/// \param pMaximalNumberOfFacets 	The number of facet until which we wtop subdivision.
/// \param pDeltaWin 				The minimal win we are interesting to subdivide for
/// \param pCellsToReffine 			The set of cell the thread must execute
/// \param pNeighbourCells 			The map of all the neighbouring.
/// \warning pNeighbourgCells must stay a read only struct for threads
/// \param pSpaceBetweenCells 		The space to set between each cell.
///< \warning pNeighbourgCells must stay a read only struct for threads
Voronoi3DCellMeshSubThread::Voronoi3DCellMeshSubThread(
	unsigned int pID,
	unsigned int pMaximalNumberOfFacets,
	double pDeltaWin,
	const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >* pNeighbourCells,
	std::vector<SpheroidalCell*> pCellsToReffine,
	double pSpaceBetweenCells
):
	RefinementThread(pID, pMaximalNumberOfFacets, pDeltaWin),
	cellsToReffine(pCellsToReffine),
	neighbourCells(pNeighbourCells),
	_spaceBetweenCells(pSpaceBetweenCells)
{
	// patch for refinement
	_deltaReffinement = std::max(minDistPts, _deltaReffinement);
	// end patch refinement
}

Voronoi3DCellMeshSubThread::~Voronoi3DCellMeshSubThread() {
	clean();
	intersectionIDMap.clear();
	cellsToReffine.clear();
}

/// \param cell The cell to reffine.
/// \return bool true if sucess.
bool Voronoi3DCellMeshSubThread::reffineCell(SpheroidalCell* cell) {
	assert(cell);
	// warning the functions called here are link one to another so do not change the order of execution
	{
		clean();
		if(!generateIntersectionPlane(cell)) 		return false;
		if(!subdivseCellMembraneMesh(cell))			return false;
		cell->computeMembraneSurfaceArea();
	}

	return true;
}

void Voronoi3DCellMeshSubThread::run() {
	// new std::vector<SpheroidalCell*> cells;
	// SpheroidalCellMesh::exportToFileGDML("/home/levrague/Documents/Geant4_these", cells, true);

	// bool sucess = true;
	/// TODO : send error if not a sucess
	for(auto const& cell : cellsToReffine) {
		if(!reffineCell(cell)) {
			// sucess = false;
			return;
		}
	}
}

void Voronoi3DCellMeshSubThread::clean() {
	reffinementValues.clear();
	// delete intersections
	for(auto* intersection : intersections)
		delete intersection;

	intersections.clear();
}

/// \param cell The cell to add to the list of cell to refine
void Voronoi3DCellMeshSubThread::addCell(SpheroidalCell* cell) {
	assert(cell);
	cellsToReffine.push_back(cell);
}

///	\param cell  The cell to reffine.
///	\return  bool True if sucess
// TODO : SpheroidalCell must be const here
bool Voronoi3DCellMeshSubThread::generateIntersectionPlane(SpheroidalCell* cell) {
	assert(cell);
	assert(neighbourCells->find(cell) != neighbourCells->end());
	assert((cell->getRadius() - _spaceBetweenCells/2.) > 0.);
	int intersectionID = 0;

	std::set<const SpheroidalCell*> cellNeighbour = neighbourCells->find(cell)->second;
	std::vector<Point_3> polyInitPts;

	for(auto const& neighbour : cellNeighbour) {
		assert(neighbour);
		// TODO : find a way to not use the sqrt.
		double distance = sqrt(squared_distance(neighbour->getOrigin(), cell->getOrigin())) - (cell->getRadius() + neighbour->getRadius() + _spaceBetweenCells);
		// if is an intersection point
		if(distance == 0)
			polyInitPts.push_back(getProjectionOnSphere(neighbour->getOrigin(), (cell->getRadius() - _spaceBetweenCells/2.), cell->getOrigin() ));

		// if a plane intersection
		if(distance < 0.) {
			// no need to calculate the sphere, we didn't need it.
			Plane_3 interPlane = K().construct_radical_plane_3_object()(
				Sphere_3(neighbour->getOrigin(), neighbour->getSquareRadius() + (_spaceBetweenCells*_spaceBetweenCells / 4.), CGAL::COUNTERCLOCKWISE),	// CGAL take the square radius in parameter
				Sphere_3(cell->getOrigin(), cell->getSquareRadius() - (_spaceBetweenCells*_spaceBetweenCells / 4.), CGAL::COUNTERCLOCKWISE)
			);

			Plane_3* plane = nullptr;
			if(interPlane.oriented_side(cell->getOrigin()) == CGAL::ON_NEGATIVE_SIDE) {
				plane = new Plane_3(interPlane.opposite());
			} else {
				plane = new Plane_3(interPlane);
			}

			intersections.push_back( plane );
			intersectionIDMap.insert(std::pair<Plane_3*, int>(plane, intersectionID));
			intersectionID++;
		}
	}

	std::vector<Point_3> basicPoints = generateBasicCellMembraneMesh(cell);
	polyInitPts.insert( polyInitPts.begin(), basicPoints.begin(), basicPoints.end());

	CGAL::convex_hull_3(polyInitPts.begin(), polyInitPts.end(), *(cell->getShape()));

	return true;
}

///	\param cell The cell to reffine.
///	\return The set of point composing the basic cell membrane
inline std::vector<Point_3> Voronoi3DCellMeshSubThread::generateBasicCellMembraneMesh(SpheroidalCell* cell) {
	assert(cell);
	assert((cell->getRadius() - _spaceBetweenCells/2.) > 0.);
	// create an interstice between cells to prevent potential overlaps due to numericam precision (Geant4)
	double preventOverlaps = 20.*UnitSystemManager::getInstance()->getMetricUnit(UnitSystemManager::Nanometer);
	std::vector<Point_3> polyInitPts = Utils::Geometry::Sphere::getNPointOnSphere(cell->getOrigin(), cell->getRadius() - preventOverlaps/2., MIN_SPHERE_NB_POINT);

	// update standard point from intersections
	for(auto& polyInitPt : polyInitPts)
		adjustPointFromIntersections(polyInitPt);
	return polyInitPts;
}

///	\param cell The cell to reffine.
///	\return bool True if sucess.
inline bool Voronoi3DCellMeshSubThread::subdivseCellMembraneMesh(SpheroidalCell* cell) {
	// TODO : Evaluer l'erreur residuel de l'approximation.
	assert(cell);

	if(VORONOI_3D_MESH_SUBDIVISION_DEBUG) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES,
			"start reffinement",
			"VoronoiCell_ToSpheroid_Refinement"
		);
	}

	// case we can avoid the creation of all projection.
	if(std::distance(cell->shape_facets_begin(), cell->shape_facets_end()) >= (int)RefinementThread::_numberOfUnitaryEntityPerCell)
		return true;

	// create stack of facet for raffinement.
	std::set<CPOP_Triangle> facets;
	for(Polyhedron_3::Facet_iterator itFacet = cell->shape_facets_begin(); itFacet != cell->shape_facets_end(); ++itFacet) {
		if(itFacet->is_triangle()) {
			Point_3 pt1 = itFacet->halfedge()->vertex()->point();
			Point_3 pt2 = itFacet->halfedge()->next()->vertex()->point();
			Point_3 pt3 = itFacet->halfedge()->next()->next()->vertex()->point();

			CPOP_Triangle facet(pt1, pt2, pt3, true);  // true == can raffine
			// will be ordered with the first win on top.
			facets.insert(facet);
		}
	}

	/// until we need to include facet
	while(facets.size() < RefinementThread::_numberOfUnitaryEntityPerCell) {
		CPOP_Triangle facetToReffine = *facets.begin();
		// if reach the win is not enought or the reffinement is useless.
		// todo : add function get potential win plutôt
		if((facetToReffine.get16squareA() < _deltaReffinement)  || (!facetToReffine.canReffine())) {
			if(VORONOI_3D_MESH_SUBDIVISION_DEBUG) {
				InformationSystemManager::getInstance()->Message(InformationSystemManager::INFORMATION_MES,
					"no more potential win, end subdivision",
					"VoronoiCell_ToSpheroid_Refinement"
				);
			}
			break;
		}

		// remove the old facet
		facets.erase(facets.begin());

		/// get the three new vertices to add. They can be:
		Point_3 ptToAdd1 = getPointNewPointFromSeg(0, 1, &facetToReffine, cell->getOrigin(), cell->getRadius() - _spaceBetweenCells/2.);
		Point_3 ptToAdd2 = getPointNewPointFromSeg(1, 2, &facetToReffine, cell->getOrigin(), cell->getRadius() - _spaceBetweenCells/2.);
		Point_3 ptToAdd3 = getPointNewPointFromSeg(2, 0, &facetToReffine, cell->getOrigin(), cell->getRadius() - _spaceBetweenCells/2.);

		/// if error during sub division
		if(
			is_nan(ptToAdd1.x()) || is_nan(ptToAdd1.y()) || is_nan(ptToAdd1.z()) ||
			is_nan(ptToAdd2.x()) || is_nan(ptToAdd2.y()) || is_nan(ptToAdd2.z()) ||
			is_nan(ptToAdd3.x()) || is_nan(ptToAdd3.y()) || is_nan(ptToAdd3.z())
		) {
			facetToReffine.setReffinement(false);
			facets.insert(facetToReffine);
		} else {
			std::vector<Point_3> pts;
			pts.push_back(ptToAdd3);
			pts.push_back(ptToAdd2);
			pts.push_back(ptToAdd1);

			// if one the new points arn't interesting enought
			if(!ptsAreInteresting(&facetToReffine, pts)) {
				facetToReffine.setReffinement(false);
				facets.insert(facetToReffine);
			} else {
				CPOP_Triangle newFacet1(facetToReffine.getA(), ptToAdd1, ptToAdd3, true);
				facets.insert(newFacet1);

				CPOP_Triangle newFacet2(ptToAdd1, facetToReffine.getB(), ptToAdd2, true);
				facets.insert(newFacet2);

				CPOP_Triangle newFacet3(ptToAdd2, facetToReffine.getC(), ptToAdd3, true);
				facets.insert(newFacet3);

				CPOP_Triangle newFacet4(ptToAdd1, ptToAdd2, ptToAdd3, true);
				facets.insert(newFacet4);
			}
		}
	}

	/// after having generating enought facet recreate the new polyhedron
	std::set<Point_3> pts;
	for(auto const& facet : facets) {
		pts.insert(facet.getA());
		pts.insert(facet.getB());
		pts.insert(facet.getC());
	}
	CGAL::convex_hull_3(pts.begin(), pts.end(), *(cell->getShape()) );

	return true;
}

/// \param ptToCheck The point to adjust according to intersections
inline void Voronoi3DCellMeshSubThread::adjustPointFromIntersections(Point_3& ptToCheck) {
	int currentLine = -1;

	for(auto const& intersection : intersections) {
		/// En : it is the equivalent of the intersection test between the segment ( medium segment point, medium segment point's projection on tte sphere)
		/// work only because we are on a sphere and one of the segment point is already on the sphere.
		/// This permit to deduce that the projection of the point can at mst intersect one of the plane (from the voronoi cell)
		/// Fr : est l'equivalent d'un test d'intersection entre le segment (milieu du seg à subdiviser, sa projection sur la sphère )
		/// Fonctionne seulement car l'on travail dans une sphère et que l'on raffine un segment dont
		/// un de ces points est déjà situé sur la sphère.
		/// On peut en déduire que le projeté du milieu du segment ne peut au pus intersecter un et un seul plan de la cellule de voronoi.
		if(intersection->oriented_side(ptToCheck) == CGAL::ON_NEGATIVE_SIDE) {
			// the intersection of the segment with the plane correspond to the projection because we are on an arc sphere that we want to reffine
			// (<=> one of the point is already set on the sphere)
			ptToCheck = intersection->projection(ptToCheck);
			// remove the point if undef. CGAL issue ?
			if(is_nan(ptToCheck.x()) || is_nan(ptToCheck.y()) || is_nan(ptToCheck.z()))
				return;
			currentLine = intersectionIDMap[intersection];
		}
	}

	// else no interssection : on the spehroid : value = -1
	reffinementValues.insert(std::pair<Point_3, int>(ptToCheck, currentLine));
}

/// \details Points aren't interesting if they are too close existing vertices.
/// \return bool true if the points is interesting for subdivision
/// \param facet The facet we want to compare the point with
/// \param pts to compare with the facet
inline bool Voronoi3DCellMeshSubThread::ptsAreInteresting(const CPOP_Triangle* facet, std::vector<Point_3> pts) {
	// TODO : remove this function and replace by the map of point to know where they are situated.
	Triangle_3 tri(facet->getA(), facet->getB(), facet->getC());
	for(auto const& pt : pts) {
		if(
			(CGAL::squared_distance(facet->getA(), pt) < _deltaReffinement ) ||
			(CGAL::squared_distance(facet->getB(), pt) < _deltaReffinement ) ||
			(CGAL::squared_distance(facet->getC(), pt) < _deltaReffinement )
		)
			return false;
	}
	return true;
}

/// \param indexVertex1 The index of the first vertex part of the segment
/// \param indexVertex2 The index of the second vertex part of the segment
/// \param triangle 	The triangle we want the segment from
/// \param sphereCenter The origin of the sphere
/// \param sphereRadius The radius of the sphere.
/// \return 		 	The point to add to the polyhedron
inline Point_3 Voronoi3DCellMeshSubThread::getPointNewPointFromSeg(int indexVertex1, int indexVertex2, const CPOP_Triangle* triangle, Point_3 sphereCenter, double sphereRadius) {
	// get the medium point on the segment
	Point_3 MediumSegPt(
		(triangle->get(indexVertex1).x() + triangle->get(indexVertex2).x())/2.,
		(triangle->get(indexVertex1).y() + triangle->get(indexVertex2).y())/2.,
		(triangle->get(indexVertex1).z() + triangle->get(indexVertex2).z())/2.
	);

	// here use InterscetionIDMap to now if need to project or not the point...
	Point_3 res = getProjectionOnSphere( MediumSegPt, sphereRadius, sphereCenter);
	adjustPointFromIntersections(res);
	return res;
}
