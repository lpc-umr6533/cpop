/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GRID_ELEMENT_HH
#define GRID_ELEMENT_HH

#include "GridElementTraits.hh"

#ifndef NULL
	#include <stddef.h>
#endif

#include "InformationSystemManager.hh"
#include "GeometrySettings.hh"

#ifndef NDEBUG
 	#define DEBUG_GRID_ELEMENT 0
#else
	#define DEBUG_GRID_ELEMENT 0	// must always stay at 0
#endif

using namespace Settings::Geometry;
using namespace std;

/// \brief define the posible positioning on gridElement
enum GridElementPosition
{
	GEP_RANDOM,	///< \brief a random position inside the grid element
	GEP_CENTER	///<\The center/barycenter of the grid element
};

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief  The grid element class : deifne the elements contained by the grid.
/// In order to make sure elements compatible with distribution, grid element can return 
/// spots inside themself.
/// Element are rectangular / box spots/
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point>
class GridElement
{

public:
	/// \brief constructor
	GridElement(const GridElementTraits<Kernel>*, Point, unsigned int );
	/// \brief destructor
	~GridElement();
	
	Point getPosition(GridElementPosition) const;
	/// \brief return the center of the element / cell
	Point getCenter() 		const					{return center;};
	/// \brief return a random position inside the element
	Point getRandomPos() 	const ;
	/// \brief return a random spot on the element / cell
	/// Point getRandomSpot(); \todo
	// /// \brief true if the cell / element is not locked yet
	// bool isFree()		{return bFree;};
	// /// \brief lock the element
	// void lock()			{ bFree = false;};
	// /// \brief unlock / set free the element
	// void unlock()		{ bFree = true;};

	/// \brief return the position on the grid
	unsigned int getID() const 	{ return idOnTheGrid;}

private:
	// bool bFree;									///< \brief boolean to check if the "element is free or not"
	unsigned int idOnTheGrid;
	Point center;								///< \brief the center of the element/cell
	const GridElementTraits<Kernel>* traits;	///< \brief the element trait
};



/////////////////////////////////////// implementation ///////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
//			Destructor
/// \param pTraits the properties/specificities of the Grid element
/// \param pCenter the center of the Grid Element
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point>
GridElement<Kernel, Point>::GridElement(const GridElementTraits<Kernel>* pTraits, Point pCenter, unsigned int pID):
	// bFree(true),
	idOnTheGrid(pID),
	center(pCenter),
	traits(pTraits)
{
#if DEBUG_GRID_ELEMENT
	cout << " grid element created at : " << pCenter << endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////
//			Constructor
///
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point>
GridElement<Kernel, Point>::~GridElement()
{
	traits = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
//			getPos()
/// \param lGEPosition the position type we want to get on the gridElement
/// \return the position requested
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point>
Point GridElement<Kernel, Point>::getPosition(GridElementPosition lGEPosition) const
{
	switch(lGEPosition)
	{
		case GEP_RANDOM :
		{
			Point randomPos = getRandomPos();
#if DEBUG_GRID_ELEMENT
			cout << "getPosition : grid at : " << center << " returning : pos : " << randomPos << endl;
#endif
			return randomPos;
		}
		case GEP_CENTER :
		{
#if DEBUG_GRID_ELEMENT
			cout << "getPosition : grid at : " << center << " returning : pos : " << getCenter() << endl;
#endif			
			return getCenter();
		}
		default:
		{
			InformationSystemManager::getInstance()->Message(	
				InformationSystemManager::CANT_PROCESS_MES,	
				"unknow GridElement position type",
				"GridElement");
			return Point();
		}
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//			getRandomPos()
/// \return A random position inside the GridElement
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel, typename Point>
Point GridElement<Kernel, Point>::getRandomPos() const
{
	InformationSystemManager::getInstance()->Message(
		InformationSystemManager::CANT_PROCESS_MES, 
		"Failed to give a random position for this templates parameters", 
		"GridElement");
	return Point();
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

#endif // GRID_ELEMENT_HH