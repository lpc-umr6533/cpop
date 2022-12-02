/*----------------------
Copyright (C): Henri Payno, Axel Delsol,
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#ifndef UNIT_SYSTEM_MANAGER_HH
#define UNIT_SYSTEM_MANAGER_HH

#include "CLHEP/Units/SystemOfUnits.h"

	namespace CLHEP
{
#ifndef nanogram
  static constexpr double nanogram = 1.e-12*kilogram;
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////
/// \brief  The UnitSystemManager permits to deal with unitSystem.
/// Defined as a singleton.
/// @author Henri Payno
/////////////////////////////////////////////////////////////////////////////////////////
class UnitSystemManager
{
public:
	/// \brief list the metrical system accessible
	enum eMetricSystem
	{
		Centimeter,
		Millimeter,
		Micrometer,
		Nanometer
	};

	/// \brief list the weight system accessible
	enum eWeightSystem
	{
		Kilogram,
		Gram,
		Milligram,
		Nanogram
	};

public:
	/// \brief constructor.
	UnitSystemManager();
	/// \brief destructor.
	~UnitSystemManager();
	/// \brief return the instance of the manager.
	static UnitSystemManager* getInstance();
	/// \brief return the requested metric unit.
	double getMetricUnit(eMetricSystem)	const;
	/// \brief retunr the current metric unit.
	double getcurrentMetricUnit() const					{ return currentMetricUnit;};
	/// \brief metric unit setter
	void setCurrentMetricUnit(eMetricSystem eSystem)	{ currentMetricUnit = getMetricUnit(eSystem);};
	/// \brief return the value of the requested weight system
	double getWeightUnit(eWeightSystem) const ;
	/// \brief convert grom CPOp metric system ( micrometer ) to G4metric (meter)
	double getConversionToG4() const 					{ return CLHEP::micrometer;};
	/// \brief convert grom CPOp metric system ( micrometer ) to G4metric (meter)
	double getConversionFromG4() const 					{ return 1./CLHEP::micrometer;};

private:
	double currentMetricUnit;		///< \brief the setted metric system.
};


#endif // UNIT_SYSTEM_MANAGER_HH
