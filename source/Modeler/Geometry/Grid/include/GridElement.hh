#ifndef GRID_ELEMENT_HH
#define GRID_ELEMENT_HH

#include "GridElementTraits.hh"

#ifndef NULL
	#include <cstddef>
#endif

#include "InformationSystemManager.hh"
#include "GeometrySettings.hh"

#ifndef NDEBUG
 	#define DEBUG_GRID_ELEMENT 0
#else
	#define DEBUG_GRID_ELEMENT 0	// must always stay at 0
#endif

using namespace Settings::Geometry;

/// \brief define the posible positioning on gridElement
enum GridElementPosition {
	GEP_RANDOM,	///< \brief a random position inside the grid element
	GEP_CENTER	///<\The center/barycenter of the grid element
};

/// \brief  The grid element class : deifne the elements contained by the grid.
/// In order to make sure elements compatible with distribution, grid element can return 
/// spots inside themself.
/// Element are rectangular / box spots/
/// @author Henri Payno
template <typename Kernel, typename Point>
class GridElement {
public:
	GridElement(const GridElementTraits<Kernel>*, Point, unsigned int);
	
	[[nodiscard]] Point getPosition(GridElementPosition) const;
	/// \brief return the center of the element / cell
	[[nodiscard]] Point getCenter() const { return _center; }
	/// \brief return a random position inside the element
	[[nodiscard]] Point getRandomPos() const;
	/// \brief return a random spot on the element / cell
	/// Point getRandomSpot(); \todo

	/// \brief return the position on the grid
	[[nodiscard]] unsigned int getID() const { return _idOnTheGrid; }

private:
	unsigned int _idOnTheGrid;
	Point _center;                                      ///< \brief the center of the element/cell
	const GridElementTraits<Kernel>* _traits = nullptr; ///< \brief the element trait
};

/////////////////////////////////////// implementation ///////////////////////////////////

//			Destructor
/// \param pTraits the properties/specificities of the Grid element
/// \param pCenter the center of the Grid Element
template <typename Kernel, typename Point>
GridElement<Kernel, Point>::GridElement(const GridElementTraits<Kernel>* pTraits, Point pCenter, unsigned int pID):
	_idOnTheGrid(pID),
	_center(pCenter),
	_traits(pTraits)
{
#if DEBUG_GRID_ELEMENT
	std::cout << " grid element created at : " << pCenter << std::endl;
#endif
}

/// \param lGEPosition the position type we want to get on the gridElement
/// \return the position requested
template <typename Kernel, typename Point>
Point GridElement<Kernel, Point>::getPosition(GridElementPosition lGEPosition) const {
	switch(lGEPosition) {
		case GEP_RANDOM :
		{
			Point randomPos = getRandomPos();
#if DEBUG_GRID_ELEMENT
			std::cout << "getPosition : grid at : " << center << " returning : pos : " << randomPos << std::endl;
#endif
			return randomPos;
		}
		case GEP_CENTER :
		{
#if DEBUG_GRID_ELEMENT
			std::cout << "getPosition : grid at : " << center << " returning : pos : " << getCenter() << std::endl;
#endif			
			return getCenter();
		}
		default:
		{
			InformationSystemManager::getInstance()->Message(	
				InformationSystemManager::CANT_PROCESS_MES,	
				"unknow GridElement position type",
				"GridElement"
			);
			return Point();
		}
	}
}

/// \return A random position inside the GridElement
template <typename Kernel, typename Point>
Point GridElement<Kernel, Point>::getRandomPos() const {
	InformationSystemManager::getInstance()->Message(
		InformationSystemManager::CANT_PROCESS_MES, 
		"Failed to give a random position for this templates parameters", 
		"GridElement"
	);
	return {};
}

////////// GetRandom position specialization ///////

// 2D
/// \brief return a random 2D position inside the element
/// \return A random position inside the GridElement
template<>
Point_2 GridElement<double, Point_2>::getRandomPos() const;

// 3D
/// \brief return a random 3D position inside the element
/// \return A random position inside the GridElement
template<>
Point_3 GridElement<double, Point_3>::getRandomPos() const;

#endif
