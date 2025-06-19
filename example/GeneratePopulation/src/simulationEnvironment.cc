#include "simulationEnvironment.hh"
#include "Grid_Utils.hh"
#include "Delaunay_3D_SDS.hh"

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
	delete cellProperties;
	delete env;
	delete platform;
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

G4Material* SimulationEnvironment::parseMaterial(std::string const& material) {
	G4Material* mat = nullptr;
	if(material == "G4_WATER")
		mat = MaterialManager::getInstance()->getMaterial(QString::fromStdString(material));
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
	cellProperties = new RoundCellProperties;

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

void SimulationEnvironment::setSpheroidProperties(double internalRadius, double externalRadius, int nbCell, bool is_distributed_in_grid) {
	// setup the main environment
	env = new t_Environment_3("main Environment");
	Point_3 center(0., 0., 0.);
	// tell where the cells should be created (here in a spheroid)
	auto* subEnvSD = new SpheresSDelimitation(internalRadius*metricSystem, externalRadius*metricSystem, center);
	// environment to simulate
	simulatedEnv = new t_SimulatedSubEnv_3(env, "MySimulatedSubEnv", static_cast<t_SpatialDelimitation_3*>(subEnvSD));

	// generate cells
	CLHEP::MTwistEngine defaultEngine(1234567);
	RandomEngineManager::getInstance()->setEngine(&defaultEngine);

	std::map<LifeCycles::LifeCycle, double> rates = Utils::generateUniformLifeCycle();

	/// 3.1 get the distribution
	ADistribution<double, Point_3, Vector_3>* distribution = DistributionFactory::getInstance()->getDistribution<double, Point_3, Vector_3>(Distribution::RANDOM);
	/// 3.2 distribute
	distribution->distribute(simulatedEnv, cellProperties, nbCell, rates);
	delete distribution;

	/// 3.3 optional: redistribute in grid
	if (is_distributed_in_grid) {
		auto* grid = Utils::nGrid::getOptimalGrid(cellProperties, simulatedEnv, 2*maximum_radius_cell * metricSystem);
		assert(simulatedEnv);
		assert(simulatedEnv->getSpatialDelimitation());
		grid->applySpatialDelimitation(simulatedEnv->getSpatialDelimitation());
		std::set<Agent*> agents = static_cast<Layer*>(simulatedEnv)->getAgents();
		grid->distributePosition(agents.begin(), agents.end(), GEP_CENTER);
		delete grid;
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
	platform = new MASPlatform;

	/// 4.2 set up platform
	platform->setLayerToSimulate(		simulatedEnv);
	platform->setStepDuration(			stepDuration);
	platform->setDuration(				duration);
	platform->setDisplacementThreshold(	displacementThreshold);
	platform->limiteNbAgentToSimulate(	numberOfAgentToExecute);

}

void SimulationEnvironment::startSimulation() {
	/// 4.3 set the adapted spatial data structure permitting agent to know their neighbors)
	simulatedEnv->addSpatialDataStructure(new Delaunay_3D_SDS("my spatial data structure"));

	platform->startSimulation();
}

void SimulationEnvironment::savePopulation(std::string const& filename) {
	IO::CPOP::save(static_cast<Writable*>(env), QString::fromStdString(filename));
}

void SimulationEnvironment::exportToVis(std::string const& filename, bool divided) {
	int error;
	t_Mesh_3* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, simulatedEnv, MeshTypes::Round_Cell_Tesselation, numberOfFacetPerCell);
	voronoiMesh->exportToFile(QString::fromStdString(filename), MeshOutFormats::OFF, divided);
	delete voronoiMesh;
}

void SimulationEnvironment::exportToSTL(std::string const& filename, bool divided) {
	int error;
	t_Mesh_3* voronoiMesh = MeshFactory::getInstance()->create_3DMesh(&error, simulatedEnv, MeshTypes::Round_Cell_Tesselation, numberOfFacetPerCell);
	voronoiMesh->exportToFile(QString::fromStdString(filename), MeshOutFormats::STL, divided);
	delete voronoiMesh;
}
