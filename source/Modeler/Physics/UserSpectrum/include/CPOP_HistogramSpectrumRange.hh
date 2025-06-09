#ifndef CPOP_HISTO_SPECTRUM_RANGE_HH
#define CPOP_HISTO_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

/// define a range of values for an histogram spectrum
class CPOP_HistogramSpectrumRange : public CPOP_SpectrumRange {
public:
	CPOP_HistogramSpectrumRange(std::pair<G4float, G4double> pLowBound, std::pair<G4float, G4double> pLighBound);

	[[nodiscard]] G4double computeEnergy() const override;
	[[nodiscard]] G4double GetEnergy(G4double) const override;

protected:
	/// \brief the energy low boundary
	G4double _energyLowBound;
	/// \brief the energy high bounday
	G4double _energyHighBound;
};

#endif
