#ifndef CPOP_SPECTRUM_RANGE_HH
#define CPOP_SPECTRUM_RANGE_HH

#include "globals.hh"

class CPOP_SpectrumRange {
public:
	CPOP_SpectrumRange(G4double probaLowBound, G4double probaHighBound);
	virtual ~CPOP_SpectrumRange() = default;

	/// \brief compute a random energy on the given energy
	[[nodiscard]] virtual G4double computeEnergy() const = 0;
	[[nodiscard]] virtual G4double GetEnergy(G4double) const = 0;

	/// \brief operator used to order SpectrumRange on set
	bool operator < (const CPOP_SpectrumRange&);

	[[nodiscard]] G4double getProbaLowBound() const { return _probaLowBound; }
	void setProbaLowBound(G4double p)	{ _probaLowBound = p;}
	[[nodiscard]] G4double getProbaHighBound() const { return _probaHighBound; }
	void setProbaHighBound(G4double p)	{ _probaHighBound = p; }

protected:
	/// \brief the probability low boundary
	G4double _probaLowBound;
	/// \brief the probability high boundary
	G4double _probaHighBound;
};

#endif // SPECTRUM_RANGE_HH
