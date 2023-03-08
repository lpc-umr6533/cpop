#include "catch.hpp"

#include <fstream>

#include "G4UImanager.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ThreeVector.hh"
#include "G4Electron.hh"

#include "Population.hh"
#include "HomogeneousSource.hh"
#include "DistributedSource.hh"

//

TEST_CASE("Homogeneous source", "[source]") {

    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;
    population.setPopulation_file("population.xml");
    population.setVerbose_level(0);
    population.setNumber_max_facet_poly(100);
    population.setDelta_reffinement(0);
    population.loadPopulation();

    G4Electron::Electron();

    SECTION("Constructor") {
        cpop::HomogeneousSource* source = new cpop::HomogeneousSource("source", population);

        REQUIRE(source != nullptr);
        REQUIRE(source->source_name() == "source");
        REQUIRE(source->particle() == nullptr);
        REQUIRE(source->already_generated() == 0);
        REQUIRE(source->total_particle() == 0);
        REQUIRE(!source->HasLeft());
        delete source;
    }

    SECTION("Set particle") {
        cpop::HomogeneousSource source{"electron", population};

        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
        REQUIRE(particle != nullptr);
        source.setParticle(particle);

        REQUIRE(source.particle() == particle);
    }

    SECTION("Set number of particle to generate") {
        cpop::HomogeneousSource source{"electron", population};

        source.setTotal_particle(10000);
        REQUIRE(source.total_particle() == 10000);
    }

    SECTION("Momentum") {
        cpop::HomogeneousSource source{"electron", population};

        int number_particle = 10000;

        std::ofstream file("momentum.txt");

        G4ThreeVector point;
        for(int i = 0; i < number_particle; ++i) {
            point = source.GetMomentum();
            file << point.x() << " " << point.y() << " " << point.z() << "\n";

        }

        file.close();

    }

    SECTION("Generation inside the spheroid") {
        cpop::HomogeneousSource source{"electron", population};

        int number_particle = 10000;
        source.setTotal_particle(number_particle);

        std::ofstream file("position.txt");

        G4ThreeVector point;
        for(int i = 0; i < number_particle; ++i) {
            point = (source.GetPosition()).front();
            file << point.x() << " " << point.y() << " " << point.z() << "\n";
            source.Update();

        }

        REQUIRE(!source.HasLeft());

        file.close();

    }

    SECTION("Messenger") {
        cpop::HomogeneousSource source{"electron", population};
        G4String base = "/cpop/source/" + source.source_name();
        source.messenger().BuildCommands(base);

        std::string macro = "homogeneous.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
        REQUIRE(source.particle() == particle);
        REQUIRE(source.total_particle() == 1000);
    }
}

TEST_CASE("Distributed source, [source]") {
    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    G4Electron::Electron();

    SECTION("Constructor") {

        cpop::Population population;
        population.setPopulation_file("population.xml");
        population.setVerbose_level(0);
        population.setNumber_max_facet_poly(100);
        population.setDelta_reffinement(0);
        population.loadPopulation();
        cpop::DistributedSource* source = new cpop::DistributedSource("source", population);

        REQUIRE(source != nullptr);
        REQUIRE(source->source_name() == "source");
        REQUIRE(source->particle() == nullptr);
        REQUIRE(!source->HasLeft());
        delete source;
    }

    SECTION("Messenger") {
        cpop::Population population;
        std::string base_name = "/cpop";
        population.messenger().BuildCommands(base_name);

        cpop::DistributedSource source{"gadolinium", population};
        G4String base = "/cpop/source/" + source.source_name();
        source.messenger().BuildCommands(base);

        std::string macro = "nanoparticle1.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        source.Initialize();

        G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");
        REQUIRE(source.particle() == particle);
        REQUIRE(source.number_distributed() == 6);
        REQUIRE(source.number_secondary_per_nano() == 100);
        REQUIRE(source.number_nanoparticle_necrosis() == 1);
        REQUIRE(source.number_nanoparticle_intermediary() == 2);
        REQUIRE(source.number_nanoparticle_external() == 3);
        REQUIRE(source.total_particle() == 600);

        REQUIRE(source.HasLeft());
    }

    SECTION("Generation") {
        cpop::Population population;
        std::string base_name = "/cpop";
        population.messenger().BuildCommands(base_name);

        cpop::DistributedSource source{"gadolinium", population};
        G4String base = "/cpop/source/" + source.source_name();
        source.messenger().BuildCommands(base);

        std::string macro = "nanoparticle2.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        source.Initialize();

        int totalSecondaries = 3000;
        REQUIRE(source.total_particle() == 3000);

        std::ofstream file("nanoparticlePosition.txt");

        REQUIRE(source.HasLeft());
        G4ThreeVector point;
        for(int i = 0; i < totalSecondaries; ++i) {
            point = (source.GetPosition()).front();
            file << point.x() << " " << point.y() << " " << point.z() << "\n";
            source.Update();

        }

        REQUIRE(!source.HasLeft());

        file.close();

    }
}
