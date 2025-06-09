#ifndef FORCE_HH
#define FORCE_HH

#include "EForceInputType.hh"
#include "SpatialDataStructureManager.hh"

template<typename Kernel, typename Point, typename Vector>
class Cell;

/// \brief The Force definition
/// define by his direction and force ( in N)
/// @author Henri Payno
template<typename Kernel, typename Point, typename Vector>
class Force {
public:
	Force(const Cell<Kernel, Point, Vector>* pCell, ForceInputType);
	virtual ~Force() = default;

	/// \brief return the force for the actual set up
	virtual Vector computeForce() const = 0;
	/// \brief return all the agent needed to apply the force
	virtual std::set<const SpatialableAgent<Kernel, Point, Vector>* > getConcernedAgent() const;

protected:
	/// \brief the cell appling the force
	const Cell<Kernel, Point, Vector>* _cell;
	/// \brief the type of agent concerned by the force
	ForceInputType _concernedAgt;
};

/// \param <pCell> {The cell we want to have the force for.}.
/// \param <pEAgtConcerned> {The type of agent location concerned by the force}
template<typename Kernel, typename Point, typename Vector>
Force<Kernel, Point, Vector>::Force(const Cell<Kernel, Point, Vector>* pCell, ForceInputType pEAgtConcerned) :
	_cell(pCell),
	_concernedAgt(pEAgtConcerned)
{
}

/// \return {The set of agent needed (concerned) to applyt the force}
template<typename Kernel, typename Point, typename Vector>
std::set<const SpatialableAgent<Kernel, Point, Vector>* > Force<Kernel, Point, Vector>::getConcernedAgent() const {
	/// \todo : change name fit : match with ? apply from ?
	assert(_cell);
	switch(_concernedAgt) {
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
			return SpatialDataStructureManager::getInstance()->getNeighbours(_cell);
		}
		default:
		{
			QString mess = "unable to get concerned agent by force for this kind of ForceInputType : "  + _concernedAgt;
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess.toStdString(), "Force");
			return std::set<const SpatialableAgent<Kernel, Point, Vector>* >();
		}
	}
}

#endif
