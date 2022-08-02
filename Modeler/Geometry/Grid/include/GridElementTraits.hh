/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef GRID_ELEMENT_TRAITS_HH
#define GRID_ELEMENT_TRAITS_HH

#include <assert.h>
#include <vector>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////
/// \brief  deifne grid element traits such as size on axis
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel>
class GridElementTraits
{
public:
	/// \brief constructor
	GridElementTraits(std::vector<Kernel>);
	/// \brief destructor
	~GridElementTraits();
	/// \brief return the value on the requested axis
	Kernel getLengthOnAxis(unsigned int) const;

private:
	std::vector<Kernel> sizes;	///< \brief size on each axis
};


/////////////////////////////////////// implementation ///////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel>
GridElementTraits<Kernel>::GridElementTraits(std::vector<Kernel> pSizes):
	sizes(pSizes)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel>
GridElementTraits<Kernel>::~GridElementTraits()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//			getLenghtOnAxis(unsigned int) const
///
/////////////////////////////////////////////////////////////////////////////////////////
template <typename Kernel>
Kernel GridElementTraits<Kernel>::getLengthOnAxis(unsigned int pAxis) const
{
	assert(pAxis < sizes.size());
	if( pAxis >= sizes.size() )
	{
		return Kernel();
	}else
	{
		return sizes[pAxis];
	}
}

#endif // GRID_ELEMENT_TRAITS_HH