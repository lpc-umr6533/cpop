#include "CellMeshSettings.hh"

unsigned int NUCLEUS_SPHERE_NB_POINT = MIN_SPHERE_NB_POINT;

namespace Settings {

/// \param pNbPts the number of degree to pick on each axis. So the total number pf point will be pNbPts*pNbPts
void setNucleusNbDegreePerAxis(unsigned int pNbPts) {
	NUCLEUS_SPHERE_NB_POINT = pNbPts;
}

}
