/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef POSITION_CONFLICT_SOLVER_HH
#define POSITION_CONFLICT_SOLVER_HH

#include "DynamicAgent.hh"
#include "ConflictSolver.hh"

#define NB_MAX_ITER_TO_SOLVE_SPA_POS 100

/////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The agent class : define rules to solve conflict between DYNAMIC AGENT ONLY and make
/// sure two dynamic agent aren't at the same spot.
/// \warning this solver only affect dynamic agent.
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class SpatialConflictSolver : public ConflictSolver
{
public:
	/// \brief constructor
	SpatialConflictSolver();
	/// \brief destructor
	~SpatialConflictSolver();
	/// \brief solve the pendante conflict with the Agents
	inline virtual bool solveConflict(std::vector<Agent*> agent) const;

};

//////////////////// FUNCTION DEFINITIONS ///////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialConflictSolver<Kernel, Point, Vector>::SpatialConflictSolver()
{

}

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
SpatialConflictSolver<Kernel, Point, Vector>::~SpatialConflictSolver()
{

}

/////////////////////////////////////////////////////////////////////////////
/// \param agents The agents we have to solve conflict for
/// \return true if conflict solving succeded
/////////////////////////////////////////////////////////////////////////////
template<typename Kernel, typename Point, typename Vector>
bool SpatialConflictSolver<Kernel, Point, Vector>::solveConflict(std::vector<Agent*> agents) const
{
	std::set<Point> alreadySettedPosition;

	/// cast agent to dynamic
	/// set none dynamic agent first.
	typename std::set<DynamicAgent<Kernel, Point, Vector>* > dynamicAgents;
	std::vector<Agent*>::iterator itIniAgt;
	for(itIniAgt = agents.begin(); itIniAgt != agents.end(); ++itIniAgt)
	{
		DynamicAgent<Kernel, Point, Vector>*  dymAgt = dynamic_cast<DynamicAgent<Kernel, Point, Vector>* >(*itIniAgt);
		if(dymAgt)
		{
			// if doesn't require a new position, he is prioritary
			if(!dymAgt->isRequiringNewPos())
			{
				alreadySettedPosition.insert(dymAgt->getPosition());
			}else
			{
				dynamicAgents.insert(dymAgt);
			}
		}
	}

	/// check position
	typename std::set<DynamicAgent<Kernel, Point, Vector>* >::iterator itAgent;
	for(itAgent = dynamicAgents.begin(); itAgent != dynamicAgents.end(); ++itAgent)
	{
		if(alreadySettedPosition.find((*itAgent)->getRequestedPosition()) == alreadySettedPosition.end())
		{
			alreadySettedPosition.insert((*itAgent)->getRequestedPosition());
		}else
		{
			// get a random point on "journey"/ flight
			double ratio;
			Vector v((*itAgent)->getPosition() -  (*itAgent)->getRequestedPosition());
			unsigned int nbIter = 0;
			while( 	(alreadySettedPosition.find((*itAgent)->getPosition() + v*ratio) != alreadySettedPosition.end() ) &&
					( nbIter < NB_MAX_ITER_TO_SOLVE_SPA_POS ) )
			{
				ratio = ratio / 2.;
				nbIter++;
			}
			
			if(nbIter >= NB_MAX_ITER_TO_SOLVE_SPA_POS )
			{
				std::cout << " too much conflict " << std::endl;
				return false;
			}

			Point newPos = (*itAgent)->getPosition() + v*ratio;
			(*itAgent)->setRequiredPosition(newPos);
			alreadySettedPosition.insert(newPos);
		}
	}
	return true;
}

#endif // POSITION_CONFLICT_SOLVER_HH