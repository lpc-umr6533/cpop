#ifndef oldUserSpectrum_H
#define oldUserSpectrum_H 1

#include "globals.hh"

class ClassicUserSpectrum
{
public :
		ClassicUserSpectrum(G4String file_to_read);
		~ClassicUserSpectrum();
		G4float GetEnergy() const;
		G4float GetEnergy(G4double) const;
		G4float GetEnergy(G4double, G4double) const;

private :
		//void Construct(G4String file_to_read);
		G4int mode;
		G4float emin_spectrum;
		G4int   dim_spectrum;
		G4float sum_proba;
		G4float* tab_proba;
		G4float* tab_sumproba;
		G4float* tab_energy;
};

#endif
