#include "SpheroidalCell_MeshSub_Thread.hh"
#include <CGAL/convex_hull_3.h>

#ifndef NDEBUG
	#define SPHEROID_CELL_MESH_SUBDIVISION_DEBUG 0
#else
	#define SPHEROID_CELL_MESH_SUBDIVISION_DEBUG 0	// must always stay at 0
#endif

#if ( defined(WIN32) || defined(WIN64) || defined(_WIN32) || defined(_WIN64) )
	#define isnan _isnan
#endif

/// \param pID 						The ID of the thread to reffine.
/// \param pMaximalNumberOfFacets 	The number of facet until which we wtop subdivision.
/// \param pDeltaWin 				The minimal win we are interesting to subdivide for
/// \param pMaxRatioToCellRadius 	The maximal ration the nucleus radius must not overtake. in [0..1]
/// \param cellsToReffine			The set of cell the thread must execute
/// \param pNeighbourCells 			The map of all the neighbouring.
/// \warning pNeighbourgCells must stay a read only struct for threads
/// \param pSpaceBetweenCells 		The space to set between each cell.
/// \warning pNeighbourgCells must stay a read only struct for threads
SpheroidalCellMeshSubThread::SpheroidalCellMeshSubThread(
	unsigned int pID,
	unsigned int pMaximalNumberOfFacets,
	double pDeltaWin,
	const std::map<SpheroidalCell*, std::set<const SpheroidalCell*> >* pNeighbourCells,
	double pMaxRatioToCellRadius,
	std::vector<SpheroidalCell*> pCellsToReffine,
	double pSpaceBetweenCells
):
	Voronoi3DCellMeshSubThread(pID, pMaximalNumberOfFacets, pDeltaWin, pNeighbourCells, pCellsToReffine, pSpaceBetweenCells)
{
	RefinementThread::setMaxRatioNucleus(pMaxRatioToCellRadius);
}

SpheroidalCellMeshSubThread::~SpheroidalCellMeshSubThread() {
	clean();
}

///	\param <cell> [in] {The cell to reffine.}
///	\return {True if sucess.}
bool SpheroidalCellMeshSubThread::reffineCell(SpheroidalCell* cell) {
	assert(cell);
	/// warning the function called here are link one to another so do no change the order of execution
	{
		clean();
		if(!generateIntersectionPlane(cell)) 		return false;
		if(!generateNucleusRadius(cell))			return false;
		if(!subdivseCellMembraneMesh(cell))			return false;
		cell->computeMembraneSurfaceArea();

	}
	return true;
}

///	\param <cell> [in] {The cell to reffine.}
///	\return {True if sucess.}
bool SpheroidalCellMeshSubThread::generateNucleusRadius(SpheroidalCell* cell) {
	/// \warning : make sure the basic shape is set otherwise error. More the shpae contain points, longer this could take.
	cell->setMaxRatioNucleusToCell(_maxRatioToCellRadius);
	cell->setNucleusCenter();
	cell->generateNuclei(intersections);
	return true;
}
