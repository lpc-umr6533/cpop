/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CONFLICT_SOLVER_HH
#define CONFLICT_SOLVER_HH

#include "Agent.hh"

#include <vector>
//////////////////////////////////////////////////////////////////////////////
/// \brief object used to solve conflit between agent. Called at each simulation step, 
/// before updating data structures
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class ConflictSolver
{
public:
	/// \brief constructor
	ConflictSolver();
	/// \brief destructor
	virtual ~ConflictSolver();

	///\brief solve the pendante conflict with the Agents
	/// \param pAgent The agent we want to solve the conflict for
	virtual bool solveConflict(std::vector<Agent*> pAgent) const = 0;

};

#endif // CONFLICT_SOLVER_HH