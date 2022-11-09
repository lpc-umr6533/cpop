#ifndef CPOP_DISCRETE_SPECTRUM_RANGE_HH
#define CPOP_DISCRETE_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// define a range of values for a disctrite spectrum
///////////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_DiscreteSpectrumRange : public CPOP_SpectrumRange
{
public:
	/// \brief constructor
	CPOP_DiscreteSpectrumRange(pair<G4double, G4double> lowBound, pair<G4double, G4double> highBound);
	/// \brief destructor
	~CPOP_DiscreteSpectrumRange();
	/// \brief compute energy one the interval
	G4double computeEnergy() const;
	virtual G4double GetEnergy(G4double) const;

protected:
	/// \brief the energy low boundary
	G4double energyLowBound;
	/// \brief the energy high bounday
	G4double energyHighBound;
};

#endif // CPOP_DISCRETE_SPECTRUM_RANGE_HH