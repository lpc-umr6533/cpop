/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GRID_HH
#define GRID_HH

#include "RandomEngineManager.hh"
#include "GridElement.hh"
#include "GridElementTraits.hh"
#include "SpatialableAgent.hh"
#include "SpatialDelimitation.hh"

#include <vector>
#include <set>

#ifndef NDEBUG
 	#define DEBUG_GRID 0
#else
	#define DEBUG_GRID 1	// must always stay at 0
#endif

using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////
/// \brief  The grid class : define a grid (matrix) with a finite number of element.
/// This class will be used to pick position inside and make sure they are picked once only.
/// Here is an example of 2D grid limited by a discSDelimitation
/// \image html grid2D.png
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
class Grid
{
	typedef SpatialableAgent<Kernel, Point, Vector> t_Spatialable_Agent;		///< \brief a spatia agent handle by the Grid
	typedef GridElement<Kernel, Point> t_GridElement;							///< \brief a Grid element

public:
	/// \brief constructor
	Grid(Point pCenter, GridElementTraits<Kernel> pElementTraits, vector<unsigned int> pGridDimensions);
	/// \brief destructor
	~Grid();
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
	Point center;								///< \brief the center of the grid

	vector<t_GridElement*> grid;				///< \brief the grid
	vector<unsigned int> gridDimensions;		///< \brief the grid dimension on each axis (nb element/cell )
	set<unsigned int> unlockedElements;			///< \brief the grid unlocked elements id
	
	GridElementTraits<Kernel> elementTraits;	///< \brief the traits of the grid elements.
};


/////////////////////////////////////////////////////////////////////////////////////////
//			Constructor
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
Grid<Kernel, Point, Vector>::Grid(Point pCenter, GridElementTraits<Kernel> pElementTraits, vector<unsigned int> pGridDimensions):
	center(pCenter),
	gridDimensions(pGridDimensions),
	elementTraits(pElementTraits)
{
	/// construct grid elements
	constructGrid();
	/// set all elements to unlocked
	releaseAllElmt();
}

/////////////////////////////////////////////////////////////////////////////////////////
//			destructor
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
Grid<Kernel, Point, Vector>::~Grid()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//			getNextUnlockElement()
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
GridElement<Kernel, Point>* Grid<Kernel, Point, Vector>::getNextUnlockElement()
{
	if(unlockedElements.size() > 0)
	{
		unsigned int lID = *unlockedElements.begin();
		return grid[lID];
	}else
	{
		return NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//			getRandomUnlockElement()
/// \return A random GridElement unlocked
/// \warning this will not lock the GridElement
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point, typename Vector>
GridElement<Kernel, Point>* Grid<Kernel, Point, Vector>::getRandomUnlockElement()
{
	if(unlockedElements.size() > 0)
	{
		return grid[*RandomEngineManager::getInstance()->pickRandom(unlockedElements.begin(), unlockedElements.end())];
	}else
	{
		return NULL;
	}
}

template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//		lock(GridElement<Kernel, Point>* pElmt)
/// \param pElmt the GridElement to lock
/// \return  0 if ok, 1 if already lock, 2 if not an element of this grid
/////////////////////////////////////////////////////////////////////////////////////////
/// history : removed sucess status of locking. Because too much time consuming
void Grid<Kernel, Point, Vector>::lock(GridElement<Kernel, Point>* pElmt)
{
	// if not existing set insure to end with the same container
	unlockedElements.erase(pElmt->getID());
}

template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//			unlock(GridElement<Kernel, Point>* pElmt)
/// \param pElmt the element to unlock
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::unlock(GridElement<Kernel, Point>* pElmt)
{
	// set insure unicity of Gridelement inside the set of unclock elements
	unlockedElements.insert(pElmt->getID());
}


template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//			releaseAllElmt()
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::releaseAllElmt()
{
	unlockedElements.clear();
	typename vector<t_GridElement*>::iterator itElmt;
	for(itElmt = grid.begin(); itElmt != grid.end(); ++itElmt)
	{
		unlock(*itElmt);
	} 
}

/// \cond
template <typename Kernel, typename Point, typename Vector>	// template class
template <typename IT>										// template function
/////////////////////////////////////////////////////////////////////////////////////////
//			distributePosition(std::set<t_Spatialable_Agent*> agts, GridElementPosition lGEPosition)
/// \param agts 		The agents for which we want to set the position
/// \param lGEPosition	The type of position on GridElement we want to set
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::distributePosition(IT pITBegin, IT pITEnd, GridElementPosition lGEPosition)
{

	IT itAgt = pITBegin;
	while((itAgt != pITEnd) && (unlockedElements.size() > 0))
	{
		assert(dynamic_cast<t_Spatialable_Agent*>(*itAgt));
		distributePosition(dynamic_cast<t_Spatialable_Agent*>(*itAgt), lGEPosition);
		++itAgt;
	}

	if(itAgt == pITEnd)
	{
		return;
	}else
	{
		// all agent given haven't a position into the grid
		InformationSystemManager::getInstance()->Message(	
			InformationSystemManager::CANT_PROCESS_MES, 
			"grid does not contained enought element to attribute a position to all agent, some will be setted at origin", 
			"Grid");
	}
}
/// \endcond

template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//			distributePosition(t_Spatialable_Agent* agt, GridElementPosition lGEPosition)
/// \param agt 			The agent for which we want to set the position
/// \param lGEPosition	The type of position on GridElement we want to set
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::distributePosition(t_Spatialable_Agent* agt, GridElementPosition lGEPosition)
{
	assert(agt);
	t_GridElement* element = getRandomUnlockElement();
	/// if no element, no set of position
	if(element == NULL)
	{
		InformationSystemManager::getInstance()->Message(	
			InformationSystemManager::CANT_PROCESS_MES,
			"no element remaining unlocked, avoid setting position",
			"Grid");
		return;
	}

	lock(element);
	agt->setPosition( element->getPosition(lGEPosition) );
}

template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//			getRandomPos()
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::constructGrid()
{
	InformationSystemManager::getInstance()->Message(
		InformationSystemManager::CANT_PROCESS_MES, 
		"Failed to construct grid with this templates parameters", 
		"Grid");
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


template <typename Kernel, typename Point, typename Vector>
/////////////////////////////////////////////////////////////////////////////////////////
//			getRandomPos()
/// \param pSpaDeli The spatialDelimiation used to "limited" the Grid
/////////////////////////////////////////////////////////////////////////////////////////
void Grid<Kernel, Point, Vector>::applySpatialDelimitation(const SpatialDelimitation<Kernel, Point, Vector>* pSpaDeli)
{
	std::vector<unsigned int> toRemove;
	typename set<unsigned int>::iterator itElmt;
	for(itElmt = unlockedElements.begin(); itElmt != unlockedElements.end(); ++itElmt )
	{
		if(!pSpaDeli->isIn(grid[*itElmt]->getCenter()))
		{
			toRemove.push_back(*itElmt);
		}
	}

	for(unsigned int iToRmv = 0; iToRmv < toRemove.size(); iToRmv++)
	{
		lock(grid[toRemove[iToRmv]]);
	}

	if(DEBUG_GRID)
	{
		cout << "after applying spatial delimitation : nb unlock element = " << unlockedElements.size() << endl;
	}
}

#endif // GRID_HH