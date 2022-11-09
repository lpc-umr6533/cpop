#ifndef CPOP_USER_SPECTRUM_H
#define CPOP_USER_SPECTRUM_H

#include "globals.hh"
#include "CPOP_SpectrumRange.hh"
#include <map>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////////////////////
class CPOP_UserSpectrum
{
public:
		CPOP_UserSpectrum(G4String file_to_read);
		~CPOP_UserSpectrum();
		G4double GetEnergy() const;
		G4double GetEnergy(G4double, G4double) const;

private:
	void uniformProbabilities(G4double ratio);

private:
		//void Construct(G4String file_to_read);
		G4int mode;
		G4float emin_spectrum;
		G4int   dim_spectrum;
		G4float sum_proba;
		G4float* tab_proba;
		G4float* tab_sumproba;
		G4float* tab_energy;
		/// \brief map linking th ehigher proba 
		map<G4double, CPOP_SpectrumRange*> mapToSpectrumRanges;
};

#endif // CPOP_USER_SPECTRUM_H
