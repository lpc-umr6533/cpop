#ifndef CPOP_USER_SPECTRUM_H
#define CPOP_USER_SPECTRUM_H

#include "globals.hh"
#include "CPOP_SpectrumRange.hh"
#include <map>

class CPOP_UserSpectrum {
public:
	CPOP_UserSpectrum(G4String file_to_read);
	~CPOP_UserSpectrum();

	[[nodiscard]] G4double GetEnergy() const;
	[[nodiscard]] G4double GetEnergy(G4double, G4double) const;

private:
	void uniformProbabilities(G4double ratio);

private:
	//void Construct(G4String file_to_read);
	G4int _mode;
	G4float _eminSpectrum;
	G4int   _dimSpectrum;
	G4float _sumProba;
	G4float* _tabProba;
	G4float* _tabSumproba;
	G4float* _tabEnergy;
	/// \brief map linking th ehigher proba
	std::map<G4double, CPOP_SpectrumRange*> _mapToSpectrumRanges;
};

#endif
