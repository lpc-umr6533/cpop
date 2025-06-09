#ifndef I_CELL_HH
#define I_CELL_HH

#include "CellProperties.hh"
#include "Dimensioned_Layer.hh"

/// \brief The ICell class is a pure virtual class, dealing with untemplated cell proeprties. 
/// such as CellProperties or age of the cell.
/// It add benn defined to simplify some cell management, not depending if the cell is 2D or 3D.
/// @author Henri Payno
class ICell {
public:
	ICell(const CellProperties*);
	
	/// \brief CellProperties getter
	[[nodiscard]] const CellProperties* getCellProperties() const { return _cellProperties; }
	/// \brief CellProperties setter
	/// \param pCellProp the cell properties to set
	void setCellProperties( CellProperties* pCellProp)	{ _cellProperties = pCellProp; }

	/// \brief return the cell dimension
	/// \return the dimension of the cell ( 2D, 3D...)
	[[nodiscard]] virtual Dimension getDimension() const = 0;

protected:
	const CellProperties* _cellProperties;	///< \brief the cell properties
	
};

#endif
