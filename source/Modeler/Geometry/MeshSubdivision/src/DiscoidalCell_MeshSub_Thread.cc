#include "DiscoidalCell_MeshSub_Thread.hh"

#ifndef NDEBUG
	#define DISCOID_CELL_MESH_SUBDIVISION_DEBUG 0
#else
	#define DISCOID_CELL_MESH_SUBDIVISION_DEBUG 0	// must always stay at 0
#endif

/// \param pID 						The ID of the thread to reffine.
/// \param pMaximalNumberOfFacets  	The number of facet threashold to stop subdivision.
/// \param pDeltaWin				The minimal win we are interesting to subdivide for
/// \param pNeighbourCells 			The map of all the neighbouring.
/// \warning pNeighbourgCells must stay a read only struct for threads
/// \param pMaxRatioToCellRadius	The maximal ration the nucleus radius must not overtake. in [0..1]
/// \param pCells					The set of cell the thread must execute
DiscoidalCellMeshSubThread::DiscoidalCellMeshSubThread(
	unsigned int pID,
	unsigned int pMaximalNumberOfFacets,
	double pDeltaWin,
	const std::map<DiscoidalCell*, std::set<const DiscoidalCell*> >* pNeighbourCells,
	double pMaxRatioToCellRadius,
	std::vector<DiscoidalCell*> pCells
):
	Voronoi2DCellMeshSubThread(pID, pMaximalNumberOfFacets, pDeltaWin, pNeighbourCells, pCells)
{
	RefinementThread::setMaxRatioNucleus(pMaxRatioToCellRadius);
}

DiscoidalCellMeshSubThread::~DiscoidalCellMeshSubThread() {
	clean();
}

///	\param pCell The cell to reffine.
///	\return True if sucess.
bool DiscoidalCellMeshSubThread::reffineCell(DiscoidalCell* pCell) {
	assert(pCell);
	/// \warning the function called here are link one to another so do no change the order of execution
	{
		clean();
		if(!generateIntersectionLine(pCell)) 		return false;
		if(!generateBasicCellMembraneMesh(pCell))	return false;
		if(!generateNucleusRadius(pCell))			return false;
	// 	if(!subdivseCellMembraneMesh(pCell))			return false;
	}
	return true;
}

///	\param pCell  The cell to reffine.
///	\return True if sucess.
bool DiscoidalCellMeshSubThread::generateNucleusRadius(DiscoidalCell* pCell) {
	assert(pCell);
	assert(pCell->getRadius() > 0.);
	assert(pCell->checkNucleiRadius());
	assert(_maxRatioToCellRadius <= 1.);
	assert(_maxRatioToCellRadius > 0.);

	/// \warning : make sure the basic shape is set otherwise error. More the shpae contain points, longer this could take.
	pCell->setMaxRatioNucleusToCell(_maxRatioToCellRadius);
	pCell->setNucleusCenter();
	pCell->generateNuclei(intersections);

	return true;
}
