/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_COMPOSITION_HH
#define CELL_COMPOSITION_HH

////////////////////////////////////////////////////////////////////////////
/// \brief define a structure of a cell
////////////////////////////////////////////////////////////////////////////
namespace CellComposition
{
	/// \brief list all potential Organelle of the cell
	enum Organelle
	{
		_CELL_MEMBRANE,		///< define the spatial limitation of the cell. For now is a "line/PLANE" <=> no width
		_CYTOPLASM,			///< All the "inside" of the cell minus other Organelle
		_NUCLEAR_MEMBRANE,
		_NUCLEOPLASM
	};
}

#endif // CELL_COMPOSITION_HH
