#ifndef UNIFORMSOURCEMESSENGER_HH
#define UNIFORMSOURCEMESSENGER_HH

#include "G4UIcmdWithAnInteger.hh"

#include "MessengerBase.hh"
#include "SourceMessenger.hh"

namespace cpop {

class SourceMessenger;
class UniformSource;

class UniformSourceMessenger : public MessengerBase
{
public:
    UniformSourceMessenger(UniformSource *source);
    ~UniformSourceMessenger() = default;

    virtual void BuildCommands(G4String base) override;
    virtual void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    UniformSource* source_;

    // We choose composition over inheritancce to avoid type casting
    std::unique_ptr<SourceMessenger> source_messenger_;

    // Commands
    std::unique_ptr<G4UIcmdWithAnInteger> total_particle_cmd_;
};

}

#endif // UNIFORMSOURCEMESSENGER_HH
