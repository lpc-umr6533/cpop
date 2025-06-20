#include "UnitSystemManager.hh"
#include "InformationSystemManager.hh"

static UnitSystemManager* unitSystemManager = nullptr;
static const UnitSystemManager::eMetricSystem initialMetric = UnitSystemManager::Micrometer;

#include <QString>

UnitSystemManager::UnitSystemManager() {
	/// init metric system values
	setCurrentMetricUnit(initialMetric);
}

/// \return The singleton of the unistSystemManager
UnitSystemManager* UnitSystemManager::getInstance() {
	if(!unitSystemManager)
		unitSystemManager = new UnitSystemManager();
	return unitSystemManager;
}

/// \param unitSyst The unit requested
/// \return double The corresponding "higher"
double UnitSystemManager::getMetricUnit(eMetricSystem unitSyst) const {
	switch(unitSyst) {
		case Centimeter:  return 1.e6;
		case Millimeter:  return 1.e3;
		case Micrometer:  return 1.;
		case Nanometer:   return 1.e-3;
		default:
		{
		  std::string mess = "unknow metric system requested";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "UnitSystemManager");
		}
	}
	return Nanometer;
}

/// \param unitSystem The unit requested
/// \return double The corresponding "higher"
double UnitSystemManager::getWeightUnit(eWeightSystem unitSystem) const {
	switch(unitSystem) {
		case Kilogram:  return CLHEP::kilogram;
		case Gram:      return CLHEP::gram;
		case Milligram: return CLHEP::milligram;
		case Nanogram:  return CLHEP::nanogram;
		default:
		{
			std::string mess = "unknow metric system requested";
			InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, mess, "UnitSystemManager");
		}
	}
	return CLHEP::kilogram;
}
