/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "GridElement.hh"
#include "RandomEngineManager.hh"

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return A random position inside the GridElement
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Point_2 GridElement<double, Point_2>::getRandomPos() const
{
	assert(traits);
	return Point_2(
		RandomEngineManager::getInstance()->randd( center.x() - traits->getLengthOnAxis(AXIS_X) / 2., center.x() + traits->getLengthOnAxis(AXIS_X) / 2.),
		RandomEngineManager::getInstance()->randd( center.y() - traits->getLengthOnAxis(AXIS_Y) / 2., center.y() + traits->getLengthOnAxis(AXIS_Y) / 2.)
		);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \return A random position inside the GridElement
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Point_3 GridElement<double, Point_3>::getRandomPos() const
{
	assert(traits);
	return Point_3(
		RandomEngineManager::getInstance()->randd( center.x() - traits->getLengthOnAxis(AXIS_X) / 2., center.x() + traits->getLengthOnAxis(AXIS_X) / 2.),
		RandomEngineManager::getInstance()->randd( center.y() - traits->getLengthOnAxis(AXIS_Y) / 2., center.y() + traits->getLengthOnAxis(AXIS_Y) / 2.),
		RandomEngineManager::getInstance()->randd( center.z() - traits->getLengthOnAxis(AXIS_Z) / 2., center.z() + traits->getLengthOnAxis(AXIS_Z) / 2.)
		);
}