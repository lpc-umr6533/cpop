/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef CELL_VARIABLE_ATTRIBUTE_HH
#define CELL_VARIABLE_ATTRIBUTE_HH


//////////////////////////////////////////////////////////////////////////////
/// \brief a simple class to store a min and a max value inside
/// used for masses, volumes...
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////
template <class T> class CellVariableAttribute
{
public:
	/// \brief consstructor
	CellVariableAttribute(T pMin, T pMax)	{v_min = pMin; v_max = pMax;}
	/// \brief destructor
	~CellVariableAttribute()				{}

	/// \brief return the min value.
	T var_max() const {return v_max;};
	/// \brief return the max value.		
	T var_min() const {return v_min;};

	/// \brief minimal value setter
	void setMin(T pMin)					{ v_min = pMin;}

	/// \brief maximal value setter
	void setMax(T pMax)					{ v_max = pMax;}

private:
	T v_min;	///< \brief minimal value
	T v_max;	///< \brief maximal value
};

#endif