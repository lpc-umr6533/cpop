#include "CPOP_HistogramSpectrumRange.hh"
#include "Randomize.hh"

CPOP_HistogramSpectrumRange::CPOP_HistogramSpectrumRange(
	std::pair<G4float, G4double> pLowBound,
	std::pair<G4float, G4double> pHighBound
):
	CPOP_SpectrumRange(pLowBound.first, pHighBound.first)
{
	_energyLowBound = pLowBound.second;
	_energyHighBound = pHighBound.second;
}

/// \brief return a random value between energyLowBound and energyHighBound
G4double CPOP_HistogramSpectrumRange::computeEnergy() const {
	return (_energyLowBound + G4UniformRand()*(_energyHighBound - _energyLowBound));
}

/// \brief return a random value between energyLowBound and energyHighBound
G4double CPOP_HistogramSpectrumRange::GetEnergy(G4double rnd) const {
	return (_energyLowBound + rnd*(_energyHighBound - _energyLowBound));
}
