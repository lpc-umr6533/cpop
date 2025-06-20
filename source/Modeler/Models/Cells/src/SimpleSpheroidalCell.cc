#include "RandomEngineManager.hh"
#include "CellMeshSettings.hh"
#include "Geometry_Utils_Sphere.hh"
#include "SimpleSpheroidalCell.hh"

#include <CGAL/centroid.h>
#include <string>

/// \param pCellProperties the cell properties
/// \param pOrigin the origin to the cell
/// \param pSpheroidRadius the radius of the cell membrane at rest
/// \param pNucleusRadius the radius of the nuclear membrane.
/// \param pPosType the type of position to set to the nucleus
/// \param pWeight the weight of the cell
/// \param pMembraneShape the inital shape of the cell
SimpleSpheroidalCell::SimpleSpheroidalCell(
	const CellProperties* pCellProperties,
	Point_3 pOrigin,
	double pSpheroidRadius,
	double pNucleusRadius,
	eNucleusPosType pPosType,
	double pWeight,
	Mesh3D::Polyhedron_3 pMembraneShape):
	SpheroidalCell(pCellProperties, pOrigin, pSpheroidRadius, pWeight, pMembraneShape)
{
	_nucleus = new RoundNucleus<double, Point_3, Vector_3>(pNucleusRadius, pOrigin, pPosType);
	_nuclei.push_back(_nucleus);
	//SpheroidalCell::convertToG4Structure()
}

/// \param nucleusPositionType The type of position of the cell
/// \return the center of the nucleus
Point_3 SimpleSpheroidalCell::getNucleusCenter(eNucleusPosType nucleusPositionType) const {
	switch(nucleusPositionType) {
		case ORIGIN:      return getPosition();
		case BARYCENTER:
		{
			// The centroid is the barycenter with equal weight for each vertices from the polyhedron
			return CGAL::centroid(_shape->points_begin(), _shape->points_end(), CGAL::Dimension_tag<0>());
		}
		case NO_STANDARD:
		{
			return getNucleusCenter(static_cast<eNucleusPosType>(RandomEngineManager::getInstance()->randi(0, (int)END_CHOICE - 2)));
		}
		default:
		{
			std::string mess = "unknow nucleusPositionType" + std::to_string(nucleusPositionType) + ", Set tposition to the barycenter ";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "SpheroidalCellMesh");
			return getNucleusCenter(BARYCENTER);
		}
	}
}

void SimpleSpheroidalCell::setNucleusCenter() {
	_nucleus->setOrigin( getNucleusCenter(_nucleus->getPositionType()));
}

/// \param intersections The plane defining the boundary
void SimpleSpheroidalCell::generateNuclei(std::vector<Plane_3*> intersections) {
	assert(getRadius() > 0.);
	assert(_nucleus->getRadius() > 0.);
	assert(getRadius() > _nucleus->getRadius());
	assert(_maxRatioToCellRadius <= 1.);
	assert(_maxRatioToCellRadius > 0.);

	// check intersection plane.
	double maxRadius = _maxRatioToCellRadius * this->getRadius();
	double optimalRadius = _nucleus->getRadius();	// the one requested

	// if intersections
	for(auto const& intersection : intersections) {
		assert(intersection);
		double localRadius = sqrt( squared_distance(intersection->projection(_nucleus->getOrigin()), _nucleus->getOrigin()) );
		if(localRadius < optimalRadius)
			optimalRadius = localRadius;
	}

	// if too larger radius
	if(optimalRadius > maxRadius)
		optimalRadius = maxRadius;

	_nucleus->setRadius(optimalRadius);

	assert(_nucleus->getRadius() > 0);
}

void SimpleSpheroidalCell::resetMesh() {
	SpheroidalCell::resetMesh();
	/// nothing to do for the nucleus mesh
}

/// \return The random spot requested on the cytoplasm
/// \details here we optimize the function because we have only one nucleus and we can speed up this process.
Point_3 SimpleSpheroidalCell::getSpotOnCytoplasm() const {
	//G4cout<< "\n\n\n getSpotOnCytoplasm" << G4endl;

	// // pick a point on the membrane, then pick a point on the segment [MembranePoint/cellOrigin] Until we found one not on the nucleus
	// // because shape is convex we are sure the picked point is on the cytoplsam (work until no other organ are defined)
	// Point_3 cellMembraneSpot = getSpotOnCellMembrane();
	// // as we are on a simple cell ( one nucleus, one cytoplasm ) we are sure that the resting space is empty.
	// // in this case nucleus is a perfect sphere
	// Point_3 projectedPtOnNucleus = Utils::Geometry::Sphere::getProjectionOnSphere(cellMembraneSpot, getNucleus()->getRadius(), getNucleus()->getOrigin());
	//
	// Vector_3 v( cellMembraneSpot - projectedPtOnNucleus);
	// double x = RandomEngineManager::getInstance()->randd(0., 1.);
	// Point_3 res = getPosition() + x*(v);
	//
	// return res;


	Vector_3 v(getSpotOnCellMembrane() - getPosition());
	double x = RandomEngineManager::getInstance()->randd(0., 1.);
	Point_3 res = getPosition() + x*(v);

	for(auto const& itNuc : _nuclei) {
		// if point is on a nucleus, pick an other one
		if(itNuc->hasIn(res))
			return getSpotOnCytoplasm();
	}

	return res;
}

void SimpleSpheroidalCell::exportNucleiToFile(std::string const& path) const {
	for(auto const& nucleus: _nuclei) {
	}
}
