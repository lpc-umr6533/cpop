#include "Cell_Utils.hh"

namespace Utils {

/// \param pAgents The set of agent we want the properties for
/// \return set<const CellProperties*> The set of Cell properties present in giving cells.
std::set<const CellProperties*> getAllCellsProperties(std::vector<const ICell*> pAgents) {
	std::set<const CellProperties*> lCellProperties;

	std::vector<const ICell*>::const_iterator itCell;
	for(itCell = pAgents.begin(); itCell != pAgents.end(); ++itCell)
		lCellProperties.insert((*itCell)->getCellProperties());

	return lCellProperties;
}

/// \return LifeCyclesRates all existing life cycle with uniform rates
std::map<LifeCycles::LifeCycle, double> generateUniformLifeCycle() {
	std::map<LifeCycles::LifeCycle, double> rates;
	for(int iState = 0; iState != LifeCycles::NA; iState++)
		rates.insert(std::pair<LifeCycles::LifeCycle, double> (static_cast<LifeCycles::LifeCycle>(iState), 0.2));

	return rates;
}

/// \param pCells the cells for which we want to reset center
/// \param pNewBary The new location of the barycenter
/// \warning this will only affect cells position, and never their meshes. So if you need cell meshes with the
/// new cell positions please run again meshing process.
void setBarycenter(std::vector<t_Cell_3*>& pCells, Point_3 pNewBary)
{
	std::vector<const t_Cell_3*> lCstCells(pCells.begin(), pCells.end());
	Point_3 lCurrentBary = getCentroid(lCstCells.begin(), lCstCells.end());
	Vector_3 lTransformation = pNewBary - lCurrentBary;

	for(auto const& pCell : pCells) {
		Point_3 lNewPos =  pCell->getPosition() + lTransformation;
		pCell->setPosition( lNewPos );
	}
}

}
