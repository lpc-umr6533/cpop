#include "DistributedSourceMessenger.hh"

#include <sstream>

#include "G4UIparameter.hh"

#include "DistributedSource.hh"
#include "G4Tokenizer.hh"

namespace cpop {

DistributedSourceMessenger::DistributedSourceMessenger(DistributedSource *source):
	_source(source),
	_sourceMessenger(std::make_unique<SourceMessenger>(source))
{
}

void DistributedSourceMessenger::BuildCommands(G4String base) {
	_sourceMessenger->BuildCommands(base);

	G4String cmd_base = base + "/totalSource";
	_totalSourceCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_base, this);
	_totalSourceCmd->SetGuidance("Set the number of sources in the population");
	_totalSourceCmd->SetParameterName("TotalSource", false);
	_totalSourceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/particlesPerSource";
	_partPerSourceCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_base,
																																this);
	_partPerSourceCmd->SetGuidance("Set the number of particles emitted from"
																		"one source");
	_partPerSourceCmd->SetParameterName("PartPerNano", false);
	_partPerSourceCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/distributionInRegion";
	_nanoPerRegionCmd = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
	_nanoPerRegionCmd->SetGuidance("Distribute the nanoparticle  in the 3 regions");
	_nanoPerRegionCmd->SetParameterName("nNecrosis", "nIntermediary", "nExternal", false);
	_nanoPerRegionCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/distributionInCell";
	_posInCellCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	auto* cell_membrane = new G4UIparameter("CellMembrane", 'd', false);
	_posInCellCmd->SetParameter(cell_membrane);
	auto* nucleoplasm = new G4UIparameter("Nucleoplasm", 'd', false);
	_posInCellCmd->SetParameter(nucleoplasm);
	auto* nucleo_membrane = new G4UIparameter("NucleusMembrane", 'd', false);
	_posInCellCmd->SetParameter(nucleo_membrane);
	auto* cytoplasm = new G4UIparameter("Cytoplasm", 'd', false);
	_posInCellCmd->SetParameter(cytoplasm);
	_posInCellCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/maxSourcesPerCell";
	_maxSourcesPerCellCmd = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
	_maxSourcesPerCellCmd->SetGuidance("Set the maximum number of sources that can be attached to a cell");
	_maxSourcesPerCellCmd->SetParameterName("MaxSourcePerCell_Necrosis", "MaxSourcesPerCell_Intermediary", "MaxSourcesPerCell_External", false);
	_maxSourcesPerCellCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/cellLabelingPercentagePerRegion";
	_cellLabelingCmd = std::make_unique<G4UIcmdWith3Vector>(cmd_base, this);
	_cellLabelingCmd->SetGuidance("Set the percentage of labeled cells with nanoparticles, per region");
	_cellLabelingCmd->SetParameterName("cellLabeling%_Necrosis", "cellLabeling%_Intermediary", "cellLabeling%_External",false);
	_cellLabelingCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/only_one_position_for_all_particles_on_a_cell";
	_onlyOnePositionForAllParticlesOnACellCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_base, this);
	_onlyOnePositionForAllParticlesOnACellCmd->SetGuidance("Allow only one position for all particles generated on the same cell");
	_onlyOnePositionForAllParticlesOnACellCmd->SetParameterName("Only_one_position_for_all_particles_on_a_cell", false);
	_onlyOnePositionForAllParticlesOnACellCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

	cmd_base = base + "/log_normal_distribution";
	_logNormalDistribCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	_logNormalDistribCmd->SetGuidance("log_normal_distribution");
	auto* choice_distrib = new G4UIparameter("bool_choice", 'b', true);
	_logNormalDistribCmd->SetParameter(choice_distrib);
	auto* mean_number_particle = new G4UIparameter("mean_number_particle", 'i', true);
	_logNormalDistribCmd->SetParameter(mean_number_particle);
	auto* shape_factor = new G4UIparameter("shape_factor", 'd', true);
	_logNormalDistribCmd->SetParameter(shape_factor);
	_logNormalDistribCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

void DistributedSourceMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	_sourceMessenger->SetNewValue(command, newValue);

	if (command == _totalSourceCmd.get()) {
		_source->setNumber_source(_totalSourceCmd->GetNewIntValue(newValue));
	} else if (command == _partPerSourceCmd.get()) {
		_source->setNumber_particles_per_source(_partPerSourceCmd->GetNewIntValue(newValue));
	} else if (command == _nanoPerRegionCmd.get()) {
		G4ThreeVector vec = _nanoPerRegionCmd->GetNew3VectorValue(newValue);
		int sum = vec.x() + vec.y() + vec.z();
		if (sum != _source->number_distributed()) {
			std::stringstream msg;
			msg << "Wrong distribution of the nanoparticle in the 3 regions : \n";
			msg << "Necrosis region : " << vec.x() << '\n';
			msg << "Intermediary region : " << vec.y() << '\n';
			msg << "External region : " << vec.z() << '\n';
			msg << "Sum : " << sum << '\n';
			msg << "Number of nanoparticle : " << _source->number_distributed() << '\n';
			throw std::runtime_error(msg.str());
		}
		_source->setNumber_source_necrosis(vec.x());
		_source->setNumber_source_intermediary(vec.y());
		_source->setNumber_source_external(vec.z());
	} else if (command == _posInCellCmd.get()) {
		double cell_membrane;
		double nucleoplasm;
		double nucleo_membrane;
		double cytoplasm;

		std::istringstream is(newValue.data());
		is >> cell_membrane >> nucleoplasm >> nucleo_membrane >> cytoplasm;

		_source->emissionInMembrane = cell_membrane;
		_source->emissionInNucleus = nucleoplasm;
		_source->emissionInNucleusMembrane = nucleo_membrane;
		_source->emissionCytoplasm = cytoplasm;

		_source->setOrganelle_weight(cell_membrane, nucleoplasm, nucleo_membrane, cytoplasm);
	} else if (command == _maxSourcesPerCellCmd.get()) {
		G4ThreeVector vec = _maxSourcesPerCellCmd->GetNew3VectorValue(newValue);
		_source->setMax_number_source_per_cell_necrosis(vec.x());
		_source->setMax_number_source_per_cell_intermediary(vec.y());
		_source->setMax_number_source_per_cell_external(vec.z());
	} else if (command == _cellLabelingCmd.get()) {
		G4ThreeVector vec = _cellLabelingCmd->GetNew3VectorValue(newValue);
		_source->setCell_Labeling_Percentage_necrosis(vec.x());
		_source->setCell_Labeling_Percentage_intermediary(vec.y());
		_source->setCell_Labeling_Percentage_external(vec.z());
	} else if (command == _onlyOnePositionForAllParticlesOnACellCmd.get()) {
		_source->onlyOnePositionForAllParticlesOnACell = _onlyOnePositionForAllParticlesOnACellCmd->GetNewIntValue(newValue) ;
	} else if (command == _logNormalDistribCmd.get()){
		G4Tokenizer next(newValue);
		bool is_log_normal_distrib = next();
		int mean_nb_particles_per_cell = StoI(next());
		double shape_factor = StoD(next());

		_source->isLogNormDistribution = is_log_normal_distrib;
		_source->meanPPC = mean_nb_particles_per_cell;
		_source->shapeFactor = shape_factor;
	}
}

}
