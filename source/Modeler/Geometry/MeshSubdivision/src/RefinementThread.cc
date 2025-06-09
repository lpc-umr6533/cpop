#include "RefinementThread.hh"

/// \param pID The ID of the thread to reffine.
/// \param pNumberOfUnitaryEntityPerCell The optiaml number of unit for each cell. This unit is in 2D segment and triangle int 3D
/// \param pDeltaWin The minimal win we are interesting to subdivide for
/// \param pMaxRatioSizeCell The maximal ration the nucleus radius must not overtake. in [0..1]
RefinementThread::RefinementThread(
	unsigned int pID,
	unsigned int pNumberOfUnitaryEntityPerCell,
	double pDeltaWin,
	double pMaxRatioSizeCell
):
	_id(pID),
	_numberOfUnitaryEntityPerCell(pNumberOfUnitaryEntityPerCell),
	_deltaReffinement(pDeltaWin),
	_maxRatioToCellRadius(pMaxRatioSizeCell)
{
}
