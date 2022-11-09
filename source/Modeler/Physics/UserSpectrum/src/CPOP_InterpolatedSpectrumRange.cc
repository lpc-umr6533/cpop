#include "CPOP_InterpolatedSpectrumRange.hh"
#include "Randomize.hh"

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
CPOP_InterpolatedSpectrumRange::CPOP_InterpolatedSpectrumRange(
	pair<G4double, G4double> pProbaBound, 
	G4double pAlpha, 
	G4double pBeta, 
	G4double pGamma,
	G4double pA,
	G4double pB):
	CPOP_SpectrumRange(pProbaBound.first, pProbaBound.second),
	alpha(pAlpha),
	beta(pBeta),
	gamma(pGamma),
	a(pA),
	b(pB)
{

}

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
CPOP_InterpolatedSpectrumRange::~CPOP_InterpolatedSpectrumRange()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
G4double CPOP_InterpolatedSpectrumRange::computeEnergy() const
{
	// solve the equation of the integral for a random x value.
	G4double	my_rndm=G4UniformRand();
	return GetEnergy(my_rndm);
}

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
G4double CPOP_InterpolatedSpectrumRange::GetEnergy(G4double my_rndm) const
{
	// cout << "[CPOP]" << endl;
	// cout << "low proba = " << probaLowBound << endl;
	// cout << "tab_proba[i+1]= " << probaHighBound - probaLowBound << endl;
	// cout << "high proba = " << probaHighBound << endl;
	// cout << "tab_energy[i+1]= " << b << endl;
	// cout << " alpha = " << alpha << endl;
	// cout << " beta = " << beta << endl;
	// cout << " gamma = " << gamma << endl;
	// cout << " a = " << a << endl;
	// cout << " b = " << b << endl;

	// solve the equation of the integral for a random x value.
	G4double x = (-beta+sqrt((alpha*a+beta)*(alpha*a+beta) + 2.*alpha*gamma*my_rndm))/(alpha);
	if( (x - a)*( x - b) <= 0)
	{
		return x;
	}else
	{
		return (-beta-sqrt((alpha*a+beta)*(alpha*a+beta)+2.*alpha*gamma*my_rndm))/(alpha);
	}
}