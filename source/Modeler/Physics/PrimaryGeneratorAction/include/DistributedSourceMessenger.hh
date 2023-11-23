#ifndef DISTRIBUTEDSOURCEMESSENGER_HH
#define DISTRIBUTEDSOURCEMESSENGER_HH

#include "G4UIcommand.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWith3Vector.hh"

#include "MessengerBase.hh"
#include "SourceMessenger.hh"

namespace cpop {

class SourceMessenger;
class DistributedSource;

class DistributedSourceMessenger : public MessengerBase
{
  /// Victor Levrague :addition of maximum particle per cell, % of cell labeling and different positions for all particles generated on the same cell ///
public:
    DistributedSourceMessenger(DistributedSource* source);

    void BuildCommands(G4String base) override;
    void SetNewValue(G4UIcommand * command , G4String newValue) override;

private:
    DistributedSource* source_;

    // We choose composition over inheritancce to avoid type casting
    std::unique_ptr<SourceMessenger> source_messenger_;

    // Commands
    /// \brief Set the number of distributed sources in the population
    std::unique_ptr<G4UIcmdWithAnInteger> total_source_cmd_;
    /// \brief Set the number of particles to be generated from one source
    std::unique_ptr<G4UIcmdWithAnInteger> part_per_source_cmd_;
    /// \brief Distribute the nanoparticle in the 3 regions
    std::unique_ptr<G4UIcmdWith3Vector> nano_per_region_cmd_;
    /// \brief Define the secondaries repartition in a cell
    std::unique_ptr<G4UIcommand> pos_in_cell_cmd_;
    /// \brief Maximum number of sources per cell to be generated, in each region
    std::unique_ptr<G4UIcmdWith3Vector> max_sources_per_cell_cmd_;
    /// \brief Percentage of labeled cells, in each region
    std::unique_ptr<G4UIcmdWith3Vector> cell_Labeling_cmd_;
    /// \brief Set the number of nanoparticle in the population
    std::unique_ptr<G4UIcmdWithAnInteger> only_one_position_for_all_particles_on_a_cell_cmd_;
    /// \brief Define the choice of lognormal distribution for particles in cells
    std::unique_ptr<G4UIcommand> log_normal_distrib_cmd_;

};

}



#endif // DISTRIBUTEDSOURCEMESSENGER_HH
