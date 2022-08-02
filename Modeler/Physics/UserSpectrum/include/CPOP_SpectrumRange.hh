#ifndef CPOP_SPECTRUM_RANGE_HH
#define CPOP_SPECTRUM_RANGE_HH

#include "globals.hh"

#include <utility>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////////////////
class CPOP_SpectrumRange
{
public:
	/// \brief constructor
	CPOP_SpectrumRange(G4double probaLowBound, G4double probaHighBound);
	/// \brief destructor
	virtual ~CPOP_SpectrumRange();
	
	/// \brief compute a random energy on the given energy 
	virtual G4double computeEnergy() const = 0;
	virtual G4double GetEnergy(G4double) const = 0;

	/// \brief operator used to order SpectrumRange on set
	bool operator < (const CPOP_SpectrumRange&);

	G4double getProbaLowBound() const 	{ return probaLowBound; } 
	void setProbaLowBound(G4double p)	{ probaLowBound = p;}
	G4double getProbaHighBound() const 	{ return probaHighBound; }
	void setProbaHighBound(G4double p)	{ probaHighBound = p; }			

protected:
	/// \brief the probability low boundary 
	G4double probaLowBound; 
	/// \brief the probability high boundary
	G4double probaHighBound;
};

#endif // SPECTRUM_RANGE_HH
