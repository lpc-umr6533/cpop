#ifndef CPOP_INTERPOLATED_SPECTRUM_RANGE_HH
#define CPOP_INTERPOLATED_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

/// The same as Histogram but with a direct distrution ( linear distribution, giving an higher weight to the highbound)
class CPOP_InterpolatedSpectrumRange : public CPOP_SpectrumRange {
public:
	CPOP_InterpolatedSpectrumRange(
		std::pair<G4double, G4double> pProbaBound,
		G4double pAlpha,
		G4double pBeta,
		G4double pGamma,
		G4double a,
		G4double b
	);

	/// \brief compute a random energy on the given energy
	[[nodiscard]] G4double computeEnergy() const override;
	[[nodiscard]] G4double GetEnergy(G4double) const override;

private:
	G4double _alpha;
	G4double _beta;
	G4double _gamma;
	G4double _a;
	G4double _b;
};

#endif
