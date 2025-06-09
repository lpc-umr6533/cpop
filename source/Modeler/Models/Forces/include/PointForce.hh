#ifndef FORCE_WITH_UnIQUE_AGENT_HH
#define FORCE_WITH_UnIQUE_AGENT_HH

#include "Force.hh"

/// \brief The same as force but redefine getConcernedAgent to 
/// set the one given on construction
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class PointForce : public Force<Kernel, Point, Vector> {
public:
	PointForce(const Cell<Kernel, Point, Vector>* pCell, Point);
	
	/// \brief return the force for the actual set up
	virtual Vector computeForce() const = 0;

protected:
	Point point;  ///< \brief the point source of the force
};

/////////////////////////////// IMPLEMENTATION ///////////////////////////////////////////////////////////

/// \param pCell The cell we want to have the force for.
/// \param pPoint the origin of the point force
template<typename Kernel, typename Point, typename Vector>
PointForce<Kernel, Point, Vector>::PointForce(
	const Cell<Kernel, Point, Vector>* pCell, 
	Point pPoint
):
	Force<Kernel, Point, Vector>(pCell, FRCE_INTERACT_WITH_NONE),
	point(pPoint)
{
}

#endif
