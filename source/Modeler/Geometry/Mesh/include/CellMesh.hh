#ifndef CELL_MESH_HH
#define CELL_MESH_HH

#include "CellProperties.hh"

/// \brief define properties to define a mesh of cell
/// @author Henri Payno
class CellMesh
{
public:
	virtual ~CellMesh() = default;

	/// \brief cell properties setter.
	void setCellProperties(const CellProperties* pCellProp) { cellProp = pCellProp; }

	/// \brief check if the mesh is valid <=> no mesh recovery
	bool isValid();

protected:
	const CellProperties* cellProp; ///< The properties of the cell to export. For now only one type to not repeat this on each cell.

};

#endif
