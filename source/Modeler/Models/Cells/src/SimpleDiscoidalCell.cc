#include "CellMeshSettings.hh"
#include "RandomEngineManager.hh"
#include "SimpleDiscoidalCell.hh"
#include "Geometry_Utils_Circle.hh"
#include "Round_Shape.hh"

#include <vector>
#include <CGAL/centroid.h>

/// \param pCellProperties the cell properties
/// \param pOrigin the origin to the cell
/// \param pDiscoidRadius the radius of the cell membrane
/// \param pNucleusRadius the radius of the nuclear membrane
/// \param pPosType the type of position to set to the nucleus
/// \param pWeight the weight of the cell
/// \param pMembraneShape the inital shape of the cell
SimpleDiscoidalCell::SimpleDiscoidalCell(
	const CellProperties* pCellProperties,
	Point_2 pOrigin,
	double pDiscoidRadius,
	double pNucleusRadius,
	eNucleusPosType pPosType,
	double pWeight,
	std::vector<Mesh2D::Segment_2> pMembraneShape):
	DiscoidalCell(pCellProperties, pOrigin, pDiscoidRadius, pWeight, pMembraneShape)
{
	_nucleus = new RoundNucleus<double, Point_2, Vector_2>(pNucleusRadius, getPosition(), pPosType );
	_nuclei.push_back(_nucleus);
}

/// \details will generate nuclei from a vector of boundaries (here set as intersections
/// \param intersections The segment defining the cell boundary/delimitation.
void SimpleDiscoidalCell::generateNuclei(std::vector<Line_2*> intersections) {
	// check intersection plane.
	double maxRadius = _maxRatioToCellRadius * getRadius();
	double optimalRadius = getRadius();	// the one requested

	// if intersections
	for(auto const& intersection : intersections) {
		assert(intersection);
		// TODO optimisation
		double localRadius = sqrt(CGAL::squared_distance(intersection->projection(_nucleus->getOrigin()), _nucleus->getOrigin()));
		if(localRadius < optimalRadius)
			optimalRadius = localRadius;
	}

	// if too larger radius
	if(optimalRadius > maxRadius)
		optimalRadius = maxRadius;

	_nucleus->setRadius( optimalRadius);
}

/// \param nucleusPositionType The type of position of the cell
/// \return the center of the nucleus
Point_2 SimpleDiscoidalCell::getNucleusCenter(eNucleusPosType nucleusPositionType) const {
	switch(nucleusPositionType) {
		case ORIGIN:      return getPosition();
		case BARYCENTER:
		{
			// The centroid is the barycenter with equal weight for each vertices from the polyhedron
			std::set<Point_2> pts;
			for(auto const& iPt : *_shape)
				pts.insert(iPt.source());
			return CGAL::centroid(pts.begin(), pts.end(), CGAL::Dimension_tag<0>());
		}
		case NO_STANDARD:
		{
			return getNucleusCenter( static_cast<eNucleusPosType>(RandomEngineManager::getInstance()->randi(0, (int)END_CHOICE - 2)));
		}
		default:
		{
			QString mess = "unknow nucleusPositionType" + QString::number(nucleusPositionType) + ", Set tposition to the barycenter ";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "SpheroidalCellMesh");
			return getNucleusCenter(BARYCENTER);
		}
	}
}

void SimpleDiscoidalCell::setNucleusCenter() {
	_nucleus->setOrigin( getNucleusCenter(_nucleus->getPositionType()));
}

void SimpleDiscoidalCell::resetMesh() {
	DiscoidalCell::resetMesh(); // reset membrane mesh
	/// nothing to do for the nucleus mesh
}
