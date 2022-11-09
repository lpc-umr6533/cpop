/*----------------------
Copyright (C): Henri Payno, Axel Delsol, 
Laboratoire de Physique de Clermont UMR 6533 CNRS-UCA

This software is distributed under the terms
of the GNU Lesser General  Public Licence (LGPL)
See LICENSE.md for further details
----------------------*/
#include "catch.hpp"

#include "G4UImanager.hh"

#include "Population.hh"
#include "SpheroidRegion.hh"
#include <algorithm>

TEST_CASE("Population test", "[Population]") {

    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;

    REQUIRE(population.verbose_level() == 0);

    auto centroid = population.spheroid_centroid();
    REQUIRE(centroid.x() == 0);
    REQUIRE(centroid.y() == 0);
    REQUIRE(centroid.z() == 0);

    REQUIRE(population.number_max_facet_poly() == 0);

    REQUIRE(population.delta_reffinement() == -1);

    REQUIRE(population.population_file().empty());

    SECTION("Initialize population. Error facet per poly") {
        population.setPopulation_file("population.xml");
        REQUIRE_THROWS_WITH(population.loadPopulation(),
                            Catch::Matchers::Contains( "nbMaxFacetPerPoly should be strictly positive." )
                            );
    }

    SECTION("Initialize population. Error delta ref") {
        population.setPopulation_file("population.xml");
        population.setNumber_max_facet_poly(100);
        REQUIRE(population.number_max_facet_poly() == 100);
        REQUIRE_THROWS_WITH(population.loadPopulation(),
                            Catch::Matchers::Contains( "deltaReffinement should be positive." )
                            );
    }

    SECTION("Initialize population. No error") {
        population.setPopulation_file("population.xml");
        population.setVerbose_level(0);

        population.setNumber_max_facet_poly(100);
        REQUIRE(population.number_max_facet_poly() == 100);

        population.setDelta_reffinement(0);
        REQUIRE(population.delta_reffinement() == 0);

        REQUIRE_NOTHROW(population.loadPopulation());

        auto centroid = population.spheroid_centroid();
        REQUIRE(centroid.x() == 0);
        REQUIRE(centroid.y() == 0);
        REQUIRE(centroid.z() == 0);

        auto radius = population.spheroid_radius();
        REQUIRE(radius == Approx(230.419*CLHEP::micrometer).margin(0.01));


    }
}

TEST_CASE("Spheroid region test", "[Population]") {
    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;
    population.setPopulation_file("population.xml");
    population.setVerbose_level(0);
    population.setNumber_max_facet_poly(100);
    population.setDelta_reffinement(0);
    population.loadPopulation();

    auto cells = population.cells();
    double radius = population.spheroid_radius();
    auto center = population.spheroid_centroid();
    cpop::SpheroidRegion necrosis = cpop::SpheroidRegion("necrosis", center, 0, 0.25*radius, cells.begin(), cells.end());

    SECTION("Constructor") {
        REQUIRE(necrosis.name() == "necrosis");
        REQUIRE(necrosis.internal_radius() == 0.);
        REQUIRE(necrosis.external_radius() == Approx(57.60475*CLHEP::micrometer).margin(0.001));
    }

    SECTION("Sampling. Error") {
        REQUIRE_THROWS_WITH(necrosis.sample(20),
                            Catch::Matchers::Contains( "Can not select 20 cells because region contains 14 cells.\n" )
                            );
    }

    SECTION("Sampling. fixed sample") {
        auto sampled_cells = necrosis.sample(10);
        REQUIRE(sampled_cells.size() == 10);
    }

    SECTION("Sampling. No sampling") {
        auto sampled_cells = necrosis.sample();
        REQUIRE(sampled_cells.size() == 14);
        for(auto&& cell : sampled_cells) {
            REQUIRE(necrosis.isSampled(cell));
        }
    }
}

TEST_CASE("Spheroid region in population", "[Population]")  {
    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;
    population.setPopulation_file("population.xml");
    population.setVerbose_level(0);
    population.setNumber_max_facet_poly(100);
    population.setDelta_reffinement(0);
    population.loadPopulation();

    SECTION("Region. Error internal ratio") {
        REQUIRE_THROWS_WITH(population.defineRegion(),
                            Catch::Matchers::Contains("InternalLayerRatio should be in [0;1]. Current value : -1.0")
                            );
    }

    SECTION("Region. Error intermediary ratio") {
        population.setInternal_layer_ratio(0.25);
        population.setIntermediary_layer_ratio(-2.5);
        REQUIRE_THROWS_WITH(population.defineRegion(),
                            Catch::Matchers::Contains("IntermediaryLayerRatio should be in [0;1]. Current value : -2.5")
                            );
    }

    SECTION("Region. No error") {
        population.setVerbose_level(1);
        population.setInternal_layer_ratio(0.25);
        population.setIntermediary_layer_ratio(0.75);
        REQUIRE_NOTHROW(population.defineRegion());

        auto cells = population.cells();
        auto sampled_cells = population.sampled_cells();
        REQUIRE(cells.size() == sampled_cells.size());
    }

    SECTION("Region. No error and fixed sampled size") {
        population.setVerbose_level(1);
        population.setInternal_layer_ratio(0.25);
        population.setIntermediary_layer_ratio(0.75);
        population.setNumber_sampling_cell_per_region(10);
        REQUIRE_NOTHROW(population.defineRegion());
        auto sampled_cells = population.sampled_cells();
        REQUIRE(sampled_cells.size() == 30);
    }
}


TEST_CASE("Population messenger", "[Population]") {
    CLHEP::MTwistEngine defaultEngineCPOP(1234567);
    RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

    cpop::Population population;
    std::string base_name = "/cpop";
    // Build the commands
    population.messenger().BuildCommands(base_name);

    SECTION("Set verbose level") {
        std::string macro = "verbose.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.verbose_level() == 1);
    }

    SECTION("Set population file") {
        std::string macro = "population.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.population_file() == "population.xml");
    }

    SECTION("Set facet") {
        std::string macro = "facet.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.number_max_facet_poly() == 200);
    }

    SECTION("Set facet") {
        std::string macro = "delta.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.delta_reffinement() == Approx(1.0).margin(10E-9));
    }

    SECTION("Set internal ratio") {
        std::string macro = "internalRatio.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.internal_layer_ratio() == Approx(0.25).margin(10E-9));
    }

    SECTION("Set intermediary ratio") {
        std::string macro = "intermediaryRatio.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.intermediary_layer_ratio() == Approx(0.75).margin(10E-9));
    }

    SECTION("Set sampling") {
        std::string macro = "sampling.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.number_sampling_cell_per_region() == 12);
    }

    SECTION("Set init") {
        std::string macro = "init.mac";
        // Get the pointer to the User Interface manager
        G4UImanager* UImanager = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command+macro);

        REQUIRE(population.verbose_level() == 1);
        REQUIRE(population.population_file() == "population.xml");
        REQUIRE(population.number_max_facet_poly() == 100);
        REQUIRE(population.delta_reffinement() == Approx(0).margin(10E-9));
        REQUIRE(population.internal_layer_ratio() == Approx(0.25).margin(10E-9));
        REQUIRE(population.intermediary_layer_ratio() == Approx(0.75).margin(10E-9));
        REQUIRE(population.number_sampling_cell_per_region() == 12);
        auto sampled_cells = population.sampled_cells();
        REQUIRE(sampled_cells.size() == 36);
    }


}






















