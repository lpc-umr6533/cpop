#ifndef oldUserSpectrum_H
#define oldUserSpectrum_H 1

#include "globals.hh"

class ClassicUserSpectrum {
public :
	ClassicUserSpectrum(G4String file_to_read);
	~ClassicUserSpectrum();

	[[nodiscard]] G4float GetEnergy() const;
	[[nodiscard]] G4float GetEnergy(G4double) const;
	[[nodiscard]] G4float GetEnergy(G4double, G4double) const;

private :
	//void Construct(G4String file_to_read);
	G4int _mode;
	G4float _eminSpectrum;
	G4int   _dimSpectrum;
	G4float _sumProba;
	G4float* _tabProba;
	G4float* _tabSumproba;
	G4float* _tabEnergy;
};

#endif
