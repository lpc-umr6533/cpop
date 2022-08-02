/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FORCE_WITH_UnIQUE_AGENT_HH
#define FORCE_WITH_UnIQUE_AGENT_HH

#include "Force.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The same as force but redefine getConcernedAgent to 
/// set the one given on construction
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class PointForce : public Force<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	PointForce(const Cell<Kernel, Point, Vector>* pCell, Point);
	/// \brief destructor
	~PointForce();
	
	/// \brief return the force for the actual set up
	virtual Vector computeForce() const = 0;

protected:
	Point point;			///< \brief the point source of the force
};

/////////////////////////////// IMPLEMENTATION ///////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \param pCell The cell we want to have the force for.
/// \param pPoint the origin of the point force
//////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
PointForce<Kernel, Point, Vector>::PointForce(
	const Cell<Kernel, Point, Vector>* pCell, 
	Point pPoint) :
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NONE),
	point(pPoint)
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
PointForce<Kernel, Point, Vector>::~PointForce()
{

}

#endif // FORCE_WITH_UnIQUE_AGENT_HH