/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FORCE_HH
#define FORCE_HH

#include "Cell.hh"
#include "EForceInputType.hh"
#include "SpatialDataStructureManager.hh"
#include "SimulationManager.hh"

template<typename Kernel, typename Point, typename Vector> 
class Cell;

//////////////////////////////////////////////////////////////////////////////
/// \brief The Force definition 
/// define by his direction and force ( in N)
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
class Force
{
public:
	/// \brief constructor
	Force( const Cell<Kernel, Point, Vector>* pCell, ForceInputType);
	/// \brief destructor
	virtual ~Force();
	/// \brief return the force for the actual set up
	virtual Vector computeForce() const = 0;
	/// \brief return all the agent needed to apply the force
	virtual std::set<const SpatialableAgent<Kernel, Point, Vector>* > getConcernedAgent() const;

protected:
	/// \brief the cell appling the force
	const Cell<Kernel, Point, Vector>* cell;
	/// \brief the type of agent concerned by the force
	ForceInputType concernedAgt;
};

////////////////////////////////////////////////////////////////////////////////
/// \param <pCell> {The cell we want to have the force for.}.
/// \param <pEAgtConcerned> {The type of agent location concerned by the force}
////////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Force<Kernel, Point, Vector>::Force(const Cell<Kernel, Point, Vector>* pCell, ForceInputType pEAgtConcerned) :
	cell(pCell),
	concernedAgt(pEAgtConcerned)
{

}

///////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
Force<Kernel, Point, Vector>::~Force()
{

}

///////////////////////////////////////////////////////////////////////////////
/// \return {The set of agent needed (concerned) to applyt the force}
///////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
std::set<const SpatialableAgent<Kernel, Point, Vector>* > Force<Kernel, Point, Vector>::getConcernedAgent() const
{
	/// \todo : change name fit : match with ? apply from ?
	assert(cell);
	switch(concernedAgt)
	{
		case FRCE_INTERACT_WITH_NONE:
		{
			return std::set<const SpatialableAgent<Kernel, Point, Vector>* >();
		}
		case FRCE_INTERACT_WITH_ALL:
		{
			/// \todo : ALL is all on the same layer ? , All on the simulation ???
			//return SimulationManager::getInstance()->getAllSpatialableAgents<Kernel, Point, Vector>();
		}
		case FRCE_INTERACT_WITH_NEIGHBOURS:
		{
			return SpatialDataStructureManager::getInstance()->getNeighbours(cell);
		}
		default:
		{
			QString mess = "unable to get concerned agent by force for this kind of ForceInputType : "  + concernedAgt;
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Force");
			return std::set<const SpatialableAgent<Kernel, Point, Vector>* >();
		}
	}
}

#endif // FORCE_HH
