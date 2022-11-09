/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_SAMPLER_HH
#define CELL_SAMPLER_HH

#include "CellSettings.hh"
#include <set>

//////////////////////////////////////////////////////////////////////////////
/// \brief interface for cell sampling
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template<typename CellType>
class CellSampler
{
public:
	/// \brief consstructor
	CellSampler()	{};
	/// \brief destructor
	virtual ~CellSampler()	{};
	/// \brief function called to sample NCells;
	virtual set<const CellType* > sampleNCells(unsigned int) const = 0;
	
};

#endif // CELL_SAMPLER_HH 