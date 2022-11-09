/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef ROUND_UP_FORCE_HH
#define ROUND_UP_FORCE_HH

#include "Force.hh"

#include <CGAL/centroid.h>

//////////////////////////////////////////////////////////////////////////////
/// \brief The Force regouping, rounding up cells.
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class RoundUpForce : public Force<Kernel, Point, Vector>
{
public:
	/// \brief constructor
	RoundUpForce( const Cell< Kernel,  Point,  Vector>*);
	/// \brief destructor
	~RoundUpForce();
	/// \brief the definition of the force
	Vector computeForce() const;
};

///////////////////////////////////////////////////////////////////////////////
/// \param pCell the cell to apply the force
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
RoundUpForce< Kernel,  Point,  Vector>::RoundUpForce( const Cell< Kernel,  Point,  Vector>* pCell):
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NEIGHBOURS)
{

}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
RoundUpForce< Kernel,  Point,  Vector>::~RoundUpForce()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \return The set of cells neighbouring the current cell
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Vector RoundUpForce< Kernel,  Point,  Vector>::computeForce() const
{
	std::set<const SpatialableAgent< Kernel,  Point,  Vector>* > agentToConsider = Force<Kernel, Point, Vector>::getConcernedAgent();

	// get neighbours from the SDSManager.
	typename std::set<const SpatialableAgent< Kernel,  Point,  Vector>* >::const_iterator itNeighbour;
	std::vector<Point> allNeighbourPosition;
	for(itNeighbour = agentToConsider.begin(); itNeighbour != agentToConsider.end(); ++itNeighbour)
	{
		Point p = (*itNeighbour)->getPosition();
		allNeighbourPosition.push_back( p);
	}
	Point barycenter = CGAL::centroid(allNeighbourPosition.begin(), allNeighbourPosition.end());
	Point o = Force< Kernel,  Point,  Vector>::cell->getPosition();
	return Vector( barycenter - o);
}

#endif // ROUND_UP_FORCE_HH