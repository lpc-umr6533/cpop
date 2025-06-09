#include "Source.hh"

#include "G4RandomDirection.hh"
#include "G4ParticleDefinition.hh"

#include "Population.hh"

#include "G4IonTable.hh"


namespace cpop {

Source::Source(const std::string &name, const Population &population):
	_sourceName(name),
	_population(&population)
{
}

std::string Source::source_name() const {
	return _sourceName;
}

void Source::setSource_name(const std::string &source_name) {
	_sourceName = source_name;
}

G4ParticleDefinition *Source::particle() const {
	return _particle;
}

void Source::setParticle(G4ParticleDefinition *particle) {
	_particle = particle;
}

void Source::setIon(G4int atomic_number, G4int atomic_mass) {
	G4IonTable* ionTable = G4IonTable::GetIonTable();
	_ion = ionTable->GetIon(atomic_number, atomic_mass);
}

G4ParticleDefinition *Source::ion() const {
	return _ion;
}

void Source::setUser_spectrum(const std::string &user_spectrum_file) {
	_userSpectrum = std::make_unique<CPOP_UserSpectrum>(user_spectrum_file);
}

G4double Source::GetEnergy() const {
	return _userSpectrum->GetEnergy();
}

G4ThreeVector Source::GetMomentum() const {
	return randSphere();
}

G4ThreeVector Source::randSphere() const {
	G4double v1, v2, S;
	do {
		v1 = 2.*G4UniformRand() - 1.;
		v2 = 2.*G4UniformRand() - 1.;
		S = v1*v1 + v2*v2;
	} while (S > 1.);
	G4double k = std::sqrt(1. - S);
	return {2*v1*k, 2*v2*k, 1 - 2*S};
}

const Population *Source::population() const {
	return _population;
}

}
