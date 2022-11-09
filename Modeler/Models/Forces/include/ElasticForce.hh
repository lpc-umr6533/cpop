/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ELASTIC_FORCE_HH
#define ELASTIC_FORCE_HH

#include "CGAL_Utils.hh"
#include "EForceInputType.hh"
#include "Force.hh"
#include "Round_Shape.hh"
#include "SpatialDataStructureManager.hh"

static double default_ratio_to_stable_case = 0.75;

#include <CGAL/centroid.h>

//////////////////////////////////////////////////////////////////////////////
/// \brief define an elastic force.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class ElasticForce : public Force<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	ElasticForce(const Cell<Kernel, Point, Vector>* cell, Kernel pRigidityCste, Kernel pRatioToStableCase = default_ratio_to_stable_case);
	/// \brief destructor
	~ElasticForce();
	
	/// \brief the definition of the force
	inline Vector computeForce() const;

protected:	
	/// \brief compute the optimal length betwwen two cell ( as the length of the ressort at rest)
	Kernel getOptimalLength(const SpatialableAgent<Kernel, Point, Vector>*) const;

private:
	Kernel rigidity_constante;		///< \brief The constante of rigidity
	Kernel ratioToStableCase;		///< \brief used to find the length at rest of the elastic force.
};

///////////////////////////////////////////////////////////////////////////////
/// \param pCell The cell which will receive the force
/// \param pRigidityCste the constante of rigidity of the elastic force. 
/// \param pRatioToStableCase used to find the length at rest of the elastic force.
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
ElasticForce<Kernel, Point, Vector>::ElasticForce(const Cell<Kernel, Point, Vector>* pCell, Kernel pRigidityCste, Kernel pRatioToStableCase) :
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NEIGHBOURS),
	rigidity_constante(pRigidityCste),
	ratioToStableCase(pRatioToStableCase)
{

}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
ElasticForce<Kernel, Point, Vector>::~ElasticForce()
{

}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Kernel  ElasticForce<Kernel, Point, Vector>::getOptimalLength(const SpatialableAgent<Kernel, Point, Vector>* pCell2) const
{
	assert( (Force<Kernel, Point, Vector>::cell));
	assert(pCell2);
	assert( (Force<Kernel, Point, Vector>::cell->getBody()) );
	assert(pCell2->getBody());

	// try to match with round cell
	const Round_Shape<Kernel, Point, Vector>* shape1 = dynamic_cast<const Round_Shape<Kernel, Point, Vector>*> (Force<Kernel, Point, Vector>::cell->getBody());
	const Round_Shape<Kernel, Point, Vector>* shape2 = dynamic_cast<const Round_Shape<Kernel, Point, Vector>*> (pCell2->getBody());

	if(shape1 && shape2)
	{
		return ( shape1->getRadius() + shape2->getRadius()) * ratioToStableCase;
	}
	return (Kernel)0;
}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
inline Vector ElasticForce<Kernel, Point, Vector>::computeForce() const
{
	std::set<const SpatialableAgent< Kernel,  Point,  Vector>* > agentToConsider = Force<Kernel, Point, Vector>::getConcernedAgent();

	Point cellOrigin = Force< Kernel,  Point,  Vector>::cell->getPosition();
	// get neighbours from the SDSManager.
	typename std::set<const SpatialableAgent< Kernel,  Point,  Vector>* >::const_iterator itNeighbour;
	Vector force;
	
	for(itNeighbour = agentToConsider.begin(); itNeighbour != agentToConsider.end(); ++itNeighbour)
	{
		Point neighbourOrigin = (*itNeighbour)->getPosition();
		Kernel optimalDistance = ElasticForce<Kernel, Point, Vector>::getOptimalLength(*itNeighbour);	// distance in um
		double currentDistance = sqrt( CGAL::squared_distance( neighbourOrigin,  cellOrigin) );			// distance in um
		/// The formula is F = -k.X with k = rigidity constante
		Kernel hForce = -1.* rigidity_constante * (optimalDistance - currentDistance); 
		force = force + hForce * Utils::myCGAL::normalize(Vector( neighbourOrigin - cellOrigin ));
	}
	return force;
}

#endif // ELASTIC_FORCE_HH