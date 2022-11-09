/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "RandomForce.hh"
#include "RandomEngineManager.hh"
#include "CGAL_Utils.hh"

using namespace Utils::myCGAL;
////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Vector_2 RandomForce<double, Point_2, Vector_2>::computeForce() const
{
	return (intensity* normalize( 
									Vector_2( 	RandomEngineManager::getInstance()->randd(-1., 1.), 
												RandomEngineManager::getInstance()->randd(-1., 1.) 
											) 
								) );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
///
////////////////////////////////////////////////////////////////////////////////////////////////////
template<>
Vector_3 RandomForce<double, Point_3, Vector_3>::computeForce() const
{
	return (intensity* normalize( 
									Vector_3( 	RandomEngineManager::getInstance()->randd(-1., 1.), 
												RandomEngineManager::getInstance()->randd(-1., 1.),
												RandomEngineManager::getInstance()->randd(-1., 1.) 
											) 
								) );
}