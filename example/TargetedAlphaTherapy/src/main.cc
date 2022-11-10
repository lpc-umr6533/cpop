#define G4MULTITHREADED // TODO hack
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include <cReader/zupply.hpp>
#include "Population.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
		auto start = std::chrono::high_resolution_clock::now();


    CLHEP::MTwistEngine defaultEngine(time(NULL));
    G4Random::setTheEngine(&defaultEngine);
    //G4int seed = time(NULL);
    //G4Random::setTheSeed(seed);

    CLHEP::MTwistEngine defaultEngineCPOP(time(NULL));
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

    // check errors
    if (parser.count_error() > 0)
    {
        std::cout << parser.get_error() << std::endl;
        // print help
        std::cout << parser.get_help() << std::endl;
        // continue or exit()??
        exit(-1);
    }


    // Construct the default run manager
    //
#ifdef G4MULTITHREADED
    G4MTRunManager * runManager = new G4MTRunManager;
    if ( nThreads > 0 ) {
        runManager->SetNumberOfThreads(nThreads);
    }
#else
    G4RunManager * runManager = new G4RunManager;
#endif

		// Create a population

		std::ofstream fid;
		fid.open ("../../example/TXT/IDCell.txt");

		cpop::Population population;
		population.messenger().BuildCommands("/cpop");

    // Set mandatory initialization classes
    //

    // Set the geometry ie a box filled with G4_WATER
    cpop::DetectorConstruction* detector = new cpop::DetectorConstruction(population);
    runManager->SetUserInitialization(detector);

    // Set the physics list
    cpop::PhysicsList* physicsList = new cpop::PhysicsList();
    physicsList->messenger().BuildCommands("/cpop/physics");
    runManager->SetUserInitialization(physicsList);

    // Set custom action to extract informations from the simulation
    cpop::ActionInitialization* actionInitialisation = new cpop::ActionInitialization(population);
    runManager->SetUserInitialization(actionInitialisation);


    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+macro);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

     std::cout << "elapsed time: " << elapsed_seconds.count() << " s\n";


    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete runManager;


    return 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
