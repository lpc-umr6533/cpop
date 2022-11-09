/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef LIFE_CYCLES_HH
#define LIFE_CYCLES_HH

#include <QString>

////////////////////////////////////////////////////////////////////////////
/// \brief define the life cycles
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
namespace LifeCycles
{
	/// \brief list of potential life cycle
	enum LifeCycle
	{
		G1,		///< Cells increase in size in Gap 1. The G1 checkpoint control mechanism ensures that everything is ready for DNA synthesis.
		S, 		///< DNA replication occurs during this phase.
		G2, 	///< During the gap between DNA synthesis and mitosis, the cell will continue to grow. The G2 checkpoint control mechanism ensures that everything is ready to enter the M (mitosis) phase and divide.
		M, 		///< Cell growth stops at this stage and cellular energy is focused on the orderly division into two daughter cells. A checkpoint in the middle of mitosis (Metaphase Checkpoint) ensures that the cell is ready to complete cell division.
		G0,		///< A resting phase where the cell has left the cycle and has stopped dividing.
		DEAD,	///< Cell is Dead, can't take bake any over state.
		NA		///< must always stay at the last position
	};

	/// \brief return the corresponding name to the life state
	QString getName(LifeCycle pLifeCycle);
}


#endif // LIFE_CYCLES_HH