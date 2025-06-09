#ifndef CELL_POPULATION_HH
#define CELL_POPULATION_HH

#include "BoundingBox.hh"
#include "Cell.hh"

/// \todo : set is writable
/// \brief represent a set of cells
template <typename Kernel, typename Point, typename Vector>
class CellPopulation {
public:
	CellPopulation(std::set<Cell<Kernel, Point, Vector>*> pCells = std::set<Cell<Kernel, Point, Vector>*>());
	virtual ~CellPopulation() = default;

	/// \brief return cell part of the cell population
	virtual std::set<Cell<Kernel, Point, Vector>*> getCells() const 	{ return cells;}
	/// \brief cells setter
	void setCells( std::set<Cell<Kernel, Point, Vector>*> pCells );
	/// \brief add a cell to the cell population
	void addCell(Cell<Kernel, Point, Vector>*);
	/// \brief compute and return the minimal bounding box the cell population is including in
	virtual BoundingBox<Point> getBoundingBox() const = 0;

protected:
	std::set<Cell<Kernel, Point, Vector>*> cells;		///< \brief cell registred to the cell population
};

template <typename Kernel, typename Point, typename Vector>
CellPopulation<Kernel, Point, Vector>::CellPopulation(std::set<Cell<Kernel, Point, Vector>*> pCells):
	cells(pCells)
{
}

/// \param pCell The cell to add to the cell population
template <typename Kernel, typename Point, typename Vector>
void CellPopulation<Kernel, Point, Vector>::addCell(Cell<Kernel, Point, Vector>* pCell) {
	assert(pCell);
	cells.insert(pCell);
}

/// \param pCells The cells to add to the cell population
template <typename Kernel, typename Point, typename Vector>
void CellPopulation<Kernel, Point, Vector>::setCells( std::set<Cell<Kernel, Point, Vector>*> pCells) {
	for(auto itCell = pCells.begin(); itCell!= pCells.end(); itCell++)
		addCell(*itCell);
}

#endif
