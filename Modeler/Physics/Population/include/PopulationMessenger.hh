#ifndef POPULATIONMESSENGER_HH
#define POPULATIONMESSENGER_HH

#include <memory>

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithoutParameter.hh"

#include "MessengerBase.hh"

namespace cpop {

class Population;

class PopulationMessenger : public MessengerBase
{
public:
    PopulationMessenger(Population* population);
    ~PopulationMessenger() = default;

    virtual void BuildCommands(G4String base) override;
    virtual void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    Population* population_;

    // Commands
    /// \brief Set verbose level
    std::unique_ptr<G4UIcmdWithAnInteger> verbose_cmd_;
    /// \brief Set population file
    std::unique_ptr<G4UIcmdWithAString> population_cmd_;
    /// \brief Set number of facet per poly
    std::unique_ptr<G4UIcmdWithAnInteger> number_facet_cmd_;
    /// \brief Set delta refinement
    std::unique_ptr<G4UIcmdWithADouble> delta_ref_cmd_;
    /// \brief Set internal layer ratio
    std::unique_ptr<G4UIcmdWithADouble> internal_ratio_cmd_;
    /// \brief Set intermediary layer ratio
    std::unique_ptr<G4UIcmdWithADouble> intermediary_ratio_cmd_;
    /// \brief Set number of sampling cell
    std::unique_ptr<G4UIcmdWithAnInteger> number_sampling_cmd_;
    /// \brief Initialize population and regions
    std::unique_ptr<G4UIcmdWithoutParameter> init_cmd_;

    /// \brief Bool to get info at the stepping level, i.e.  all energy deposited and positions for each step
    std::unique_ptr<G4UIcmdWithAnInteger> get_stepping_level_info_cmd_;
    /// \brief  Bool to get info at the event level, i.e.  all entrance and exit energies of alpha particles in nuclei
    std::unique_ptr<G4UIcmdWithAnInteger> get_event_level_info_cmd_;


};

}

#endif // POPULATIONMESSENGER_HH
