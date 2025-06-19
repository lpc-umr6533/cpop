#define G4MULTITHREADED // TODO hack
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include <cReader/zupply.hpp>
#include "Population.hh"
#include "PhysicsList.hh"
#include "CpopActionInitialization.hh"

#include "DetectorConstruction.hh"

#include "G4UImanager.hh"
#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include <RandomEngineManager.hh>

#include <ctime>
#include <chrono>
#include <memory>

#include "DetectorConstruction.hh"

#include <filesystem>  // C++17 feature
namespace fs = std::filesystem;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv) {
	auto start = std::chrono::high_resolution_clock::now();

	CLHEP::MTwistEngine defaultEngine(time(nullptr));
	G4Random::setTheEngine(&defaultEngine);
	//G4int seed = time(NULL);
	//G4Random::setTheSeed(seed);

	CLHEP::MTwistEngine defaultEngineCPOP(time(nullptr));
	//CLHEP::MTwistEngine defaultEngineCPOP(1234567);
	RandomEngineManager::getInstance()->setEngine(&defaultEngineCPOP);

	// Command line arguments

	// First we add an argument parser to add parameters
	zz::cfg::ArgParser parser;

#ifdef G4MULTITHREADED
	// Get the spectrum file. Specify option -t nbThread or --thread nbThread
	int nThreads;
	parser.add_opt_value('t', "thread", nThreads, 0, "number of threads", "int");
#endif

	// Get the macro file. Specify option -m <fileName> or --macro <filename>
	std::string macro;
	parser.add_opt_value('m', "macro", macro, std::string("input_filename.mac"), "macro file", "file").require();

	parser.parse(argc, argv);
	if (parser.count_error() > 0) {
		std::cout << parser.get_error() << std::endl;
		std::cout << parser.get_help() << std::endl;
		exit(-1);
	}

	// Construct the default run manager
#ifdef G4MULTITHREADED
	auto* runManager = new G4MTRunManager;
	if ( nThreads > 0 ) {
		runManager->SetNumberOfThreads(nThreads);
	}
#else
	auto* runManager = new G4RunManager;
#endif

	// Create a population
	cpop::Population population;
	population.messenger().BuildCommands("/cpop");

	std::string home_path = std::filesystem::current_path().string();
	std::string output_txt_folder = home_path + "/OutputTxt";
	if (!fs::exists(output_txt_folder)) {
		if (!fs::create_directory(output_txt_folder)) {
			std::cerr << "Error: Failed to create directory: " << output_txt_folder << "\n";
		}
	}
	std::ofstream id_cell_file(output_txt_folder + "/IDCell.txt", std::ios::trunc);

	// Set mandatory initialization classes
	// Set the geometry ie a box filled with G4_WATER
	auto* detector = new cpop::DetectorConstruction(population);
	runManager->SetUserInitialization(detector);

	// Set the physics list
	auto* physicsList = new cpop::PhysicsList();
	physicsList->messenger().BuildCommands("/cpop/physics");
	runManager->SetUserInitialization(physicsList);

	// Set custom action to extract informations from the simulation
	auto* actionInitialisation = new cpop::CpopActionInitialization(population);
	runManager->SetUserInitialization(actionInitialisation);


	// Get the pointer to the User Interface manager
	auto* UImanager = G4UImanager::GetUIpointer();
	G4String command = "/control/execute ";
	UImanager->ApplyCommand(command+macro);

	id_cell_file.close();

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";

	{
		std::string outputDir = std::filesystem::absolute("output_stl").string();
		std::filesystem::create_directories(outputDir);
		std::string outputPath = outputDir + "/" + "cell";
		// simulationEnv->exportToSTL(outputPath, divided);
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted
	// in the main() program
	delete runManager;
}
