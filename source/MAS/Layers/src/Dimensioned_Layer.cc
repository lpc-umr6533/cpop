/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "Dimensioned_Layer.hh"
/// \cond
template<>
Dimension Dimensioned_Layer<double, Point_2, Vector_2>::getDimension() const
{
	return _2D;
}

template<>
Dimension Dimensioned_Layer<double, Point_3, Vector_3>::getDimension() const
{
	return _3D;
}

/// \endcond