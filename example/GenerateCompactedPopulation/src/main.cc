#include <iostream>
#include <filesystem> // Include the filesystem library

// CPOP headers
#include <cReader/configreader.hh>
#include <cReader/zupply.hpp>


#include "unitSection.hh"
#include "cellSection.hh"
#include "spheroidSection.hh"
#include "meshSection.hh"
#include "forceSection.hh"
#include "simulationSection.hh"

// Header containing everything required to create a population
#include "simulationEnvironment.hh"

using namespace zz;

int main(int argc, char** argv) {
	// First we add an argument parser to simplify the use of the population generator
	zz::cfg::ArgParser argparser;

	// Generate visualization file. Specify option --vis in the command line. This is an optional flag
	bool vis;
	argparser.add_opt_flag(-1,"vis","generate off file to visualize population", &vis);

	// Get the configuration file from the command line. Specify option -f <fileName>
	std::string input;
	auto inputArg = argparser.add_opt_value('f', "", input, std::string("input_filename.cfg"), "configuration file", "file").require();

	//Retrieve arguments from command line
	argparser.parse(argc, argv);

	// check errors
	if (argparser.count_error() > 0) {
		std::cout << argparser.get_error() << std::endl;
		// print help
		std::cout << argparser.get_help() << std::endl;
		// continue or exit()??
		exit(-1);
	}

	// Extract the basename of the input file in order to create output files
	// Example : ../config.cfg will produce config.xml (and config.off if --vis is used)
	std::string basename = zz::os::path_split_basename(input);

	// Create the configuration file
	// Each section read one part of the configuration file

	// How to create your own configuration reader to build a T object
	/* 1) Declare a ConfigReader object
	 * conf::ConfigReader<T>* reader = new conf::ConfigReader<T>();
	 *
	 * 2) Add sections to the reader (documentation in xxxSection.hh)
	 * reader->addSection<Section1>();
	 * reader->addSection<Section2>();
	 *
	 * 3) Parse your file and retrieve your object
	 * T* myObject = reader->parse(configFile);
	 *
	 * 4) Do not forget to free memory when you do not need it anymore
	 * delete reader;
	 * delete myObject;
	 */
	auto* reader = new conf::ConfigReader<SimulationEnvironment>();
	reader->addSection<UnitSection>();
	reader->addSection<CellSection>();
	reader->addSection<SpheroidSection>();
	reader->addSection<MeshSection>();
	reader->addSection<ForceSection>();
	reader->addSection<SimulationSection>();

	// Parse the configuration file and start the simulation
	// SimulationEnvironment contains everything required to create a cell population
	// (documentation in simulationEnvironment.hh and simulationEnvironment.cc)
	SimulationEnvironment* simulationEnv = reader->parse(input.c_str());

	// Start the simulation to apply elastic force
	simulationEnv->startSimulation();

	// Save the generated cell population in an xml file
	std::string outputDir = std::filesystem::absolute("output").string();
	std::filesystem::create_directories(outputDir);
	std::string outputPath = outputDir + "/" + basename + ".xml";
	simulationEnv->savePopulation(outputPath.c_str());
	std::cout << "Generated : "<< outputPath << std::endl;

	// If vis flag is used, create an off file
	if (vis) {
		std::string outputVis = std::filesystem::absolute("outputVis").string();
		std::filesystem::create_directories(outputVis);
		std::string outputPathOff = outputVis + "/" + basename;
		simulationEnv->exportToVis(outputPathOff.c_str());
		std::cout << "Generated : "<< outputPathOff + ".off" << std::endl;
	}

	delete reader;
	delete simulationEnv;
}
