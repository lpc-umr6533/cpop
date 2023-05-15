#include "PopulationMessenger.hh"
#include "Population.hh"



namespace cpop {

PopulationMessenger::PopulationMessenger(Population *population)
    :MessengerBase(),
      population_(population)
{

}

void PopulationMessenger::BuildCommands(G4String base)
{
    //G4String cmd_base = this->make_directory(base, "population");
    G4String cmd_base = base + "/population";

    G4String cmd_name = cmd_base + "/verbose";
    verbose_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
    verbose_cmd_->SetGuidance("Set vebose level. 0 (silent) 1 (output to terminal");
    verbose_cmd_->SetParameterName("Verbose",true);
    verbose_cmd_->SetDefaultValue(0);
    verbose_cmd_->SetRange("Verbose == 0 || Verbose == 1");
    verbose_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/input";
    population_cmd_ = std::make_unique<G4UIcmdWithAString>(cmd_name, this);
    population_cmd_->SetGuidance("Set cell population file");
    population_cmd_->SetParameterName("CellPop",false);
    population_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/numberFacet";
    number_facet_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
    number_facet_cmd_->SetGuidance("Set maximum number of facet for each cell representation");
    number_facet_cmd_->SetParameterName("NumberFacet", true);
    number_facet_cmd_->SetDefaultValue(100);
    number_facet_cmd_->SetRange("NumberFacet > 0");
    number_facet_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/deltaRef";
    delta_ref_cmd_ = std::make_unique<G4UIcmdWithADouble>(cmd_name,this);
    delta_ref_cmd_->SetGuidance("Set delta refinement parameter");
    delta_ref_cmd_->SetParameterName("DeltaRef", true);
    delta_ref_cmd_->SetDefaultValue(0);
    delta_ref_cmd_->SetRange("DeltaRef >= 0");
    delta_ref_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/internalRatio";
    internal_ratio_cmd_ = std::make_unique<G4UIcmdWithADouble>(cmd_name,this);
    internal_ratio_cmd_->SetGuidance("Set internal layer ratio");
    internal_ratio_cmd_->SetParameterName("internalRatio", false);
    internal_ratio_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/intermediaryRatio";
    intermediary_ratio_cmd_ = std::make_unique<G4UIcmdWithADouble>(cmd_name, this);
    intermediary_ratio_cmd_->SetGuidance("Set intermediary layer ratio");
    intermediary_ratio_cmd_->SetParameterName("intermediaryRatio", false);
    intermediary_ratio_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/sampling";
    number_sampling_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_name,this);
    number_sampling_cmd_->SetGuidance("Set number of cell to observe. Use ! to observe the whole population");
    number_sampling_cmd_->SetParameterName("Sampling", true);
    number_sampling_cmd_->SetDefaultValue(-1);
    number_sampling_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/init";
    init_cmd_ = std::make_unique<G4UIcmdWithoutParameter>(cmd_name,this);
    init_cmd_->SetGuidance("Load population file and define regions");
    init_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/stepInfo";
    get_stepping_level_info_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
    get_stepping_level_info_cmd_->SetGuidance("Get info at the stepping level");
    get_stepping_level_info_cmd_->SetParameterName("StepInfoBool", true);
    get_stepping_level_info_cmd_->SetDefaultValue(0);
    get_stepping_level_info_cmd_->AvailableForStates(G4State_PreInit);

    cmd_name = cmd_base + "/eventInfo";
    get_event_level_info_cmd_ = std::make_unique<G4UIcmdWithAnInteger>(cmd_name, this);
    get_event_level_info_cmd_->SetGuidance("Get info at the event level");
    get_event_level_info_cmd_->SetParameterName("EventInfoBool", true);
    get_event_level_info_cmd_->SetDefaultValue(0);
    get_event_level_info_cmd_->AvailableForStates(G4State_PreInit);

    cmd_base = cmd_base + "/writeInfoPrimariesTxt";
    infos_primaries_cmd_ = std::make_unique<G4UIcommand>(cmd_base, this);
    infos_primaries_cmd_->SetGuidance("Write positions,"
     "directions and energy of primary particles in .txt");
    G4UIparameter* bool_info_primaries = new G4UIparameter("bool_info_primaries",
                                                          's', false);
    infos_primaries_cmd_->SetParameter(bool_info_primaries);
    G4UIparameter* name_file = new G4UIparameter("name_file", 's', false);
    infos_primaries_cmd_->SetParameter(name_file);
    infos_primaries_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);


}

void PopulationMessenger::SetNewValue(G4UIcommand *command, G4String newValue)
{
    if (command == verbose_cmd_.get()) {
        population_->setVerbose_level(verbose_cmd_->GetNewIntValue(newValue));
    } else if (command == population_cmd_.get()) {
        population_->setPopulation_file(newValue.data());
    } else if (command == number_facet_cmd_.get()) {
        population_->setNumber_max_facet_poly(number_facet_cmd_->GetNewIntValue(newValue));
    } else if (command == delta_ref_cmd_.get()) {
        population_->setDelta_reffinement(delta_ref_cmd_->GetNewDoubleValue(newValue));
    } else if (command == internal_ratio_cmd_.get()) {
        population_->setInternal_layer_ratio(internal_ratio_cmd_->GetNewDoubleValue(newValue));
    } else if (command == intermediary_ratio_cmd_.get()) {
        population_->setIntermediary_layer_ratio(intermediary_ratio_cmd_->GetNewDoubleValue(newValue));
    } else if (command == number_sampling_cmd_.get()) {
        population_->setNumber_sampling_cell_per_region(number_sampling_cmd_->GetNewIntValue(newValue));
    } else if (command == init_cmd_.get()) {
        population_->loadPopulation();
        population_->defineRegion();
    } else if (command == get_stepping_level_info_cmd_.get()) {
        population_->set_Stepping_level_info_bool(get_stepping_level_info_cmd_->GetNewIntValue(newValue));
    } else if (command == get_event_level_info_cmd_.get()) {
        population_->set_Event_level_info_bool(get_event_level_info_cmd_->GetNewIntValue(newValue));
    } else if (command == infos_primaries_cmd_.get()) {
        G4String bool_writing;
        G4String name_file;

        std::istringstream is(newValue.data());
        is >> bool_writing >> name_file;
        population_->enableWritingInfoPrimariesTxt(bool_writing, name_file);
    }


}

}
