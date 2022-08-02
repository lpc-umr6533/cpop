/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ATTRACTION_FORCE_HH
#define ATTRACTION_FORCE_HH

#include "PointForce.hh"

//////////////////////////////////////////////////////////////////////////////
/// \brief define an attraction force based on distance and Reynolds law.
/// The farthest the agent is the higher we attract.
/// Also include a " rigidity, a 'rate' of repulsion"
/// near the repulsion point but distance is set to ².
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class AttractionForcePoint : public PointForce<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	AttractionForcePoint(const Cell<Kernel, Point, Vector>* cell, Point origin, Kernel, Kernel = 1.);
	/// \brief destructo
	~AttractionForcePoint();
	/// \brief the definition of the force
	Vector computeForce() const;

private:
	Kernel distanceOfAction;						///< \brief the distance throught we start the force ( only circular for now )
	Kernel rigidity;								///< \brief the "rigidity" of the repulsion
};

////////////////////////////////////////////////////////////////////////////////////////////////
/// \param cell 				The cell on which we want to apply the force
/// \param pPoint 				The origin of the force
/// \param pDistanceOfAction	The distance from where we start to apply the force
/// \param pRigidity 			The rigidity of the force
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
AttractionForcePoint<Kernel, Point, Vector>::AttractionForcePoint(
	const Cell<Kernel, Point, Vector>* cell, 
	Point pPoint, 
	Kernel pDistanceOfAction, 
	Kernel pRigidity ):
	PointForce<Kernel, Point, Vector>(cell, pPoint),
	distanceOfAction( pDistanceOfAction),
	rigidity(pRigidity)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
AttractionForcePoint<Kernel, Point, Vector>::~AttractionForcePoint()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Vector AttractionForcePoint<Kernel, Point, Vector>::computeForce() const
{
	Vector force;

	Kernel actualDistance = sqrt( CGAL::squared_distance(Force<Kernel, Point, Vector>::cell->getPosition(), PointForce<Kernel, Point, Vector>::point ) );	// distance in um
	if( actualDistance < distanceOfAction )
	{
		// add the repulsion force
		Kernel hForce = rigidity * (distanceOfAction - actualDistance); 
		force = hForce * Utils::myCGAL::normalize(Vector( PointForce<Kernel, Point, Vector>::point - Force<Kernel, Point, Vector>::cell->getPosition() ));
	} 
	return force;
}

#endif // ATTRACTION_FORCE_HH