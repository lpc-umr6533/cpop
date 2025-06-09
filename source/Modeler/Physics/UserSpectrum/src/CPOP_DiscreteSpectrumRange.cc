#include "CPOP_DiscreteSpectrumRange.hh"

CPOP_DiscreteSpectrumRange::CPOP_DiscreteSpectrumRange(
	std::pair<G4double, G4double> pLowBound, std::pair<G4double, G4double> pHighBound
):
	CPOP_SpectrumRange(pLowBound.first, pHighBound.first)
{
	_energyLowBound = pLowBound.second;
	_energyHighBound = pHighBound.second;
}

G4double CPOP_DiscreteSpectrumRange::computeEnergy() const {
	return _energyHighBound;
}

G4double CPOP_DiscreteSpectrumRange::GetEnergy(G4double) const {
	return _energyHighBound;
}
