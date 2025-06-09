#ifndef SOURCE_HH
#define SOURCE_HH

#include <memory>
#include <random>

#include "G4ThreeVector.hh"

#include "CPOP_UserSpectrum.hh"
#include "G4IonTable.hh"
class G4ParticleDefinition;

// Use G4 naming convention when methods are meant to be used by the users (or dev)

namespace cpop {

class Population;

class Source {
public:
	Source(const std::string& name, const Population& population);
	virtual ~Source() = default;

	[[nodiscard]] std::string source_name() const;
	void setSource_name(const std::string &source_name);

	[[nodiscard]] G4ParticleDefinition *particle() const;
	void setParticle(G4ParticleDefinition *particle);

	[[nodiscard]] G4ParticleDefinition *ion() const;
	void setIon(G4int atomic_number, G4int atomic_mass);

	void setUser_spectrum(const std::string& user_spectrum_file);

	/// \brief Generate a random energy (in MeV)
	[[nodiscard]] G4double GetEnergy() const;
	/// \brief Generate a random momentum direction (in G4 unit)
	[[nodiscard]] G4ThreeVector GetMomentum() const;

	/// \brief Initialize the object
	virtual void Initialize() {}

	// Pure virtual methods
	// Not const to let the user change object state (eg keep track of what has been generated)
	/// \brief Generate a random position (in G4 unit)
	virtual std::vector<G4ThreeVector> GetPosition() = 0;
	/// \brief Called at the end of GeneratePrimaries
	virtual void Update() = 0;
	/// \brief Tell if the source has generated all its particles
	virtual bool HasLeft() = 0;

	int lineNumberPositionsDirectionsFile = 1;

protected:
    /// \brief Generate uniformely a 3D point on the unit sphere using G.Marsaglia method
    [[nodiscard]] G4ThreeVector randSphere() const;

    [[nodiscard]] const Population *population() const;

private:
	/// \brief Source name
	std::string _sourceName = "";
	/// \brief Cell population
	const Population* _population = nullptr;
	/// \brief User energy spectrum
	std::unique_ptr<CPOP_UserSpectrum> _userSpectrum;
	/// \brief Particle sent by this source
	G4ParticleDefinition* _particle = nullptr;
	/// \brief Ion sent by this source
	G4ParticleDefinition* _ion = nullptr;
};

}

#endif
