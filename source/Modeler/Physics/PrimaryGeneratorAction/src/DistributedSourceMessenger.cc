#include "DistributedSourceMessenger.hh"

#include <sstream>

#include "G4UIparameter.hh"

#include "DistributedSource.hh"
#include "G4Tokenizer.hh"

namespace cpop {

DistributedSourceMessenger::DistributedSourceMessenger(DistributedSource *source)
    : MessengerBase(),
      source_(source),
      source_messenger_(std::make_unique<SourceMessenger>(source))
{
}

void DistributedSourceMessenger::BuildCommands(G4String base)
{
    source_messenger_->BuildCommands(base);

    G4String cmd_base = base + "/totalSource";
    total_source_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_base, this);
    total_source_cmd_->SetGuidance("Set the number of sources in the population");
    total_source_cmd_->SetParameterName("TotalSource", false);
    total_source_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/particlesPerSource";
    part_per_source_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_base,
                                                                  this);
    part_per_source_cmd_->SetGuidance("Set the number of particles emitted from"
                                      "one source");
    part_per_source_cmd_->SetParameterName("PartPerNano", false);
    part_per_source_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/distributionInRegion";
    nano_per_region_cmd_ = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
    nano_per_region_cmd_->SetGuidance("Distribute the nanoparticle  in the 3 regions");
    nano_per_region_cmd_->SetParameterName("nNecrosis", "nIntermediary", "nExternal", false);
    nano_per_region_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/distributionInCell";
    pos_in_cell_cmd_ = std::make_unique<G4UIcommand>(cmd_base, this);
    G4UIparameter* cell_membrane = new G4UIparameter("CellMembrane", 'd', false);
    pos_in_cell_cmd_->SetParameter(cell_membrane);
    G4UIparameter* nucleoplasm = new G4UIparameter("Nucleoplasm", 'd', false);
    pos_in_cell_cmd_->SetParameter(nucleoplasm);
    G4UIparameter* nucleo_membrane = new G4UIparameter("NucleusMembrane", 'd', false);
    pos_in_cell_cmd_->SetParameter(nucleo_membrane);
    G4UIparameter* cytoplasm = new G4UIparameter("Cytoplasm", 'd', false);
    pos_in_cell_cmd_->SetParameter(cytoplasm);
    pos_in_cell_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/maxSourcesPerCell";
    max_sources_per_cell_cmd_ = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
    max_sources_per_cell_cmd_->SetGuidance("Set the maximum number of sources that can be attached to a cell");
    max_sources_per_cell_cmd_->SetParameterName("MaxSourcePerCell_Necrosis", "MaxSourcesPerCell_Intermediary", "MaxSourcesPerCell_External", false);
    max_sources_per_cell_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/cellLabelingPercentagePerRegion";
    cell_Labeling_cmd_ = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
    cell_Labeling_cmd_->SetGuidance("Set the percentage of labeled cells with nanoparticles, per region");
    cell_Labeling_cmd_->SetParameterName("cellLabeling%_Necrosis", "cellLabeling%_Intermediary", "cellLabeling%_External",false);
    cell_Labeling_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/only_one_position_for_all_particles_on_a_cell";
    only_one_position_for_all_particles_on_a_cell_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_base, this);
    only_one_position_for_all_particles_on_a_cell_cmd_->SetGuidance("Allow only one position for all particles generated on the same cell");
    only_one_position_for_all_particles_on_a_cell_cmd_->SetParameterName("Only_one_position_for_all_particles_on_a_cell", false);
    only_one_position_for_all_particles_on_a_cell_cmd_->AvailableForStates(G4State_PreInit, G4State_Idle);

    cmd_base = base + "/log_normal_distribution";
    log_normal_distrib_cmd_ = std::make_unique<G4UIcommand>(cmd_base, this);
    log_normal_distrib_cmd_->SetGuidance("log_normal_distribution");
    G4UIparameter* choice_distrib = new G4UIparameter("bool_choice", 'b', true);
    log_normal_distrib_cmd_->SetParameter(choice_distrib);
    G4UIparameter* mean_number_particle = new G4UIparameter("mean_number_particle", 'i', true);
    log_normal_distrib_cmd_->SetParameter(mean_number_particle);
    G4UIparameter* shape_factor = new G4UIparameter("shape_factor", 'd', true);
    log_normal_distrib_cmd_->SetParameter(shape_factor);
    log_normal_distrib_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);
}

void DistributedSourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    source_messenger_->SetNewValue(command, newValue);

    if (command == total_source_cmd_.get()) {
        source_->setNumber_source(total_source_cmd_->GetNewIntValue(newValue));
    } else if (command == part_per_source_cmd_.get()) {
        source_->setNumber_particles_per_source(part_per_source_cmd_->GetNewIntValue(newValue));
    } else if (command == nano_per_region_cmd_.get()) {
        G4ThreeVector vec = nano_per_region_cmd_->GetNew3VectorValue(newValue);
        int sum = vec.x() + vec.y() + vec.z();
        if (sum != source_->number_distributed()) {
            std::stringstream msg;
            msg << "Wrong distribution of the nanoparticle in the 3 regions : \n";
            msg << "Necrosis region : " << vec.x() << '\n';
            msg << "Intermediary region : " << vec.y() << '\n';
            msg << "External region : " << vec.z() << '\n';
            msg << "Sum : " << sum << '\n';
            msg << "Number of nanoparticle : " << source_->number_distributed() << '\n';
            throw std::runtime_error(msg.str());
        }
        source_->setNumber_source_necrosis(vec.x());
        source_->setNumber_source_intermediary(vec.y());
        source_->setNumber_source_external(vec.z());
    } else if (command == pos_in_cell_cmd_.get()) {
        double cell_membrane;
        double nucleoplasm;
        double nucleo_membrane;
        double cytoplasm;

        std::istringstream is(newValue.data());
        is >> cell_membrane >> nucleoplasm >> nucleo_membrane >> cytoplasm;

        source_->emission_in_membrane_ = cell_membrane;
        source_->emission_in_nucleus_ = nucleoplasm;
        source_->emission_in_nucleus_membrane_ = nucleo_membrane;
        source_->emission_cytoplasm_ = cytoplasm;

        source_->setOrganelle_weight(cell_membrane, nucleoplasm, nucleo_membrane, cytoplasm);
    }
    else if (command == max_sources_per_cell_cmd_.get()) {
      G4ThreeVector vec = max_sources_per_cell_cmd_->GetNew3VectorValue(newValue);
      source_->setMax_number_source_per_cell_necrosis(vec.x());
      source_->setMax_number_source_per_cell_intermediary(vec.y());
      source_->setMax_number_source_per_cell_external(vec.z());
   }
   else if (command == cell_Labeling_cmd_.get()) {
      G4ThreeVector vec = cell_Labeling_cmd_->GetNew3VectorValue(newValue);
      source_->setCell_Labeling_Percentage_necrosis(vec.x());
      source_->setCell_Labeling_Percentage_intermediary(vec.y());
      source_->setCell_Labeling_Percentage_external(vec.z());
   }
   else if (command == only_one_position_for_all_particles_on_a_cell_cmd_.get()) {
     source_->only_one_position_for_all_particles_on_a_cell = only_one_position_for_all_particles_on_a_cell_cmd_->GetNewIntValue(newValue) ;
   }
   else if (command == log_normal_distrib_cmd_.get()){
     G4Tokenizer next(newValue);
     bool is_log_normal_distrib = next();
     int mean_nb_particles_per_cell = StoI(next());
     double shape_factor = StoD(next());

     source_->is_log_norm_distribution = is_log_normal_distrib;
     source_->mean_ppc = mean_nb_particles_per_cell;
     source_->shape_factor = shape_factor;
   }
}

}
