#ifndef CELL_TYPES_HH
#define CELL_TYPES_HH

#include <QString>

/// \brief define the cell defined types
/// @author Henri Payno
enum CellType {
	SIMPLE_ROUND,   ///< \brief a simple round cell (a shape and a nucleus)
	UNKNOW          ///< \warning unkonw type must always be set last.
};

/// \brief return the name given to the cell type
QString getCellTypeName(CellType type);

#endif
