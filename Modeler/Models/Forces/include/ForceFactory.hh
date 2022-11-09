/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef FORCE_FACTORY_HH
#define FORCE_FACTORY_HH

#include "ForceSettings.hh"
#include "CellSettings.hh"

using namespace Settings::nForce;
using namespace Settings::nCell;

//////////////////////////////////////////////////////////////////////////////
/// \brief the factory of forces. 
/// @author Henri Payno
//////////////////////////////////////////////////////////////////////////////class ForceFactory
class ForceFactory
{
public:
	/// \brief the defined force type
	enum ForceType
	{
		ELASTIC,
		ROUND_UP
	};

public:
	/// \brief constructor
	ForceFactory();
	/// \brief destructor
	~ForceFactory();
	/// \brief return the instance of the factory
	static ForceFactory* getInstance();
	/// \brief return the force targetted
	t_Force_2* computeForce2D(ForceType, t_Cell_2*);
	/// \brief return the force targetted
	t_Force_3* computeForce3D(ForceType, t_Cell_3*);
};

#endif // FORCE_FACTORY_HH
