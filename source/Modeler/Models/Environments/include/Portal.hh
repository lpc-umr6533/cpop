#ifndef PORTAL_HH
#define PORTAL_HH

#include "ActiveDelimitation.hh"
#include "DynamicAgent.hh"

#include <CGAL/Vector_3.h> 	///< CGAL vect 3 definition
#include <CGAL/float.h> 	///< CGAL float definition
#include <CGAL/Cartesian.h>

/// \brief Defined a one side portal on the space.
/// A portal link like a bridge two comparable spatial areas (ActiveDelimitation).
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class Portal : public Writable {
public:
	Portal(ActiveDelimitation<Kernel, Point, Vector>* pIn, ActiveDelimitation<Kernel, Point, Vector>* pOut);

	/// \brief function to know if the agent is going to go throw the portal
	bool isGoingThrowPortal(DynamicAgent<Kernel, Point, Vector>*) const;
	/// \brief return the position of the agent when passing to the portal
	Vector getNewPositionThrowPortal(DynamicAgent<Kernel, Point, Vector>*) const;

protected:
	ActiveDelimitation<Kernel, Point, Vector>* in;  ///< \brief The entry of the portal
	ActiveDelimitation<Kernel, Point, Vector>* out; ///< \brief The exit of the portal
};

////////////////////// FUNCTION DEFINITION //////////////////

/// \param pIn 	The entry of the portal
/// \param pOut The exit of the portal
template <typename Kernel, typename Point, typename Vector>
Portal<Kernel, Point, Vector>::Portal(ActiveDelimitation<Kernel, Point, Vector>* pIn, ActiveDelimitation<Kernel, Point, Vector>* pOut):
	Writable(),
	in(pIn), out(pOut)
{
	/// \todo : check in et out are comparable ( same shape...)
}

/// \brief function to know if the agent is going to go throw the portal
template <typename Kernel, typename Point, typename Vector>
bool Portal<Kernel, Point, Vector>::isGoingThrowPortal(DynamicAgent<Kernel, Point, Vector>* pDynAgent) const
{
	/// \todo
	return false;
}

/// \brief return the position of the agent when passing to the portal
template <typename Kernel, typename Point, typename Vector>
Vector Portal<Kernel, Point, Vector>::getNewPositionThrowPortal(DynamicAgent<Kernel, Point, Vector>*) const {
	/// \todo 
	return {};
}

#endif
