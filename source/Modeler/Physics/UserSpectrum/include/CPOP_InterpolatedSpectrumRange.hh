#ifndef CPOP_INTERPOLATED_SPECTRUM_RANGE_HH
#define CPOP_INTERPOLATED_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

/////////////////////////////////////////////////////////////////////////////////////////
/// The same as Histogram but with a direct distrution ( linear distribution, giving an higher weight to the highbound)
/////////////////////////////////////////////////////////////////////////////////////////
class CPOP_InterpolatedSpectrumRange : public CPOP_SpectrumRange
{
public:
	/// \brief constructor
	CPOP_InterpolatedSpectrumRange(pair<G4double, G4double> pProbaBound, G4double pAlpha, G4double pBeta, G4double pGamma, G4double a, G4double b);
	/// \brief destructor
	~CPOP_InterpolatedSpectrumRange();

	/// \brief compute a random energy on the given energy 
	virtual G4double computeEnergy() const;
	virtual G4double GetEnergy(G4double) const;

private:
	G4double alpha;
	G4double beta;
	G4double gamma;
	G4double a;
	G4double b;
};

#endif // CPOP_INTERPOLATED_SPECTRUM_RANGE_HH