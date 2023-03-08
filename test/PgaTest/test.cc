#include "catch.hpp"

#include "G4UImanager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4Electron.hh"

#include "Population.hh"
#include "HomogeneousSource.hh"
//#include "Source.hh"
#include "PGA_impl.hh"

TEST_CASE("Primary Generator Action test", "[PGA]") {

    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;
    population.messenger().BuildCommands("/cpop");

    G4Electron::Electron();
    cpop::PGA_impl::resetFlags();

//    SECTION("Homogeneous") {
//        cpop::PGA_impl pga(population);
//        pga.messenger().BuildCommands("/cpop/source");
//        REQUIRE(pga.TotalEvent() == 0);

//        std::string macro = "init.mac";
//        // Get the pointer to the User Interface manager
//        G4UImanager* UImanager = G4UImanager::GetUIpointer();
//        G4String command = "/control/execute ";
//        UImanager->ApplyCommand(command+macro);

//        // Check population settings
//        REQUIRE(population.verbose_level() == 1);
//        REQUIRE(population.population_file() == "population.xml");
//        REQUIRE(population.number_max_facet_poly() == 100);
//        REQUIRE(population.delta_reffinement() == Approx(0).margin(10E-9));
//        REQUIRE(population.internal_layer_ratio() == Approx(0.25).margin(10E-9));
//        REQUIRE(population.intermediary_layer_ratio() == Approx(0.75).margin(10E-9));
//        REQUIRE(population.number_sampling_cell_per_region() == 12);
//        auto sampled_cells = population.sampled_cells();
//        REQUIRE(sampled_cells.size() == 36);


//        // Check pga settings
//        cpop::HomogeneousSource* source = pga.homogeneous_source();
//        REQUIRE(source != nullptr);

//        // Check source settings
//        REQUIRE(source->source_name() == "electron");
//        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
//        REQUIRE(particle != nullptr);
//        REQUIRE(source->particle() == particle);
//        REQUIRE(source->total_particle() == 1000);
//    }

//    SECTION("Nanoparticle") {
//        cpop::PGA_impl pga(population);
//        pga.messenger().BuildCommands("/cpop/source");

//        std::string macro = "nano.mac";
//        // Get the pointer to the User Interface manager
//        G4UImanager* UImanager = G4UImanager::GetUIpointer();
//        G4String command = "/control/execute ";
//        UImanager->ApplyCommand(command+macro);

//        // Check pga settings
//        pga.Initialize();
//        cpop::DistributedSource* source = pga.distributed_source();
//        REQUIRE(source != nullptr);

//        // Check source settings
//        REQUIRE(source->source_name() == "gadolinium");
//        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
//        REQUIRE(particle != nullptr);
//        REQUIRE(source->particle() == particle);
//        REQUIRE(source->total_particle() == 600);
//    }

    SECTION("Both") {
        cpop::PGA_impl pga(population);
        pga.messenger().BuildCommands("/cpop/source");

        std::string macro = "both.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        // Check pga settings
        cpop::HomogeneousSource* sourceH = pga.homogeneous_source();
        REQUIRE(sourceH != nullptr);

        // Check source settings
        REQUIRE(sourceH->source_name() == "electron");
        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
        REQUIRE(particle != nullptr);
        REQUIRE(sourceH->particle() == particle);
        REQUIRE(sourceH->total_particle() == 1000);

        // Check pga settings
        cpop::DistributedSource* sourceN = pga.distributed_source();
        REQUIRE(sourceN != nullptr);

        // Check source settings
        REQUIRE(sourceN->source_name() == "gadolinium");
        REQUIRE(particle != nullptr);
        REQUIRE(sourceN->particle() == particle);
        REQUIRE(sourceN->total_particle() == 600);
    }

    SECTION("Generation") {
        cpop::PGA_impl pga(population);
        pga.messenger().BuildCommands("/cpop/source");

        std::string macro = "both.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        int expected_number_event = 1000 + 6*100;
        REQUIRE(expected_number_event == pga.TotalEvent());

        // Check pga settings
        cpop::DistributedSource* sourceN = pga.distributed_source();
        REQUIRE(sourceN != nullptr);

        // Check pga settings
        cpop::HomogeneousSource* sourceH = pga.homogeneous_source();
        REQUIRE(sourceH != nullptr);

        for(int i = 0 ; i < expected_number_event; ++i) {
            cpop::Source* source = pga.selectSource();
            REQUIRE(source != nullptr);
            source->GetPosition();
            source->Update();
        }

        REQUIRE(pga.selectSource() == nullptr);
        REQUIRE(cpop::PGA_impl::nbHomogeneous == 1000);
        REQUIRE(cpop::PGA_impl::nbDistributed == 600);
    }


}
