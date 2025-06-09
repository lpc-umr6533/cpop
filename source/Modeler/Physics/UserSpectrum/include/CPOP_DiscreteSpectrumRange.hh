#ifndef CPOP_DISCRETE_SPECTRUM_RANGE_HH
#define CPOP_DISCRETE_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

/// define a range of values for a disctrite spectrum
class CPOP_DiscreteSpectrumRange : public CPOP_SpectrumRange {
public:
	CPOP_DiscreteSpectrumRange(std::pair<G4double, G4double> lowBound, std::pair<G4double, G4double> highBound);

	/// \brief compute energy one the interval
	[[nodiscard]] G4double computeEnergy() const override;
	[[nodiscard]] G4double GetEnergy(G4double) const override;

protected:
	/// \brief the energy low boundary
	G4double _energyLowBound;
	/// \brief the energy high bounday
	G4double _energyHighBound;
};

#endif
