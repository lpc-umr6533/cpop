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

class Source
{
public:
    Source(const std::string& name, const Population& population);
    virtual ~Source() = default;

    std::string source_name() const;
    void setSource_name(const std::string &source_name);

    G4ParticleDefinition *particle() const;
    void setParticle(G4ParticleDefinition *particle);

    G4ParticleDefinition *ion() const;
    void setIon(G4int atomic_number, G4int atomic_mass);

    void setUser_spectrum(const std::string& user_spectrum_file);

    /// \brief Generate a random energy (in MeV)
    G4double GetEnergy() const;
    /// \brief Generate a random momentum direction (in G4 unit)
    G4ThreeVector GetMomentum() const;

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

protected:
    /// \brief Generate uniformely a 3D point on the unit sphere using G.Marsaglia method
    G4ThreeVector randSphere() const;

    const Population *population() const;

private:
    /// \brief Source name
    std::string source_name_ = "";
    /// \brief Cell population
    const Population* population_ = nullptr;
    /// \brief User energy spectrum
    std::unique_ptr<CPOP_UserSpectrum> user_spectrum_;
    /// \brief Particle sent by this source
    G4ParticleDefinition* particle_ = nullptr;
    /// \brief Ion sent by this source
    G4ParticleDefinition* ion_ = nullptr;
};

}

#endif // SOURCE_HH
