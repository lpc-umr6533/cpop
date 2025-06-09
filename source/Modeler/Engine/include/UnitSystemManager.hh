#ifndef UNIT_SYSTEM_MANAGER_HH
#define UNIT_SYSTEM_MANAGER_HH

#include "CLHEP/Units/SystemOfUnits.h"

namespace CLHEP {

#ifndef nanogram
  static constexpr double nanogram = 1.e-12*kilogram;
#endif

}

/// \brief  The UnitSystemManager permits to deal with unitSystem.
/// Defined as a singleton.
/// @author Henri Payno
class UnitSystemManager {
public:
	/// \brief list the metrical system accessible
	enum eMetricSystem {
		Centimeter,
		Millimeter,
		Micrometer,
		Nanometer
	};

	/// \brief list the weight system accessible
	enum eWeightSystem {
		Kilogram,
		Gram,
		Milligram,
		Nanogram
	};

public:
	UnitSystemManager();

	/// \brief return the instance of the manager.
	static UnitSystemManager* getInstance();
	/// \brief return the requested metric unit.
	[[nodiscard]] double getMetricUnit(eMetricSystem)	const;
	/// \brief retunr the current metric unit.
	[[nodiscard]] double getcurrentMetricUnit() const { return _currentMetricUnit; }
	/// \brief metric unit setter
	void setCurrentMetricUnit(eMetricSystem eSystem)	{ _currentMetricUnit = getMetricUnit(eSystem); }
	/// \brief return the value of the requested weight system
	[[nodiscard]] double getWeightUnit(eWeightSystem) const ;
	/// \brief convert grom CPOp metric system ( micrometer ) to G4metric (meter)
	[[nodiscard]] double getConversionToG4() const { return CLHEP::micrometer; }
	/// \brief convert grom CPOp metric system ( micrometer ) to G4metric (meter)
	[[nodiscard]] double getConversionFromG4() const { return 1./CLHEP::micrometer; }

private:
	double _currentMetricUnit;		///< \brief the setted metric system.
};


#endif
