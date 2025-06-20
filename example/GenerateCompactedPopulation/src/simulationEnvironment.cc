#include "simulationEnvironment.hh"

#include "SpheroidalCellMesh.hh"
#include "MaterialManager.hh"

#include <G4String.hh>
#include <G4UnitsTable.hh>
#include <cmath>

#include "IDManager.hh"
#include <filesystem>

SimulationEnvironment::SimulationEnvironment() {
	// metric system
	metricSystem = 1.0;

	// cell properties
	cellProperties = nullptr;

	// environments
	env = nullptr;
	simulatedEnv = nullptr;

	// mesh properties
	numberOfFacetPerCell = 50;

	// simulation
	platform = nullptr;
}

SimulationEnvironment::~SimulationEnvironment() {
	if (cellProperties) delete cellProperties;
	if (env)            delete env;
	if (platform)       delete platform;
}

void SimulationEnvironment::setMetricSystem(const std::string& metric) {
	std::string input = metric;
	// transforms the input string to uppercase to be case insensitive
	transform(input.begin(), input.end(), input.begin(), ::toupper);

	if (metric == "CENTIMETER") {
		metricSystem = UnitSystemManager::getInstance()->getMetricUnit(UnitSystemManager::Centimeter);
	} else if (metric == "MILLIMETER") {
		metricSystem = UnitSystemManager::getInstance()->getMetricUnit(UnitSystemManager::Millimeter);
	} else if (metric == "MICROMETER") {
		metricSystem = UnitSystemManager::getInstance()->getMetricUnit(UnitSystemManager::Micrometer);
	} else if (metric == "NANOMETER") {
		metricSystem = UnitSystemManager::getInstance()->getMetricUnit(UnitSystemManager::Nanometer);
	}
}

G4Material* SimulationEnvironment::parseMaterial(const char* material) {
	G4Material* mat = nullptr;
	if(!strcmp(material, "G4_WATER"))
		mat = MaterialManager::getInstance()->getMaterial(material);
	else
		mat = MaterialManager::getInstance()->getDefaultMaterial();

	return mat;
}

void SimulationEnvironment::setCellProperties(
	double minRadiusNucleus, double maxRadiusNucleus,
	double minRadiusMembrane, double maxRadiusMembrane,
	const std::string& cytoplasmMaterials,
	const std::string& nucleusMaterials
) {
	cellProperties = new RoundCellProperties();

	// cells weights range (not used for the simulation but needed to call automaticFill)
	double weightSystem = UnitSystemManager::getInstance()->getWeightUnit(UnitSystemManager::Nanogram);
	CellVariableAttribute<double> weights(1.0*weightSystem, 1.0*weightSystem);
	// membrane radius range
	CellVariableAttribute<double> membraneRadius(minRadiusMembrane*metricSystem, maxRadiusMembrane*metricSystem);
	maximum_radius_cell = maxRadiusMembrane;
	// nucleus radius range
	CellVariableAttribute<double> nucleusRadius(minRadiusNucleus*metricSystem, maxRadiusNucleus*metricSystem);
	// cytoplasm material
	G4Material* cytoMat =  		parseMaterial(cytoplasmMaterials.c_str());
	// nucleoplasm radius
	G4Material* nucleusMat =  	parseMaterial(nucleusMaterials.c_str());
	// fill with those parameters for all life cycle.
	cellProperties->automaticFill(membraneRadius, weights, nucleusRadius, BARYCENTER, cytoMat, nucleusMat);
}

void SimulationEnvironment::setSpheroidProperties(double internalRadius, double externalRadius, double required_compaction, double precision_compaction) {
	// setup the main environment
	//env = new t_Environment_3("main Environment");
	Point_3 center(0., 0., 0.);
	// tell where the cells should be created (here in a spheroid)
	auto* subEnvSD = new SpheresSDelimitation(internalRadius*metricSystem, externalRadius*metricSystem, center);

	CLHEP::MTwistEngine defaultEngine(1234567);
	RandomEngineManager::getInstance()->setEngine(&defaultEngine);

	std::map<LifeCycles::LifeCycle, double> rates = Utils::generateUniformLifeCycle();

	double precision = precision_compaction;
	int nbCell_min = 0;
	int nbCell_max = 100000;
	int nbCell_mid;

	const int max_iterations = 50;
	int iteration = 0;

	while (fabs(computed_compaction - required_compaction) > precision && iteration < max_iterations) {
		std::string home_path = std::filesystem::current_path().string();
		std::string output_txt_folder = home_path + "/OutputTxt";

		if (!std::filesystem::exists(output_txt_folder)) {
			if (!std::filesystem::create_directory(output_txt_folder)) {
				std::cerr << "Error: Failed to create directory: " << output_txt_folder << "\n";
			}
		}

		std::ofstream id_cell_file(output_txt_folder + "/IDCell.txt", std::ios::trunc);

		nbCell_mid = (nbCell_min + nbCell_max) / 2;
		// New distribution
		env = new t_Environment_3("main Environment");
		simulatedEnv = new t_SimulatedSubEnv_3(env, "MySimulatedSubEnv", static_cast<t_SpatialDelimitation_3*>(subEnvSD));
		ADistribution<double, Point_3, Vector_3>* distribution = DistributionFactory::getInstance()->getDistribution<double, Point_3, Vector_3>(Distribution::RANDOM);
		distribution->distribute(simulatedEnv, cellProperties, nbCell_mid, rates);
		delete distribution;
		computed_compaction = ComputeCompaction(simulatedEnv, externalRadius);
		G4cout << "Iteration " << iteration + 1 << ", Compaction: " << computed_compaction << G4endl;
		// Bisection
		if (computed_compaction > required_compaction + precision) {
			nbCell_max = nbCell_mid;
		} else if (computed_compaction < required_compaction - precision) {
			nbCell_min = nbCell_mid;
		}
		iteration++;

		IDManager::getInstance()->reset();
		//IDManager::getInstance()->destroyInstance();
	}

	G4cout << "\n\n\nFinal compaction: " << computed_compaction * 100 << " %\n\n\n" << G4endl;

	if (iteration >= max_iterations) {
		G4cout << "Warning: Maximum iterations reached. Solution may not be optimal." << G4endl;
	}
}

double SimulationEnvironment::ComputeCompaction(
	const t_Sub_Env_3* simulatedEnv,
	double externalRadius
) {
	int error;
	t_Mesh_3* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, simulatedEnv, MeshTypes::Round_Cell_Tesselation, numberOfFacetPerCell);
	dynamic_cast<SpheroidalCellMesh*>(voronoiMesh)->generateMesh();

	G4PVPlacement* physiWorld = dynamic_cast<SpheroidalCellMesh*>(voronoiMesh)->convertToG4World(false);
	G4LogicalVolume* logicWorld = physiWorld->GetLogicalVolume();

	double mass_cells = 0;
	ComputeMassOfCells(logicWorld, mass_cells);

	double volume_spheroid = (4./3.) * M_PI * pow(externalRadius, 3); //µm3
	double volumic_mass = pow(10, -15); //kg/µm3
	double mass_spheroid = volume_spheroid * volumic_mass;
	double cell_packing = (mass_cells/CLHEP::kg) / (mass_spheroid);
	return cell_packing;
}

void SimulationEnvironment::PrintMassOfCells(G4LogicalVolume* logicalVolume) {
	if (G4StrUtil::contains(logicalVolume->GetName(), "LV_cell")) {
		G4cout << "logicalVolume mass: " << G4BestUnit(logicalVolume->GetMass(), "Mass") << " of " << logicalVolume->GetName() << G4endl;
	}

	for (size_t i = 0; i < logicalVolume->GetNoDaughters(); ++i) {
		G4VPhysicalVolume* physVol = logicalVolume->GetDaughter(i);
		PrintMassOfCells(physVol->GetLogicalVolume());
	}
}

void SimulationEnvironment::ComputeMassOfCells(G4LogicalVolume* logicalVolume, double& mass_cells) {
	if (G4StrUtil::contains(logicalVolume->GetName(), "LV_cell")) {
		mass_cells += logicalVolume->GetMass();
		// G4cout << "logicalVolume mass: " << G4BestUnit(logicalVolume->GetMass(), "Mass") << " of " << logicalVolume->GetName() << G4endl;
	}

	for (size_t i = 0; i < logicalVolume->GetNoDaughters(); ++i) {
		G4VPhysicalVolume* physVol = logicalVolume->GetDaughter(i);
		ComputeMassOfCells(physVol->GetLogicalVolume(), mass_cells);
	}
}

void SimulationEnvironment::setMeshProperties(int nOfFacetPerCell) {
	numberOfFacetPerCell = nOfFacetPerCell;
}

void SimulationEnvironment::setForceProperties(double ratioToStableLength, double rigidity) {
	int error;
	// get the generated cells
	t_Mesh_3* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, simulatedEnv, MeshTypes::Round_Cell_Tesselation, numberOfFacetPerCell);
	std::set<t_Cell_3*> lCells = voronoiMesh->getCells();
	delete voronoiMesh;

	// apply elastic forces to each cells
	for(auto const& lCell : lCells) {
		auto* elasForce = new t_ElasticForce_3( lCell, rigidity, ratioToStableLength);
		lCell->addForce(elasForce);
	}
}

void SimulationEnvironment::setSimulationProperties(
	double duration, int numberOfAgentToExecute,
	double displacementThreshold,
	double stepDuration
) {
	// 4. execute agent define a MASPlatform
	/// 4.1 create platform
	platform = new MASPlatform();

	/// 4.2 set up platform
	platform->setLayerToSimulate(simulatedEnv);
	platform->setStepDuration(stepDuration);
	platform->setDuration(duration);
	platform->setDisplacementThreshold(displacementThreshold);
	platform->limiteNbAgentToSimulate(numberOfAgentToExecute);
}

void SimulationEnvironment::startSimulation() {
	/// 4.3 set the adapted spatial data structure permitting agent to know their neighbors)
	simulatedEnv->addSpatialDataStructure(new Delaunay_3D_SDS(" my spatial data structure"));

	platform->startSimulation();
}

void SimulationEnvironment::savePopulation(std::string const& filename) {
	IO::CPOP::save(static_cast<Writable*>(env), filename.c_str());
}

void SimulationEnvironment::exportToVis(std::string const& filename, bool divided) {
	int error;
	t_Mesh_3* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, simulatedEnv, MeshTypes::Round_Cell_Tesselation, numberOfFacetPerCell);
	voronoiMesh->exportToFile(filename, MeshOutFormats::OFF, divided);
	delete voronoiMesh;
}
