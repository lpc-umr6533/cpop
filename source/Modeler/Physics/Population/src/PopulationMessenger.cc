#include "PopulationMessenger.hh"
#include "Population.hh"

namespace cpop {

PopulationMessenger::PopulationMessenger(Population *population):
	_population(population)
{
}

void PopulationMessenger::BuildCommands(G4String base) {
	//G4String cmd_base = this->make_directory(base, "population");
	G4String cmd_base = base + "/population";

	G4String cmd_name = cmd_base + "/verbose";
	_verboseCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
	_verboseCmd->SetGuidance("Set vebose level. 0 (silent) 1 (output to terminal");
	_verboseCmd->SetParameterName("Verbose",true);
	_verboseCmd->SetDefaultValue(0);
	_verboseCmd->SetRange("Verbose == 0 || Verbose == 1");
	_verboseCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/input";
	_populationCmd = std::make_unique<G4UIcmdWithAString>(cmd_name, this);
	_populationCmd->SetGuidance("Set cell population file");
	_populationCmd->SetParameterName("CellPop",false);
	_populationCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/numberFacet";
	_numberFacetCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
	_numberFacetCmd->SetGuidance("Set maximum number of facet for each cell representation");
	_numberFacetCmd->SetParameterName("NumberFacet", true);
	_numberFacetCmd->SetDefaultValue(100);
	_numberFacetCmd->SetRange("NumberFacet > 0");
	_numberFacetCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/deltaRef";
	_deltaRefCmd = std::make_unique<G4UIcmdWithADouble>(cmd_name,this);
	_deltaRefCmd->SetGuidance("Set delta refinement parameter");
	_deltaRefCmd->SetParameterName("DeltaRef", true);
	_deltaRefCmd->SetDefaultValue(0);
	_deltaRefCmd->SetRange("DeltaRef >= 0");
	_deltaRefCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/internalRatio";
	_internalRatioCmd = std::make_unique<G4UIcmdWithADouble>(cmd_name,this);
	_internalRatioCmd->SetGuidance("Set internal layer ratio");
	_internalRatioCmd->SetParameterName("internalRatio", false);
	_internalRatioCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/intermediaryRatio";
	_intermediaryRatioCmd = std::make_unique<G4UIcmdWithADouble>(cmd_name, this);
	_intermediaryRatioCmd->SetGuidance("Set intermediary layer ratio");
	_intermediaryRatioCmd->SetParameterName("intermediaryRatio", false);
	_intermediaryRatioCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/sampling";
	_numberSamplingCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_name,this);
	_numberSamplingCmd->SetGuidance("Set number of cell to observe. Use ! to observe the whole population");
	_numberSamplingCmd->SetParameterName("Sampling", true);
	_numberSamplingCmd->SetDefaultValue(-1);
	_numberSamplingCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/init";
	_initCmd = std::make_unique<G4UIcmdWithoutParameter>(cmd_name,this);
	_initCmd->SetGuidance("Load population file and define regions");
	_initCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/stepInfo";
	_getSteppingLevelInfoCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
	_getSteppingLevelInfoCmd->SetGuidance("Get info at the stepping level");
	_getSteppingLevelInfoCmd->SetParameterName("StepInfoBool", true);
	_getSteppingLevelInfoCmd->SetDefaultValue(0);
	_getSteppingLevelInfoCmd->AvailableForStates(G4State_PreInit);

	cmd_name = cmd_base + "/eventInfo";
	_getEventLevelInfoCmd = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
	_getEventLevelInfoCmd->SetGuidance("Get info at the event level");
	_getEventLevelInfoCmd->SetParameterName("EventInfoBool", true);
	_getEventLevelInfoCmd->SetDefaultValue(0);
	_getEventLevelInfoCmd->AvailableForStates(G4State_PreInit);

	cmd_base = cmd_base + "/writeInfoPrimariesTxt";
	_infosPrimariesCmd = std::make_unique<G4UIcommand>(cmd_base, this);
	_infosPrimariesCmd->SetGuidance("Write positions,"
	 "directions and energy of primary particles in .txt");
	auto* bool_info_primaries = new G4UIparameter("bool_info_primaries", 's', false);
	_infosPrimariesCmd->SetParameter(bool_info_primaries);
	auto* name_file = new G4UIparameter("name_file", 's', false);
	_infosPrimariesCmd->SetParameter(name_file);
	_infosPrimariesCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

void PopulationMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
	if (command == _verboseCmd.get()) {
		_population->setVerbose_level(_verboseCmd->GetNewIntValue(newValue));
	} else if (command == _populationCmd.get()) {
		_population->setPopulation_file(newValue.data());
	} else if (command == _numberFacetCmd.get()) {
		_population->setNumber_max_facet_poly(_numberFacetCmd->GetNewIntValue(newValue));
	} else if (command == _deltaRefCmd.get()) {
		_population->setDelta_reffinement(_deltaRefCmd->GetNewDoubleValue(newValue));
	} else if (command == _internalRatioCmd.get()) {
		_population->setInternal_layer_ratio(_internalRatioCmd->GetNewDoubleValue(newValue));
	} else if (command == _intermediaryRatioCmd.get()) {
		_population->setIntermediary_layer_ratio(_intermediaryRatioCmd->GetNewDoubleValue(newValue));
	} else if (command == _numberSamplingCmd.get()) {
		_population->setNumber_sampling_cell_per_region(_numberSamplingCmd->GetNewIntValue(newValue));
	} else if (command == _initCmd.get()) {
		_population->loadPopulation();
		_population->defineRegion();
	} else if (command == _getSteppingLevelInfoCmd.get()) {
		_population->set_Stepping_level_info_bool(_getSteppingLevelInfoCmd->GetNewIntValue(newValue));
	} else if (command == _getEventLevelInfoCmd.get()) {
		_population->set_Event_level_info_bool(_getEventLevelInfoCmd->GetNewIntValue(newValue));
	} else if (command == _infosPrimariesCmd.get()) {
		G4String bool_writing;
		G4String name_file;

		std::istringstream is(newValue.data());
		is >> bool_writing >> name_file;
		_population->enableWritingInfoPrimariesTxt(bool_writing, name_file);
	}
}

}
