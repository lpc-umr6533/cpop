/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "DiscsSDelimitation.hh"

#include "CGAL_Utils.hh"
#include "Round_Shape.hh"
#include "RandomEngineManager.hh"
#include "Geometry_Utils_Circle.hh"

#ifndef NDEBUG
	#define DISCS_DELIM_DEBUG 0
#else
	#define DISCS_DELIM_DEBUG 0	// must always stay at 0
#endif

/////////////////////////////////////////////////////////////////////////////////////////////
/// get spot 2D
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
Point_2 DiscsSDelimitation<double, Point_2, Vector_2>::getSpot(Distribution::DistributionType pDistType) const
{
	Point_2 p;
	switch(pDistType)
	{
		case Distribution::RANDOM :
		{
			p = Utils::Geometry::Circle::getSpotOnDisc(externalRadius, origin, internalRadius);
			break;
		}
		case Distribution::NORMALE : 
		{
			/// \todo 
			break;
		}
		default :
		{

		}
	}

	if(DISCS_DELIM_DEBUG)
	{
		QString mess = "find a spot at " + QString::number(p.x()) + ", " + QString::number(p.y());
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "DiscsSDelimitation");
	}
	return p;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// get spot
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
Point_3 DiscsSDelimitation<double, Point_3, Vector_3>::getSpot(Distribution::DistributionType pDistType) const
{
	Point_3 p;
	switch(pDistType)
	{
		case Distribution::RANDOM :
		{
			p = Utils::Geometry::Circle::getSpotOnDisc(externalRadius, origin, internalRadius);
			break;
		}
		case Distribution::NORMALE : 
		{
			/// \todo
			break;
		}
		default:
		{

		}
	}

	if(DISCS_DELIM_DEBUG)
	{
		QString mess = "find a spot at " + QString::number(p.x()) + ", " + QString::number(p.y()) + ", 0.";
		InformationSystemManager::getInstance()->Message(InformationSystemManager::DEBUG_MES, mess.toStdString(), "DiscsSDelimitation");
	}
	return p;
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// hasIn 2D
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
bool DiscsSDelimitation<double, Point_2, Vector_2>::isIn(Point_2 p) const
{

	double squaredDistance = CGAL::squared_distance(p, origin);
	return (
		(squaredDistance >= (internalRadius*internalRadius) ) &&
		(squaredDistance <= (externalRadius*externalRadius) )
		);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// hasIn 3D
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
bool DiscsSDelimitation<double, Point_3, Vector_3>::isIn(Point_3 p) const
{
	double squaredDistance = CGAL::squared_distance(p, origin);
	return (
		(squaredDistance >= (internalRadius*internalRadius) ) &&
		(squaredDistance <= (externalRadius*externalRadius) ) &&
		(p.z() == 0. )
		);
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// create bounding box for 2D
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
BoundingBox<Point_2> DiscsSDelimitation<double, Point_2, Vector_2>::createBoundingBox()
{
	return BoundingBox<Point_2> ( 
				Point_2( origin.x() - externalRadius, origin.y() - externalRadius),
				Point_2( origin.x() - externalRadius, origin.y() - externalRadius)
				 );
}

/////////////////////////////////////////////////////////////////////////////////////////////
/// create bounding box for 3D
/////////////////////////////////////////////////////////////////////////////////////////////
template<>
BoundingBox<Point_3> DiscsSDelimitation<double, Point_3, Vector_3>::createBoundingBox()
{
	return BoundingBox<Point_3> ( 
				Point_3( origin.x() - externalRadius, origin.y() - externalRadius, origin.z() - externalRadius),
				Point_3( origin.x() + externalRadius, origin.y() + externalRadius, origin.z() + externalRadius )
				 );
}
