#ifndef CONFLICT_SOLVER_HH
#define CONFLICT_SOLVER_HH

#include "Agent.hh"

#include <vector>

/// \brief object used to solve conflit between agent. Called at each simulation step,
/// before updating data structures
/// @author Henri Payno
class ConflictSolver {
public:
	/// \brief destructor
	virtual ~ConflictSolver() = default;

	///\brief solve the pendante conflict with the Agents
	/// \param pAgent The agent we want to solve the conflict for
	[[nodiscard]] virtual bool solveConflict(std::vector<Agent*> pAgent) const = 0;
};

#endif
