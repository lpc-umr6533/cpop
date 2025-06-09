#ifndef SIMULATION__ENVIRONMENT__H
#define SIMULATION__ENVIRONMENT__H

#include <string>

#include <CellFactory.hh>          // needed to call the mesh factory, creating the mesh
#include <Cell_Utils.hh>           // used for the getNearestAndFarthestPoints function
#include <DistributionFactory.hh>  // used to distribute cell inside the sub environment
#include <ElasticForce.hh>         // The type of force we want to apply
#include <MASPlatform.hh>          // THe platform used to manage agent ( cell ) execution
#include <File_CPOP_Data.hh>       // CPOP tools for saving files
#include <MeshFactory.hh>          // used to get the reuested mesh
#include <RandomEngineManager.hh>  // manager used to generate Random numbers
#include <SimpleSpheroidalCell.hh> // the basic cell we want to use
#include <SpheresSDelimitation.hh> // the spatial delimitation we are using
#include <UnitSystemManager.hh>    // used to have the requested unitn

#include <CLHEP/Random/MTwistEngine.h>

using namespace Settings::nCell;
using namespace Settings::nEnvironment;

using t_ElasticForce_3 = ElasticForce<double, Point_3, Vector_3>;

class SimulationEnvironment {
	// Metric  system
	double metricSystem;
	// Cell properties
	RoundCellProperties* cellProperties;
	double maximum_radius_cell;
	// Spheroid properties
	t_Environment_3* env;
	t_SimulatedSubEnv_3* simulatedEnv;
	// Mesh properties
	int numberOfFacetPerCell;
	// Simulation properties
	MASPlatform* platform;

	G4Material* parseMaterial(const char* material);

	public:
	SimulationEnvironment();
	~SimulationEnvironment();

	double computed_compaction = 0.;

	// Setter used by the xxxSection class
	void setMetricSystem(const std::string& metric);
	void setCellProperties(
		double minRadiusNucleus, double maxRadiusNucleus,
		double minRadiusMembrane, double maxRadiusMembrane,
		const std::string& cytoplasmMaterials,
		const std::string& nucleusMaterials
	);
	void setSpheroidProperties(double internalRadius, double externalRadius, double compaction, double precision);
	void setMeshProperties(int nOfFacetPerCell);
	void setForceProperties(double ratioToStableLength, double rigidity);
	void setSimulationProperties(
		double duration, int numberOfAgentToExecute,
		double displacementThreshold,
		double stepDuration
	);

	void PrintMassOfCells(G4LogicalVolume* logicalVolume);
	void ComputeMassOfCells(G4LogicalVolume* logicalVolume, double& mass_cells);
	double ComputeCompaction(const t_Sub_Env_3* simulatedEnv, double externalRadius);

	// start the simulation
	void startSimulation();

	// save the population
	void savePopulation(std::string const& filename);

	// export to off format to visualise the population
	void exportToVis(std::string const& filename);
};

#endif
