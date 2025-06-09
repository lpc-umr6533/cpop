#include "Cell_Utils.hh"
#include "Octree.hh"
#include "OctreeNodeForSpheroidalCell.hh"
#include "catch.hpp"

#include <chrono>

#include "G4UImanager.hh"

#include "AgentSettings.hh"
#include "CGAL_Utils.hh"
#include "CellSettings.hh"
#include "Population.hh"
#include "RandomEngineManager.hh"
#include "UniformSource.hh"
#include "Randomize.hh"

TEST_CASE("Stepping action", "[UserAction]") {
	CLHEP::MTwistEngine defaultEngineCPOP(1234567);
	RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

	CLHEP::MTwistEngine defaultEngine(7654321);
	G4Random::setTheEngine(&defaultEngine);
	G4int seed = time(nullptr);
	G4Random::setTheSeed(seed);

	cpop::Population population;
	std::string base_name = "/cpop";
	// Build the commands
	population.messenger().BuildCommands(base_name);


	SECTION("Position in cell") {
		std::string macro = "init.mac";
		// Get the pointer to the User Interface manager
		G4UImanager* UImanager = G4UImanager::GetUIpointer();
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);

		cpop::UniformSource source{"source", population};

		std::vector<const Settings::nCell::t_Cell_3*> sampled_cells = population.sampled_cells();
		std::vector<const Settings::nAgent::t_SpatialableAgent_3*> spatialables(sampled_cells.begin(), sampled_cells.end());

		int nbCellPerNode = 2000;
		auto octree = std::make_unique<Octree<OctreeNodeForSpheroidalCell>>(
			Utils::getBoundingBox(spatialables.begin(), spatialables.end()),
			&spatialables,
			nbCellPerNode);

		int number_particle = 1000000;
		source.setTotal_particle(number_particle);

		std::ofstream file("position.txt");

		// Step simulation and use octree to find cell
		std::cout << "Starting simulation" << std::endl;
		auto start = std::chrono::system_clock::now();
		G4ThreeVector pt;
		for(int i = 0; i < number_particle; ++i) {
			pt = (source.GetPosition()).front();
			source.Update();
			Point_3 point = Utils::myCGAL::to_CPOP(pt);
			const t_SpatialableAgent_3* lNearestAgent = octree->getNearestSpatialableAgent(point);
			auto cell = dynamic_cast<const Settings::nCell::t_Cell_3*>(lNearestAgent);
			if (cell && cell->hasIn(point)) {
				file << pt.x() << " " << pt.y() << " " << pt.z() << "\n";
			}
		}

		auto end = std::chrono::system_clock::now();
		int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
		std::cout << "elapsed time: " << elapsed_ms << "ms\n";

		file.close();
		REQUIRE(!source.HasLeft());
	}

	SECTION("Position in Cell with memory") {
		std::string macro = "init.mac";
		// Get the pointer to the User Interface manager
		G4UImanager* UImanager = G4UImanager::GetUIpointer();
		G4String command = "/control/execute ";
		UImanager->ApplyCommand(command+macro);

		cpop::UniformSource source{"source", population};

		std::vector<const Settings::nCell::t_Cell_3*> sampled_cells = population.sampled_cells();
		std::vector<const Settings::nAgent::t_SpatialableAgent_3*> spatialables(sampled_cells.begin(), sampled_cells.end());

		int nbCellPerNode = 2000;
		auto octree = std::make_unique<Octree<OctreeNodeForSpheroidalCell>>(
			Utils::getBoundingBox(spatialables.begin(), spatialables.end()),
			&spatialables,
			nbCellPerNode);

		int number_particle = 1000000;
		source.setTotal_particle(number_particle);

		std::ofstream file("position2.txt");

		// Step simulation and use octree to find cell
		G4ThreeVector pt;
		const Settings::nCell::t_Cell_3* last_cell = nullptr;
		const Settings::nCell::t_Cell_3* cell = nullptr;
		const t_SpatialableAgent_3* lNearestAgent = nullptr;
		std::cout << "Starting simulation" << std::endl;
		auto start = std::chrono::system_clock::now();
		for(int i = 0; i < number_particle; ++i) {
			pt = (source.GetPosition()).front();
			source.Update();
			Point_3 point = Utils::myCGAL::to_CPOP(pt);

			if(last_cell && last_cell->hasIn(point)) {
				file << pt.x() << " " << pt.y() << " " << pt.z() << "\n";
			} else {
				lNearestAgent = octree->getNearestSpatialableAgent(point);
				cell = dynamic_cast<const Settings::nCell::t_Cell_3*>(lNearestAgent);
				if (cell && cell->hasIn(point)) {
					file << pt.x() << " " << pt.y() << " " << pt.z() << "\n";
					last_cell = cell;
				}
			}
		}

		auto end = std::chrono::system_clock::now();
		int elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
		std::cout << "elapsed time: " << elapsed_ms << "ms\n";

		file.close();
		REQUIRE(!source.HasLeft());

	}


}
