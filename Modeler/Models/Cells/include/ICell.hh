/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef I_CELL_HH
#define I_CELL_HH

#include "CellProperties.hh"
#include "Dimensioned_Layer.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// \brief The ICell class is a pure virtual class, dealing with untemplated cell proeprties. 
/// such as CellProperties or age of the cell.
/// It add benn defined to simplify some cell management, not depending if the cell is 2D or 3D.
/// @author Henri Payno
////////////////////////////////////////////////////////////////////////////////////////////////////
class ICell
{
public:
	/// \brief constructor
	ICell(const CellProperties*);
	/// \brief destructor
	~ICell();
	
	/// \brief CellProperties getter
	const CellProperties* getCellProperties() const 	{ return cellProperties;};
	/// \brief CellProperties setter
	/// \param pCellProp the cell properties to set
	void setCellProperties( CellProperties* pCellProp )	{ cellProperties = pCellProp;};

	/// \brief return the cell dimension
	/// \return the dimension of the cell ( 2D, 3D...)
	virtual Dimension getDimension() const = 0;

protected:
	const CellProperties* cellProperties;	///< \brief the cell properties
	
};


#endif // I_CELL_HH