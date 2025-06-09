#include "Population.hh"
#include <stdexcept>

#include "cReader/zupply.hpp"
#include "EnvironmentSettings.hh"
#include "MeshFactory.hh"
#include "SpheroidalCellMesh.hh"
#include "CPOP_Loader.hh"
#include "CGAL_Utils.hh"

#include "G4UnitsTable.hh"

#include "analysis.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

namespace cpop {

Population::Population():
	_messenger(std::make_unique<PopulationMessenger>(this))
{
}

Population::~Population() {
	delete _voronoiMesh;
}

int Population::verbose_level() const {
	return _verboseLevel;
}

void Population::setVerbose_level(double verbose_level) {
	_verboseLevel = verbose_level;
}

double Population::spheroid_radius() const {
	return spheroidRadius;
}

void Population::setSpheroid_radius(double spheroidRadius) {
	Population::spheroidRadius = spheroidRadius*conversionFrmCPOPToG4;
}

Settings::Geometry::Point_3 Population::spheroid_centroid() const {
	return _spheroidCentroid;
}

void Population::setSpheroid_centroid(const Settings::Geometry::Point_3 &spheroidCentroid) {
	_spheroidCentroid = Utils::myCGAL::to_G4(spheroidCentroid);
}

unsigned int Population::number_max_facet_poly() const {
	return _numberMaxFacetPoly;
}

void Population::setNumber_max_facet_poly(unsigned int number_max_facet_poly) {
	_numberMaxFacetPoly = number_max_facet_poly;
}

double Population::delta_reffinement() const {
	return _deltaReffinement;
}

void Population::setDelta_reffinement(double delta_reffinement) {
	_deltaReffinement = delta_reffinement;
}

std::string Population::population_file() const {
  return _populationFile;
}

void Population::setPopulation_file(const std::string &population_file) {
	G4cout << "\n\n\n" << &population_file<< "\n\n\n";
	_populationFile = population_file;
}

//VictorLevrague
G4int Population::calculateNumberOfCells_InXML_File() {
  QString qstr = QString::fromStdString(_populationFile);
  QFile file(qstr);

  QString cells_flag = "CELLS";

  if(file.open(QIODevice::ReadOnly)) {
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

		// skip information until found the cells_flag
		while(!xmlReader.atEnd() &&  !xmlReader.hasError() &&	(xmlReader.name() != "CELLS"))
			xmlReader.readNext();

		xmlReader.readNext();

		while(!xmlReader.atEnd() &&  !xmlReader.hasError() &&	(xmlReader.name() != "CELLS")) {
			if(xmlReader.name() == cell_flag)
				nbCellXml+=1;
			xmlReader.readNext();
		}
  }

  nbCellXml /= 2; // Both variables (positions and nuclei of CELL tag will be counted)

  return nbCellXml;
}

void Population::loadPopulation() {
	if (number_max_facet_poly() <= 0)
		throw std::runtime_error("nbMaxFacetPerPoly should be strictly positive. Current value : " + std::to_string(number_max_facet_poly()));

	if (delta_reffinement() < 0)
		throw std::runtime_error("deltaReffinement should be positive. Current value : " + std::to_string(delta_reffinement()));

	if (population_file().empty())
		throw std::runtime_error("Population file not set. Use /cpop/detector/setPopulation file.xml to set it.");
	else {
		//zz::fs::Path path = "./" + population_file();
		zz::fs::Path path = population_file();
		//G4cout << "\n\n\n" << path.rel_path<< "\n\n\n";

		if(!path.is_file()) {
			std::string errorMsg = "Could not find population file at " + path.relative_path();
			throw std::runtime_error(errorMsg.c_str());
		}
	}

	Settings::nEnvironment::t_Environment_3* env;
	{
		CPOP_Loader loader;
		// G4cout << "\n\n\n load3DEnvironment \n\n\n" << G4endl;
		env = loader.load3DEnvironment(population_file().c_str(), true);
	}

	G4cout << "\n\n nb_cell_xml \n\n" <<calculateNumberOfCells_InXML_File() << G4endl;

	if(!env)
		throw std::runtime_error("No environment found on the given file. Is the file name/path correct ?");

	assert(env);
	assert(dynamic_cast<Settings::nEnvironment::t_SimulatedSubEnv_3*>(env->getFirstChild()));

	assert(env->getFirstChild());
	std::set<Agent*> lAgts = env->getFirstChild()->getAgents();
	// warning : this should be done before the meshing because only cell positions are modified, not their meshes
	std::set<t_SpatialableAgent_3*> spaAgts;
	// G4int int_test = 0 ;
	for(auto const& lAgt : lAgts) {
		auto* lSpaAgt = dynamic_cast<t_SpatialableAgent_3*>(lAgt);
		if(lSpaAgt) {
			// int_test += 1;
			spaAgts.insert(lSpaAgt);
			// G4cout << "\n lSpaAgt.getID()" << lSpaAgt->getID() << G4endl;
		}
	}

	//G4cout << "\n int_test" << int_test << G4endl;
	Utils::setBarycenter(spaAgts.begin(), spaAgts.end(), spheroid_centroid());

	// define a mesh
	int error;                        //< error value when create_3DMesh is executed
	_voronoiMesh = MeshFactory::getInstance()->create_3DMesh(
		&error,
		dynamic_cast<t_SimulatedSubEnv_3*>( env->getFirstChild()),
		MeshTypes::Round_Cell_Tesselation,
		number_max_facet_poly(),
		delta_reffinement()
	);
	dynamic_cast<SpheroidalCellMesh*>(_voronoiMesh)->generateMesh();

	//Call of this function allow to write geometry informations in .gdml (HACKED FOR NOW)
	//A sub call of SpheroidalCell:convertToG4Structure writes the masses of cells in a txt file
	dynamic_cast<SpheroidalCellMesh*>(_voronoiMesh)->exportToFile("",MeshOutFormats::GDML , false);

	// create the vector of cells
	{
		auto lCells = _voronoiMesh->getCellsWithShape();
		_cells.clear();
		_cells.insert(_cells.begin(), lCells.begin(), lCells.end());
	}

	// compute spheroid radius from the farthest cell
	double nearest, farthest;
	Utils::getNearestAndFarthestPoints(_spheroidCentroid, _cells.begin(), _cells.end(), nearest, farthest );
	setSpheroid_radius(farthest);

	if(verbose_level() > 0){
		printPopulationInfo();
	}

	if (writeInfoPrimariesTxt) {
		std::ofstream infos_primaries_file_to_write;
		infos_primaries_file_to_write.open(nameFilePrimaries);
	}
}

void Population::printPopulationInfo() {
	zz::fs::Path path = "./" + population_file();

	std::cout << "\t Defining geometry" << std::endl;
	std::cout << "Found "<< population_file() << " at " << path.abs_path() << std::endl;
	std::cout << "Spheroid centroid = " << spheroid_centroid() << " radius of the spheroid = " << G4BestUnit(spheroid_radius(),"Length") << std::endl;
}

void Population::defineRegion() {
	if (_internalLayerRatio < 0 || _internalLayerRatio > 1)
		throw std::runtime_error("InternalLayerRatio should be in [0;1]. Current value : " + std::to_string(_internalLayerRatio));

	if (_intermediaryLayerRatio < 0 || _intermediaryLayerRatio > 1)
		throw std::runtime_error("IntermediaryLayerRatio should be in [0;1]. Current value : " + std::to_string(_intermediaryLayerRatio));

	double intermediaryLayerRadius = spheroidRadius*_intermediaryLayerRatio;
	double internalLayerRadius = spheroidRadius*_internalLayerRatio;

	// region creation
	_regions.reserve(3);
	_regions.emplace_back("Necrosis", _spheroidCentroid, 0, internalLayerRadius, _cells.begin(), _cells.end());
	_regions.emplace_back("Intermediary", _spheroidCentroid, internalLayerRadius, intermediaryLayerRadius, _cells.begin(), _cells.end());
	_regions.emplace_back("External"    , _spheroidCentroid, intermediaryLayerRadius, spheroidRadius, _cells.begin(), _cells.end());

	for(SpheroidRegion& region : _regions) {
		auto samples = region.sample(_numberSamplingCellPerRegion);
		_sampledCells.insert(_sampledCells.end(), samples.begin(), samples.end());
	}

	if(verbose_level() > 0)
		printRegionInfo();
}

void Population::printRegionInfo() {
	double intermediaryLayerRadius = spheroidRadius*_intermediaryLayerRatio;
	double internalLayerRadius = spheroidRadius*_internalLayerRatio;

	std::cout << "\tCreating Regions" << std::endl;
	std::cout << "IntermediaryLayerRadius " << G4BestUnit(intermediaryLayerRadius,"Length") << std::endl;
	std::cout << "InternalLayerRadius " << G4BestUnit(internalLayerRadius,"Length") << std::endl;
	std::cout << "Spheroid radius " << G4BestUnit(spheroidRadius,"Length") << std::endl;
	for(SpheroidRegion& region : _regions) {
		auto samples = region.sample();
		std::cout << "Region " << region.name() << " observes " << samples.size() << " cells" << std::endl;
		if(_verboseLevel > 0) {
			for(auto sample : samples)
				std::cout << "  Cell id : " << sample->getID() <<'\n';
		}
	}
}

const std::vector<const Settings::nCell::t_Cell_3 *>& Population::sampled_cells() const {
	return _sampledCells;
}

PopulationMessenger &Population::messenger() {
	return (*_messenger);
}

double Population::internal_layer_ratio() const {
	return _internalLayerRatio;
}

double Population::intermediary_layer_ratio() const {
	return _intermediaryLayerRatio;
}

double Population::number_sampling_cell_per_region() const {
	return _numberSamplingCellPerRegion;
}

std::vector<SpheroidRegion> Population::regions() const {
	return _regions;
}

void Population::setNumber_sampling_cell_per_region(double value) {
	_numberSamplingCellPerRegion = value;
}

void Population::setIntermediary_layer_ratio(double intermediary_layer_ratio) {
	_intermediaryLayerRatio = intermediary_layer_ratio;
}

void Population::setInternal_layer_ratio(double internal_layer_ratio) {
	_internalLayerRatio = internal_layer_ratio;
}

const std::vector<const Settings::nCell::t_Cell_3 *> &Population::cells() const {
	return _cells;
}

void Population::set_Stepping_level_info_bool(int stepping_level_info_arg) {
  steppingLevelInfo = stepping_level_info_arg;
}

void Population::set_Event_level_info_bool(int event_level_info_arg) {
  eventLevelInfo = event_level_info_arg;
}

void Population::enableWritingInfoPrimariesTxt(G4String choice, G4String name_file) {
  if (choice == "yes") {
		writeInfoPrimariesTxt = true;
		nameFilePrimaries = name_file;
	} else if (choice == "no") {
		writeInfoPrimariesTxt = false;
	} else {
		std::stringstream error_msg;
    error_msg << "Chose yes or no for the choice of writing information about primaries in a txt file";
		throw std::runtime_error(error_msg.str());
	}
}

}
