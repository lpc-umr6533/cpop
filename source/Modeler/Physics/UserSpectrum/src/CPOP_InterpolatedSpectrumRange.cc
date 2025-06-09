#include "CPOP_InterpolatedSpectrumRange.hh"
#include "Randomize.hh"

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
CPOP_InterpolatedSpectrumRange::CPOP_InterpolatedSpectrumRange(
	std::pair<G4double, G4double> pProbaBound,
	G4double pAlpha,
	G4double pBeta,
	G4double pGamma,
	G4double pA,
	G4double pB
):
	CPOP_SpectrumRange(pProbaBound.first, pProbaBound.second),
	_alpha(pAlpha),
	_beta(pBeta),
	_gamma(pGamma),
	_a(pA),
	_b(pB)
{
}

G4double CPOP_InterpolatedSpectrumRange::computeEnergy() const {
	// solve the equation of the integral for a random x value.
	G4double	my_rndm=G4UniformRand();
	return GetEnergy(my_rndm);
}

G4double CPOP_InterpolatedSpectrumRange::GetEnergy(G4double my_rndm) const {
	// solve the equation of the integral for a random x value.
	auto delta = (_alpha*_a+_beta)*(_alpha*_a+_beta) + 2.*_alpha*_gamma*my_rndm;
	auto sqrtDelta = sqrt(delta);
	G4double x = (-_beta+sqrtDelta) / _alpha;
	if((x - _a)*( x - _b) <= 0) {
		return x;
	} else {
		return (-_beta-sqrtDelta) / _alpha;
	}
}
