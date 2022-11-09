#ifndef CPOP_HISTO_SPECTRUM_RANGE_HH
#define CPOP_HISTO_SPECTRUM_RANGE_HH

#include "CPOP_SpectrumRange.hh"

///////////////////////////////////////////////////////////////////////////////////////////////////
/// define a range of values for an histogram spectrum
///////////////////////////////////////////////////////////////////////////////////////////////////
class CPOP_HistogramSpectrumRange : public CPOP_SpectrumRange
{
public:
	/// \brief constructor
	CPOP_HistogramSpectrumRange(pair<G4float, G4double> pLowBound, pair<G4float, G4double> pLighBound);
	/// \brief destructor
	~CPOP_HistogramSpectrumRange();

	G4double computeEnergy() const;
	virtual G4double GetEnergy(G4double) const;

protected:
	/// \brief the energy low boundary
	G4double energyLowBound;
	/// \brief the energy high bounday
	G4double energyHighBound;	
};

#endif // CPOP_HISTO_SPECTRUM_RANGE_HH