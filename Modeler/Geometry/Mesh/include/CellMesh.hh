/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_MESH_HH
#define CELL_MESH_HH

#include "CellProperties.hh"
#include "ECellTypes.hh"
#include "Nucleus.hh"

//////////////////////////////////////////////////////////////////////////////
/// \brief define properties to define a mesh of cell
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
class CellMesh
{
public:
	/// \brief constructor
	CellMesh();
	/// \brief destructor
	virtual ~CellMesh();

	/// \brief cell properties setter.
	void setCellProperties(const CellProperties* pCellProp) 	{cellProp = pCellProp;};

	/// \brief check if the mesh is valid <=> no mesh recovery
	bool isValid();

protected:
	const CellProperties* cellProp;		///< The properties of the cell to export. For now only one type to not repeat this on each cell.
	
};

#endif // CELL_MESH_HH