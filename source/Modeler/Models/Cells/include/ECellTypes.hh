/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_TYPES_HH
#define CELL_TYPES_HH

#include <QString>

////////////////////////////////////////////////////////////////////////////
/// \brief define the cell defined types
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////
enum CellType
{
	SIMPLE_ROUND, 	///< \brief a simple round cell (a shape and a nucleus)
	UNKNOW 			///< \warning unkonw type must always be set last.
};

/// \brief return the name given to the cell type
QString getCellTypeName(CellType type);

#endif // CELL_TYPES_HH