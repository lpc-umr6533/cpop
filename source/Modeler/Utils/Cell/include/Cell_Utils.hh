/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_UTILS_HH
#define CELL_UTILS_HH

#include "ICell.hh"
#include "BoundingBox.hh"
#include "CellProperties.hh"
#include "GeometrySettings.hh"
#include "CellSettings.hh"
#include "SpheroidalCell.hh"

#include <CGAL/bounding_box.h>
#include <CGAL/centroid.h>

#include <vector>
#include <set>

using namespace std;
using namespace Settings::nCell;
using namespace Settings::Geometry;

//////////////////////////////////////////////////////////////////////////////
/// @namespace Utils
//////////////////////////////////////////////////////////////////////////////	
namespace Utils
{
	/// \brief return all CellProperties stored on agents
	set<const CellProperties*> getAllCellsProperties(vector<const ICell*> pAgents);
	/// \brief generate life state rates uniformaly
	map<LifeCycles::LifeCycle, double> generateUniformLifeCycle();	
	
	/// \brief return the centroid of the given cells.
	/// \param pItStart the first iterator position
	/// \param pItEnd the last iterator position 
	/// \return the centroid of given cells
	template<typename TIteratorOnConstCell>
	Point_3 getCentroid(TIteratorOnConstCell pItStart, TIteratorOnConstCell pItEnd)
	{
		vector<Point_3> cellsPositions;
		TIteratorOnConstCell it;
		for(it = pItStart; it != pItEnd; ++it)
		{
			cellsPositions.push_back((*it)->getPosition());
		}
		return CGAL::centroid(cellsPositions.begin(), cellsPositions.end(), CGAL::Dimension_tag<0>());
	}

	/// \brief recenter all cells to the given position
	template<typename TIteratorOnCell>
	void setBarycenter(TIteratorOnCell pItStart, TIteratorOnCell pItEnd, Point_3 pNewBary)
	{
		Point_3 lCurrentBary = getCentroid(pItStart, pItEnd);
		Vector_3 lTransformation = pNewBary - lCurrentBary;
		TIteratorOnCell it;
		for(it = pItStart; it != pItEnd; ++it)
		{
			Point_3 lNewPos =  (*it)->getPosition() + lTransformation;
			(*it)->setPosition( lNewPos );
		}
	}

	/// \brief return the nearest and the farthest cell from the given point.
	/// \param ptConsidered the point for which we want the nearest and farthest cell
	/// \param pItStart the first iterator position
	/// \param pItEnd the last iterator position 
	/// \param nearestCell the nearest cell iterator. Setted at pItEnd position if none found.
	/// \param farthestCell the farthest cell iterator. Setted at pItEnd position if none found.
	template<typename Point, typename TIteratorOnConstCell, typename TCell>
	void getNearestAndFarthestCells(Point ptConsidered, TIteratorOnConstCell pItStart, TIteratorOnConstCell pItEnd, TCell nearestCell, TCell farthestCell)
	{
		double nearestDistance = std::numeric_limits<double>::max();
		double farthestDistance = -1.;
		
		TIteratorOnConstCell it = pItStart;
		while(it != pItEnd )
		{
			assert(*it);
			double squareDist = CGAL::squared_distance( (*it)->getPosition(), ptConsidered );
			if(squareDist < nearestDistance)
			{
				nearestDistance = squareDist;
				nearestCell = *it;
			}

			if(squareDist > farthestDistance)
			{
				farthestDistance = squareDist;
				farthestCell = *it;
			}
			++it;
		}
	}


	/// \brief return the nearest and the farthest cell from the given point.
	/// \param ptConsidered the point for which we want the nearest and farthest cell
	/// \param pItStart the first iterator position
	/// \param pItEnd the last iterator position 
	/// \param nearestCell the nearest cell iterator. Setted at pItEnd position if none found.
	/// \param farthestCell the farthest cell iterator. Setted at pItEnd position if none found.
	template<typename Point, typename TIteratorOnConstCell>
	void getNearestAndFarthestPoints(Point ptConsidered, TIteratorOnConstCell pItStart, TIteratorOnConstCell pItEnd, double &nearest, double &farthest)
	{
		double nearestDistance = std::numeric_limits<double>::max();
		double farthestDistance = -1.;
		
		TIteratorOnConstCell it = pItStart;
		while(it != pItEnd )
		{
			assert(*it);
			double squareDist = CGAL::squared_distance( (*it)->getPosition(), ptConsidered );
			if(squareDist < nearestDistance)
			{
				nearestDistance = squareDist;
			}

			if(squareDist > farthestDistance)
			{
				farthestDistance = squareDist;
			}
			++it;
		}
		nearest = sqrt(nearestDistance);
		farthest = sqrt(farthestDistance);
	}	

	//////////////////////////////////////////////////////////////////////////////
	///
	//////////////////////////////////////////////////////////////////////////////
	template<typename IIt>
	BoundingBox<Point_3> getBoundingBox(IIt begin, IIt end)
	{
		IIt it;
		double maxCellRoundCellRadius = 0;
		vector<Point_3> pts;
		for(it = begin; it != end; ++it)
		{
			const SpheroidalCell* lCell = dynamic_cast<const SpheroidalCell*> (*it);
			if(lCell)
			{
				maxCellRoundCellRadius = max(lCell->getRadius(), maxCellRoundCellRadius);
			}
			pts.push_back((*it)->getPosition());
		}

		K::Iso_cuboid_3 bb = CGAL::bounding_box( pts.begin(), pts.end());
		// add the potential next point from the voronoi cell and sphere shape
		return BoundingBox<Point_3>( 
			Point_3(bb.xmin()-2*maxCellRoundCellRadius, bb.ymin()-2*maxCellRoundCellRadius, bb.zmin()-2*maxCellRoundCellRadius),
			Point_3(bb.xmax()+2*maxCellRoundCellRadius, bb.ymax()+2*maxCellRoundCellRadius, bb.zmax()+2*maxCellRoundCellRadius) );	// for now weight correspond to the cell radius.
																												// * 2 because is own radius and the radius of bounding point
	}
}


#endif // CELL_UTILS_HH
