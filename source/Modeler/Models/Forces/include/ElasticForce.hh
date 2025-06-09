#ifndef ELASTIC_FORCE_HH
#define ELASTIC_FORCE_HH

#include "CGAL_Utils.hh"
#include "EForceInputType.hh"
#include "Force.hh"
#include "Round_Shape.hh"
#include "SpatialDataStructureManager.hh"

static double default_ratio_to_stable_case = 0.75;

#include <CGAL/centroid.h>

/// \brief define an elastic force.
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class ElasticForce : public Force<Kernel, Point, Vector> {
public:
	ElasticForce(const Cell<Kernel, Point, Vector>* cell, Kernel pRigidityCste, Kernel pRatioToStableCase = default_ratio_to_stable_case);
	
	/// \brief the definition of the force
	inline Vector computeForce() const;

protected:	
	/// \brief compute the optimal length betwwen two cell ( as the length of the ressort at rest)
	Kernel getOptimalLength(const SpatialableAgent<Kernel, Point, Vector>*) const;

private:
	Kernel _rigidityConstant;  ///< \brief The constante of rigidity
	Kernel _ratioToStableCase; ///< \brief used to find the length at rest of the elastic force.
};

/// \param pCell The cell which will receive the force
/// \param pRigidityCste the constante of rigidity of the elastic force. 
/// \param pRatioToStableCase used to find the length at rest of the elastic force.
template<typename Kernel, typename Point, typename Vector>
ElasticForce<Kernel, Point, Vector>::ElasticForce(const Cell<Kernel, Point, Vector>* pCell, Kernel pRigidityCste, Kernel pRatioToStableCase) :
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NEIGHBOURS),
	_rigidityConstant(pRigidityCste),
	_ratioToStableCase(pRatioToStableCase)
{
}

template<typename Kernel, typename Point, typename Vector>
Kernel  ElasticForce<Kernel, Point, Vector>::getOptimalLength(const SpatialableAgent<Kernel, Point, Vector>* pCell2) const {
	assert((Force<Kernel, Point, Vector>::_cell));
	assert(pCell2);
	assert((Force<Kernel, Point, Vector>::_cell->getBody()) );
	assert(pCell2->getBody());

	// try to match with round cell
	const auto* shape1 = dynamic_cast<const Round_Shape<Kernel, Point, Vector>*> (Force<Kernel, Point, Vector>::_cell->getBody());
	const auto* shape2 = dynamic_cast<const Round_Shape<Kernel, Point, Vector>*> (pCell2->getBody());

	if(shape1 && shape2)
		return (shape1->getRadius() + shape2->getRadius()) * _ratioToStableCase;

	return (Kernel)0;
}

template<typename Kernel, typename Point, typename Vector>
inline Vector ElasticForce<Kernel, Point, Vector>::computeForce() const {
	std::set<const SpatialableAgent< Kernel,  Point,  Vector>* > agentToConsider = Force<Kernel, Point, Vector>::getConcernedAgent();

	Point cellOrigin = Force< Kernel,  Point,  Vector>::_cell->getPosition();
	// get neighbours from the SDSManager.
	typename std::set<const SpatialableAgent< Kernel,  Point,  Vector>* >::const_iterator itNeighbour;
	Vector force;
	
	for(itNeighbour = agentToConsider.begin(); itNeighbour != agentToConsider.end(); ++itNeighbour) {
		Point neighbourOrigin = (*itNeighbour)->getPosition();
		Kernel optimalDistance = ElasticForce<Kernel, Point, Vector>::getOptimalLength(*itNeighbour);	// distance in um
		double currentDistance = sqrt( CGAL::squared_distance( neighbourOrigin,  cellOrigin) );			// distance in um
		/// The formula is F = -k.X with k = rigidity constante
		Kernel hForce = -1.* _rigidityConstant * (optimalDistance - currentDistance); 
		force = force + hForce * Utils::myCGAL::normalize(Vector( neighbourOrigin - cellOrigin ));
	}

	return force;
}

#endif
