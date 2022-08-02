/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef REPULSION_FORCE_HH
#define REPULSION_FORCE_HH

#include "PointForce.hh"

//////////////////////////////////////////////////////////////////////////////
/// \brief define a repulsion force based on distance and Reynolds law.
/// The nearest agent is, the higher they are repusled.
/// Also include a " rigidity, a 'rate' of repulsion"
/// near the attraction point but distance is set to ³.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class RepulsionForcePoint : public PointForce<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	RepulsionForcePoint(const Cell<Kernel, Point, Vector>* cell, Point origin, Kernel, Kernel = 1.);
	/// \brief destructo
	~RepulsionForcePoint();
	
	/// \brief the definition of the force
	Vector computeForce() const;

private:
	Kernel distanceOfAction;	///< \brief the distance of begining of the force ( only circular for now )
	Kernel rigidity;			///< \brief the "rigidity" of the repulsion
};


////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCell				The cell we want to apply the force for
/// \param pOrigin 				The origin of the force
/// \param pDistanceOfAction 	The distance from where we start to apply the force
/// \param pRigidity 			The rigidity of the force
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
RepulsionForcePoint<Kernel, Point, Vector>::RepulsionForcePoint(const Cell<Kernel, Point, Vector>* pCell, 
	Point pOrigin, 
	Kernel pDistanceOfAction, 
	Kernel pRigidity ):
	PointForce<Kernel, Point, Vector>(pCell, pOrigin),
	distanceOfAction(pDistanceOfAction),
	rigidity(pRigidity)
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
RepulsionForcePoint<Kernel, Point, Vector>::~RepulsionForcePoint()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////
/// \return The force generated
////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Vector RepulsionForcePoint<Kernel, Point, Vector>::computeForce() const
{
	Vector force;

	Kernel actualDistance = sqrt( CGAL::squared_distance(Force<Kernel, Point, Vector>::cell->getPosition(), PointForce<Kernel, Point, Vector>::point ) );	// distance in um
	if( (actualDistance < distanceOfAction) && (actualDistance > 0.) )
	{
		// add the repulsion force
		Kernel hForce = rigidity * (distanceOfAction - actualDistance); 
		force = hForce * Utils::myCGAL::normalize(Vector( Force<Kernel, Point, Vector>::cell->getPosition() - PointForce<Kernel, Point, Vector>::point ));
	} 
	return force;
}

#endif // REPULSION_FORCE_HH