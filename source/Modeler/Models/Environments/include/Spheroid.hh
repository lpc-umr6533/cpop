#ifndef SPHEROID_HH
#define SPHEROID_HH

#include "CellPopulation.hh"
#include "CellSettings.hh"
#include "CGAL_Utils.hh"
#include "DistributionFactory.hh"
#include "EngineSettings.hh"
#include "EnvironmentSettings.hh"
#include "ForceSettings.hh"
#include "GridSettings.hh"
#include "Grid_Utils.hh"
#include "MASPlatform.hh"
#include "Mesh_Statistics.hh"
#include "Mesh3DSettings.hh"
#include "SpheresSDelimitation.hh"
#include "SimpleSpheroidalCell.hh"
#include "Writable.hh"

#include "OctreeSDS.hh"
#include "OctreeNodeSDSForSpheroidalCell.hh"

#include <iostream>
#include <fstream>
#include <utility>

using namespace Settings::Geometry;
using namespace Settings::Geometry::Mesh3D;
using namespace Settings::nCell;
using namespace Settings::nForce;
using namespace Settings::nGrid;
using namespace Settings::nEnvironment;
using namespace Settings::SDS;

/// \brief Spheroid is class defining a cell population for a given type of cell.
/// The spheroid class regroups the following entities :
///		- Environment
///		- SubEnvironment  Used for simulation and definig the spheroid spatial delimitation (from internal and external radius )
template<typename Cell_type >
class Spheroid : public CellPopulation<double, Point_3, Vector_3>, public Writable {
public:
	/// \warning cell materials are defined in CellProperties
	Spheroid(
		const CellProperties*, unsigned int, double, double, Layer* pMother = nullptr,
		std::map<LifeCycles::LifeCycle, double> pRates=std::map<LifeCycles::LifeCycle, double>(), bool pUseGrid = true, double gridCellLength=1
	);
	/// \brief construct the spheroid from a Simulated Sub environment
	Spheroid(t_Environment_3* subEnv, t_SimulatedSubEnv_3* pSpheroidSubEnv);
	~Spheroid() override;

	/// \brief generate the mesh for the cell parts of the cell population
	virtual t_Mesh_3* getMesh(int*, unsigned int, double);

	/// \brief simulate time action on cells
	virtual bool simulateTimeAction(double time, double timeStep, int nbAgtToSimulate, double movement_threshold);

	/// \brief compute and return the minimal bounding box the cell population is including in
	[[nodiscard]] BoundingBox<Point_3> getBoundingBox() const override;

	/// \brief export stats of the spheroid meshes to a R readeable format.
	[[nodiscard]] virtual bool exportMeshStats(QString name) const;

	/// \brief  export all information needed to sumarize the Writable
	void write(QXmlStreamWriter&) const override;

	/// \brief will add an elastic force to all the cell already defined
	/// \warning this will not be applied to the cell added on the future
	void addElasticForce(double, double);

	/// \brief will reset meshse of all cells
	void resetMeshes();

private:
	/// \brief called during initialization
	virtual bool init(Layer*);
	/// \brief distribute cells
	virtual bool distribute();
	/// \brief distribute spacial position from the grid
	virtual void distributeFromGrid();
	/// \brief return the adapted spatial structure to simulate time action
	[[nodiscard]] t_SpatialDataStructure_3* getSpatialDataStructure() const;
	/// \brief return the cell properties of cell composing the spheroid

private:
	const CellProperties* _cellProperties;                   /// < \brief cell properties to give to the cell present inside the spheroid
	unsigned int _nbCell;                                    /// < \brief number of cell inside the spheroid
	double _internalRadius;                                  /// < \brief internal radius of the spheroid ( minimal distance for cell from the center of the spheroid)
	double _externalRadius;                                  /// < \brief external radius of the spheroid ( maximal distance for cell from the center of the spheroid)
	std::map<LifeCycles::LifeCycle, double> _lifeCycleRates; /// < \brief the proportion of all lifeState inside the spheroid
	bool _useAGrid;                                          /// < \brief if true will generate a more regular cell spatial positionning. But will also increase density (avoid many cell conflict)
	double _gridWidth;                                       /// < \brief width of a grid cell. If setted grid cell are cubes.
	t_SimulatedSubEnv_3* _spheroidSubEnvironment;            /// < \brief the environment generated for the spheroid
};

/////////////////////////// IMPLEMENTATION /////////////////////////////////////

/// \param pCellProperties the properties to set to cells
/// \param pNbCell The number of cell to generate into the spheroid
/// \param pInternalRadius The internal boundary dimension
/// \param pExternalRadius The external boundary dimension
/// \param pMother The mother layer of this layer
/// \param pRates the proportion of all lifeState inside the spheroid
/// \param pUseGrid true if we want to use a grid to distribute cells. Will generate a more regular cell spatial positionning. But will also increase density (avoid many cell conflict)
/// \param pGridWidth the requested grid size for a cubique definition
template <typename Cell_type>
Spheroid<Cell_type>::Spheroid(
	const CellProperties* pCellProperties,
	unsigned int pNbCell,
	double pInternalRadius,
	double pExternalRadius,
	Layer* pMother,
	std::map<LifeCycles::LifeCycle, double> pRates,
	bool pUseGrid,
	double pGridWidth
):
	_cellProperties(pCellProperties),
	_nbCell(pNbCell),
	_internalRadius(pInternalRadius),
	_externalRadius(pExternalRadius),
	_lifeCycleRates(std::move(pRates)),
	_useAGrid(pUseGrid),
	_gridWidth(pGridWidth)
{
	G4cout << "init spheroid" << G4endl;
	if(!init(pMother)) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Failed initialization", "Spheroid");
		exit(0);
	}

	if(!distribute()) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "Failed distribution", "Spheroid");
		exit(0);
	}

	// if we use a grid redistribute position from the grid
	if(_useAGrid)
		distributeFromGrid();
}

/// \param pEnv 			The top level entity containing a Simulated Sub env ( the simulated sub env should contains the cells)
/// \param pSpheroidSubEnv 	The simulated sub environment containing the spheroid
template <typename Cell_type>
Spheroid<Cell_type>::Spheroid(t_Environment_3* pEnv, t_SimulatedSubEnv_3* pSpheroidSubEnv) {
	assert(pEnv);
	assert(pSpheroidSubEnv);
	assert(pEnv->contains(pSpheroidSubEnv));

	_spheroidSubEnvironment = pSpheroidSubEnv;
	if(!dynamic_cast<SpheresSDelimitation*>(pSpheroidSubEnv->getSpatialDelimitation())) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::FATAL_ERROR_MES, "The Spatial delimitation of the given simulated sub environment is not a SpheresSDelimitation. Failed creation", "Spheroid");
		exit(0);
	}

	// set cells composing the cell population
	std::set<t_Cell_3*> lNewCells;
	std::set<Agent*> lAgts = static_cast<Layer*>(pSpheroidSubEnv)->getAgents();
	for(auto const& lAgt : lAgts) {
		if(dynamic_cast<t_Cell_3*>(lAgt))
			lNewCells.insert(dynamic_cast<t_Cell_3*>(lAgt));
	}
	CellPopulation<double, Point_3, Vector_3>::setCells(lNewCells);
}

template <typename Cell_type>
Spheroid<Cell_type>::~Spheroid() {
	delete _spheroidSubEnvironment;
}

/// \param pMother the Layer mother of this layer
/// \return true if init is a success
template <typename Cell_type>
bool Spheroid<Cell_type>::init(Layer* pMother) {
	t_Environment_3* lEnv;

	// create environment if needed
	if(!pMother && (!dynamic_cast<t_Environment_3*>(pMother))) {
		// create the top level entity
		lEnv = new t_Environment_3("main Environment");
	} else {
		lEnv = dynamic_cast<t_Environment_3*>(pMother);
	}

	// create sub environment
	auto* subEnvSD = new SpheresSDelimitation(_internalRadius, _externalRadius);
	_spheroidSubEnvironment = new t_SimulatedSubEnv_3(lEnv, "Spheroid", static_cast<t_SpatialDelimitation_3*>(subEnvSD));

	return true;
}

template <typename Cell_type>
void Spheroid<Cell_type>::distributeFromGrid() {
	t_Grid_3* grid = Utils::nGrid::getOptimalGrid(_cellProperties, _spheroidSubEnvironment, _gridWidth);
	assert(_spheroidSubEnvironment);
	assert(_spheroidSubEnvironment->getSpatialDelimitation());
	grid->applySpatialDelimitation(_spheroidSubEnvironment->getSpatialDelimitation());

	grid->distributePosition(cells.begin(), cells.end(), GEP_CENTER);
	delete grid;
}

template <typename Cell_type>
bool Spheroid<Cell_type>::distribute() {
	assert(_cellProperties);

	// check lifeRates
	{
		// if unvalid lifeStates : create new one
		if(_lifeCycleRates.size() == 0 )
			_lifeCycleRates = Utils::generateUniformLifeCycle();

		// make sure life cycle are set percentage
		Utils::myCGAL::normalize(&_lifeCycleRates);
	}

	ADistribution<double, Point_3, Vector_3>* distribution = DistributionFactory::getInstance()->getDistribution<double, Point_3, Vector_3>(Distribution::RANDOM);
	distribution->distribute(_spheroidSubEnvironment, _cellProperties, _nbCell, _lifeCycleRates);
	delete distribution;

	// insert all new cells distributed to the CellPopulation
	std::set<Agent*> agents = static_cast<Layer*>(_spheroidSubEnvironment)->getAgents();
	std::set<t_Cell_3*> lCells;
	for(auto const& agent : agents)
	{
		assert(dynamic_cast<t_Cell_3*>(agent));
		if(dynamic_cast<t_Cell_3*>(agent))
			CellPopulation<double, Point_3, Vector_3>::cells.insert(dynamic_cast<t_Cell_3*>(agent));
	}

	return true;
}

// simulateTimeAction
/// \param pDuration The duration of the total simulation
/// \param pTimeStep The duration for a step of the simulation
/// \param pNbAgtToSimulate The number of agent to simulate at each step (randomly picked)
/// \param pMovement_threshold The max movement distance possible during one step
template <typename Cell_type>
bool Spheroid<Cell_type>::simulateTimeAction(double pDuration, double pTimeStep, int pNbAgtToSimulate, double pMovement_threshold) {
	/// create platform
	MASPlatform platform;

	/// set up platform
	platform.setLayerToSimulate(_spheroidSubEnvironment);
	platform.setStepDuration(pTimeStep);
	platform.setDuration(pDuration);
	platform.setDisplacementThreshold(pMovement_threshold);		// limit to 0.5µm hte displacement per step.
	platform.limiteNbAgentToSimulate(pNbAgtToSimulate);

	/// set SDS needed to simulate
	/// add the SDS to the layer
	auto* lSDS = getSpatialDataStructure();
	if(!lSDS) {
		InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Fail to get the SDS, simulate Time action exiting", "Spheroid");
		exit(0);
	}

	_spheroidSubEnvironment->addSpatialDataStructure( lSDS );
	/// start simulation
	return (platform.startSimulation() == 0);
}

// add spatial data structure
template <typename Cell_type>
t_SpatialDataStructure_3* Spheroid<Cell_type>::getSpatialDataStructure() const {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't create an adapted spatial data structure for this kind of cell", "Spheroid");
	return nullptr;
}

/// \brief return the adapted spatial structure to simulate time action
/// \return the spatial data structured to use
template<>
t_SpatialDataStructure_3* Spheroid<SimpleSpheroidalCell>::getSpatialDataStructure() const;

template <typename Cell_type>
t_Mesh_3* Spheroid<Cell_type>::getMesh(int* error, unsigned int, double) {
	*error = -1;
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't generate mesh for this type of cell", "Spheroid");
	return nullptr;
}

/// \brief generate the generated mesh
/// \return the created mesh
template<>
t_Mesh_3* Spheroid<SimpleSpheroidalCell>::getMesh(int*, unsigned int, double);

template <typename Cell_type>
BoundingBox<Point_3> Spheroid<Cell_type>::getBoundingBox() const {
	InformationSystemManager::getInstance()->Message(InformationSystemManager::CANT_PROCESS_MES, "Can't generate bounding box for this type of cell", "Spheroid");
	return {};
}

/// \brief return the bounding box containing the spheroid
template<>
BoundingBox<Point_3> Spheroid<SimpleSpheroidalCell>::getBoundingBox() const;

/// \param pName the common name of files to export statistics
/// \return true if succeded
template <typename Cell_type>
bool Spheroid<Cell_type>::exportMeshStats(QString pName) const {
	// todo : get only cells with a mesh
	std::set<t_Cell_3*> lTmpCells = CellPopulation<double, Point_3, Vector_3>::getCells();
	std::vector<const t_Cell_3*> lCells(lTmpCells.begin(), lTmpCells.end());
	// create stats files
	QString nucleiPathName = "Nuclei_" + pName + ".cpop.stats";
	QString cellPathName = "Cell_" + pName + ".cpop.stats";

	Statistics::generateMeshStats(
		lCells,
		MeshOutFormats::GEANT_4,
		new std::ofstream(cellPathName.toStdString().c_str()),
		new std::ofstream(nucleiPathName.toStdString().c_str())
	);

	return true;
}

/// \param pWriter QXmlStreamWriter to adress information
template <typename Cell_type>
void Spheroid<Cell_type>::write(QXmlStreamWriter& pWriter) const {
	assert(_spheroidSubEnvironment);
	assert(_spheroidSubEnvironment->getParent());
	// spheroidSubEnvironment and is parent if not one given is created in construction and deleted by the destructor, so must always exists during the call of write
	_spheroidSubEnvironment->getParent()->write(pWriter);
}

/// \param pRigidity 			the regidity to set to the elastic force
/// \param pRatioToStableLength the ratio to stable length to set to the elastic force. Used to compute length at rest
template <typename Cell_type>
void Spheroid<Cell_type>::addElasticForce(double pRigidity, double pRatioToStableLength) {
	assert(_spheroidSubEnvironment);
	auto const& lCells = CellPopulation<double, Point_3, Vector_3>::getCells();
	for(auto const& lCell : lCells)
		lCell->addForce(new t_ElasticForce_3(const_cast<t_Cell_3*>(lCell), pRigidity, pRatioToStableLength));
}

template<typename Cell_type>
void Spheroid<Cell_type>::resetMeshes() {
	auto const& lCells = CellPopulation<double, Point_3, Vector_3>::getCells();
	for(auto lCell : lCells)
		lCell->resetMesh();
}

#endif
