#ifndef ROUND_UP_FORCE_HH
#define ROUND_UP_FORCE_HH

#include "Force.hh"

#include <CGAL/centroid.h>

/// \brief The Force regouping, rounding up cells.
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class RoundUpForce : public Force<Kernel, Point, Vector> {
public:
	RoundUpForce( const Cell< Kernel,  Point,  Vector>*);

	/// \brief the definition of the force
	[[nodiscard]] Vector computeForce() const override;
};

/// \param pCell the cell to apply the force
template<typename Kernel, typename Point, typename Vector>
RoundUpForce< Kernel,  Point,  Vector>::RoundUpForce( const Cell< Kernel,  Point,  Vector>* pCell):
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NEIGHBOURS)
{
}

/// \return The set of cells neighbouring the current cell
template<typename Kernel, typename Point, typename Vector>
Vector RoundUpForce< Kernel,  Point,  Vector>::computeForce() const {
	std::set<const SpatialableAgent< Kernel,  Point,  Vector>*> agentToConsider = Force<Kernel, Point, Vector>::getConcernedAgent();

	// get neighbours from the SDSManager.
	std::vector<Point> allNeighbourPosition;
	for(auto itNeighbour = agentToConsider.begin(); itNeighbour != agentToConsider.end(); ++itNeighbour) {
		Point p = (*itNeighbour)->getPosition();
		allNeighbourPosition.push_back(p);
	}

	Point barycenter = CGAL::centroid(allNeighbourPosition.begin(), allNeighbourPosition.end());
	Point o = Force< Kernel,  Point,  Vector>::cell->getPosition();
	return {barycenter - o};
}

#endif
