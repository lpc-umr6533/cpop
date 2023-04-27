#ifndef SOURCEMESSENGER_HH
#define SOURCEMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"

class G4ParticleTable;

#include "MessengerBase.hh"

namespace cpop {

class Source;

class SourceMessenger : public MessengerBase
{
public:
    SourceMessenger(Source* source);
    ~SourceMessenger() = default;

    virtual void BuildCommands(G4String base) override;
    virtual void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    Source* source_;

    // Commands
    /// \brief Set particle to be generated
    std::unique_ptr<G4UIcmdWithAString> particle_cmd_;
    /// \brief Set ion to be generated
    std::unique_ptr<G4UIcommand> ion_cmd_;
    /// \brief Set spectrum file
    std::unique_ptr<G4UIcmdWithAString> user_spectrum_cmd_;
    /// \brief Particle table
    G4ParticleTable* particle_table_;

};

}

#endif // SOURCEMESSENGER_HH
