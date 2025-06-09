#ifndef GRID_HH
#define GRID_HH

#include "RandomEngineManager.hh"
#include "GridElement.hh"
#include "GridElementTraits.hh"
#include "SpatialableAgent.hh"
#include "SpatialDelimitation.hh"

#include <utility>
#include <vector>
#include <set>

#ifndef NDEBUG
 	#define DEBUG_GRID 0
#else
	#define DEBUG_GRID 1	// must always stay at 0
#endif

/// \brief  The grid class : define a grid (matrix) with a finite number of element.
/// This class will be used to pick position inside and make sure they are picked once only.
/// Here is an example of 2D grid limited by a discSDelimitation
/// \image html grid2D.png
/// @author Henri Payno
template <typename Kernel, typename Point, typename Vector>
class Grid {
	using t_Spatialable_Agent = SpatialableAgent<Kernel, Point, Vector>;  ///< \brief a spatia agent handle by the Grid
	using t_GridElement = GridElement<Kernel, Point>;                     ///< \brief a Grid element

public:
	Grid(Point pCenter, GridElementTraits<Kernel> pElementTraits, std::vector<unsigned int> pGridDimensions);

	/// \brief return the next unlocked element
	t_GridElement* getNextUnlockElement();
	/// \brief return the next unlocked element
	t_GridElement* getRandomUnlockElement();
	/// \brief unlock the setted element
	void unlock(t_GridElement* pElmt);
	/// \brief will lock the element
	void lock(t_GridElement* pElmt);
	/// \brief will make the grid match with the spatial delimitation. Each center of the grid must be inside the spatial delimitations.
	/// Otherwise we will lock the GridElement.
	/// After this operation each GridElement unlock has his center inside of the given spatial delimitations.
	void applySpatialDelimitation(const SpatialDelimitation<Kernel, Point, Vector>* );

	/// \brief return the number of unlocked elemets
	unsigned int getNumberOfUnlockElement()	{ return unlockedElements.size();};
	/// \brief reset all Gridelement to unlocked
	void releaseAllElmt();
	/// \brief set spatial distribution according to the unlock elements resting on the grid
	template<typename IT>
	void distributePosition(IT, IT, GridElementPosition lGEPosition);
	/// \brief set spatial distribution according to the unlock elements resting on the grid
	void distributePosition(t_Spatialable_Agent* agt, GridElementPosition lGEPosition);
	/// \brief construct the grid
	void constructGrid();

private:
	Point _center;								///< \brief the center of the grid

	std::vector<t_GridElement*> grid;				///< \brief the grid
	std::vector<unsigned int> _gridDimensions;		///< \brief the grid dimension on each axis (nb element/cell )
	std::set<unsigned int> unlockedElements;			///< \brief the grid unlocked elements id

	GridElementTraits<Kernel> _elementTraits;	///< \brief the traits of the grid elements.
};

template <typename Kernel, typename Point, typename Vector>
Grid<Kernel, Point, Vector>::Grid(Point pCenter, GridElementTraits<Kernel> pElementTraits, std::vector<unsigned int> pGridDimensions):
	_center(pCenter),
	_gridDimensions(std::move(pGridDimensions)),
	_elementTraits(pElementTraits)
{
	constructGrid();
	releaseAllElmt();
}

template <typename Kernel, typename Point, typename Vector>
GridElement<Kernel, Point>* Grid<Kernel, Point, Vector>::getNextUnlockElement() {
	if(unlockedElements.size() > 0) {
		unsigned int lID = *unlockedElements.begin();
		return grid[lID];
	} else {
		return nullptr;
	}
}

/// \return A random GridElement unlocked
/// \warning this will not lock the GridElement
template <typename Kernel, typename Point, typename Vector>
GridElement<Kernel, Point>* Grid<Kernel, Point, Vector>::getRandomUnlockElement() {
	if(unlockedElements.size() > 0) {
		return grid[*RandomEngineManager::getInstance()->pickRandom(unlockedElements.begin(), unlockedElements.end())];
	} else {
		return nullptr;
	}
}

/// \param pElmt the GridElement to lock
/// \return  0 if ok, 1 if already lock, 2 if not an element of this grid
/// history : removed sucess status of locking. Because too much time consuming
template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::lock(GridElement<Kernel, Point>* pElmt) {
	// if not existing set insure to end with the same container
	unlockedElements.erase(pElmt->getID());
}

/// \param pElmt the element to unlock
template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::unlock(GridElement<Kernel, Point>* pElmt) {
	// set insure unicity of Gridelement inside the set of unclock elements
	unlockedElements.insert(pElmt->getID());
}

template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::releaseAllElmt() {
	unlockedElements.clear();
	for(auto itElmt = grid.begin(); itElmt != grid.end(); ++itElmt)
		unlock(*itElmt);
}

/// \cond
/// \param agts 		The agents for which we want to set the position
/// \param lGEPosition	The type of position on GridElement we want to set
template <typename Kernel, typename Point, typename Vector>	// template class
template <typename IT>										// template function
void Grid<Kernel, Point, Vector>::distributePosition(IT pITBegin, IT pITEnd, GridElementPosition lGEPosition) {
	IT itAgt = pITBegin;
	while((itAgt != pITEnd) && (unlockedElements.size() > 0)) {
		assert(dynamic_cast<t_Spatialable_Agent*>(*itAgt));
		distributePosition(dynamic_cast<t_Spatialable_Agent*>(*itAgt), lGEPosition);
		++itAgt;
	}

	if(itAgt == pITEnd) {
		return;
	} else {
		// all agent given haven't a position into the grid
		InformationSystemManager::getInstance()->Message(
			InformationSystemManager::CANT_PROCESS_MES,
			"grid does not contained enought element to attribute a position to all agent, some will be setted at origin",
			"Grid"
		);
	}
}
/// \endcond

/// \param agt 			The agent for which we want to set the position
/// \param lGEPosition	The type of position on GridElement we want to set
template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::distributePosition(t_Spatialable_Agent* agt, GridElementPosition lGEPosition) {
	assert(agt);
	t_GridElement* element = getRandomUnlockElement();
	/// if no element, no set of position
	if(element == NULL) {
		InformationSystemManager::getInstance()->Message(
			InformationSystemManager::CANT_PROCESS_MES,
			"no element remaining unlocked, avoid setting position",
			"Grid"
		);
		return;
	}

	lock(element);
	agt->setPosition( element->getPosition(lGEPosition) );
}

template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::constructGrid() {
	InformationSystemManager::getInstance()->Message(
		InformationSystemManager::CANT_PROCESS_MES,
		"Failed to construct grid with this templates parameters",
		"Grid"
	);
}

////////// GetRandom position specialization ///////

/// \cond
// 2D
template<>
void Grid<double, Point_2, Vector_2>::constructGrid();

// 3D
template<>
void Grid<double, Point_3, Vector_3>::constructGrid();
/// \endcond

/// \param pSpaDeli The spatialDelimiation used to "limited" the Grid
template <typename Kernel, typename Point, typename Vector>
void Grid<Kernel, Point, Vector>::applySpatialDelimitation(const SpatialDelimitation<Kernel, Point, Vector>* pSpaDeli) {
	std::vector<unsigned int> toRemove;
	for(auto const& unlockedElement : unlockedElements) {
		if(!pSpaDeli->isIn(grid[unlockedElement]->getCenter()))
			toRemove.push_back(unlockedElement);
	}

	for(auto const& iToRmv : toRemove)
		lock(grid[iToRmv]);
}

#endif
