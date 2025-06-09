#ifndef POSITION_CONFLICT_SOLVER_HH
#define POSITION_CONFLICT_SOLVER_HH

#include "DynamicAgent.hh"
#include "ConflictSolver.hh"

#define NB_MAX_ITER_TO_SOLVE_SPA_POS 100

/// \brief The agent class : define rules to solve conflict between DYNAMIC AGENT ONLY and make
/// sure two dynamic agent aren't at the same spot.
/// \warning this solver only affect dynamic agent.
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class SpatialConflictSolver : public ConflictSolver {
public:
	/// \brief solve the pendante conflict with the Agents
	[[nodiscard]] inline bool solveConflict(std::vector<Agent*> agent) const override;
};

//////////////////// FUNCTION DEFINITIONS ///////////////////////////////////

/// \param agents The agents we have to solve conflict for
/// \return true if conflict solving succeded
template<typename Kernel, typename Point, typename Vector>
bool SpatialConflictSolver<Kernel, Point, Vector>::solveConflict(std::vector<Agent*> agents) const {
	std::set<Point> alreadySettedPosition;

	/// cast agent to dynamic
	/// set none dynamic agent first.
	using DynamicAgentKPV = DynamicAgent<Kernel, Point, Vector>;
	using DynamicAgents = std::set<DynamicAgentKPV*>;
	DynamicAgents dynamicAgents;

	for(auto const& iniAgt: agents) {
		auto const* dymAgt = dynamic_cast<DynamicAgentKPV const*>(iniAgt);
		if(dymAgt) {
			// if doesn't require a new position, he is prioritary
			if(!dymAgt->isRequiringNewPos())
				alreadySettedPosition.insert(dymAgt->getPosition());
			else
				dynamicAgents.insert(dymAgt);
		}
	}

	/// check position
	for(auto const& agent: dynamicAgents) {
		if(alreadySettedPosition.find(agent->getRequestedPosition()) == alreadySettedPosition.end())
			alreadySettedPosition.insert(agent->getRequestedPosition());
		else {
			// get a random point on "journey"/ flight
			double ratio;
			Vector v(agent->getPosition() - agent->getRequestedPosition());
			unsigned int nbIter = 0;
			while((alreadySettedPosition.find(agent->getPosition() + v*ratio) != alreadySettedPosition.end() ) &&
					(nbIter < NB_MAX_ITER_TO_SOLVE_SPA_POS)) {
				ratio = ratio / 2.;
				nbIter++;
			}

			if(nbIter >= NB_MAX_ITER_TO_SOLVE_SPA_POS ) {
				std::cout << " too much conflict " << std::endl;
				return false;
			}

			Point newPos = agent->getPosition() + v*ratio;
			agent->setRequiredPosition(newPos);
			alreadySettedPosition.insert(newPos);
		}
	}
	return true;
}

#endif
