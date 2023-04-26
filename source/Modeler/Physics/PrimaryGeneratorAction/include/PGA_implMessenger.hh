#ifndef PGA_IMPLMESSENGER_HH
#define PGA_IMPLMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "MessengerBase.hh"

namespace cpop {

class PGA_impl;

class PGA_implMessenger : public MessengerBase
{
public:
    PGA_implMessenger(PGA_impl* pga_impl);

    void BuildCommands(G4String base) override;
    void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    PGA_impl* pga_impl_;

    G4String base_;

    // Commands
    /// \brief Add a uniform source
    std::unique_ptr<G4UIcmdWithAString> uniform_cmd_;
    /// \brief Add a distributed source
    std::unique_ptr<G4UIcmdWithAString> distributed_cmd_;
    /// \brief Activate radionuclide's daughter diffusion
    std::unique_ptr<G4UIcmdWithAString> diffusion_cmd_;
    /// \brief Initialize the sources
    std::unique_ptr<G4UIcmdWithoutParameter> init_cmd_;
};

}



#endif // PGA_IMPLMESSENGER_HH
