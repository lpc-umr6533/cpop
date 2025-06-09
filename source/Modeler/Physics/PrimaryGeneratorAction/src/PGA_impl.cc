#include "PGA_impl.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

#include "Population.hh"
#include "Source.hh"

#include "SteppingAction.hh"

#include <vector>

#include "analysis.hh"
#include "G4Step.hh"

#include "CGAL_Utils.hh"
#include "CellSettings.hh"
#include "AgentSettings.hh"
#include "BoundingBox.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "G4RunManager.hh"

#include <math.h>
#include <random>
#include <iostream>

#include <G4UnitsTable.hh>

#include "G4PhysicalConstants.hh"

#include <cmath>

#include "G4IonTable.hh"

#define square(a)  (a)*(a)

namespace cpop {

std::atomic<int> PGA_impl::nbUniform{0};
std::atomic<int> PGA_impl::nbDistributed{0};

std::mutex PGA_impl::_generatePrimariesMutex;
std::mutex PGA_impl::_addUniformMutex;
std::mutex PGA_impl::_addDistributedMutex;
std::mutex PGA_impl::_initializeMutex;
bool PGA_impl::_isInit{false};

PGA_impl::PGA_impl(const Population &population):
	_population(&population),
	_particleGun(std::make_unique<G4ParticleGun>(1)),
	_messenger(std::make_unique<PGA_implMessenger>(this))
{
}

void PGA_impl::GeneratePrimaries(G4Event *event) {
	// Since the source may have an internal state, we protect the whole method
	std::lock_guard<std::mutex> lock(_generatePrimariesMutex);

	if(!_isChecked) {
		checkBeamOn();
		_isChecked = true;
	}

	indiceIfDiffusion = 0;
	nbEssaisDiffusion = 0;

	// (distributed_source()->organelle_weight_vector).clear();
	// particle_gun_->SetNumberOfParticles(10);

	// We do not need to check if source is nullptr because checkPrecondition() would have thrown an exception
	Source* source = selectSource();

	// Get the particle or ion from the source
	if (source->ion()) {
		_particleGun->SetParticleDefinition(source->ion());
	} else if (source->particle()) {
		_particleGun->SetParticleDefinition(source->particle());
	}

	particleEnergy = source->GetEnergy();
	g4ParticlePosition = (source->GetPosition()).front();

	//TODO : debug the (source->GetPosition()) call when multiple sources are used in one simulation
	//(source->GetPosition()) value is well attributed in DistributedSource GetPosition(), but the call is PGA_impl.cc fails.

	if (_distributedSource) {
		labeledCells = _distributedSource->labeledCells;
		_population->set_labeled_cells(labeledCells);

		if (_distributedSource->onlyOnePositionForAllParticlesOnACell == 0)
			_distributedSource->eraseFront_position_cell();

		G4String name_radionuclide = "At211"; //TODO : create a macro command with radionuclide name and associate corresponding energies

		if (name_radionuclide.compare("At211")==0) {
			halfLife = 0.516;
			ifEnergyDiffusionVector = {7.4502, 6.8912, 6.5684};
		}

		if (
			diffusionBool &&
			((distributed_source()->getOrganelle_weight()).at(0) == 1) &&
			(std::find(ifEnergyDiffusionVector.begin(), ifEnergyDiffusionVector.end(), particleEnergy) != ifEnergyDiffusionVector.end())
		) {
			double distance_after_decay = GenerateDistanceAfterDiffusion(GenerateTimeBeforeDecay());
			if (distance_after_decay>1) {
				g4ParticlePosition = GenerateNewPositionAfterDiffusion(g4ParticlePosition, distance_after_decay);
				indiceIfDiffusion=1;
			}
		}

		currentCellId = distributed_source()->getID_OfCell();
	}

	direction = source->GetMomentum();

	// if (li7_BNCT_spectra) {
	//   setPositionsDirections(name_info_primaries_file, name_method_for_info_primaries);
	// }

	if (readInputPositionFile)
		setPositions(nameInfoPrimariesFile);

	_particleGun->SetParticlePosition(g4ParticlePosition);
	// particle_gun_->SetParticlePosition(G4ThreeVector(0, 0, 0));

	// Choose an energy
	// if (li7_BNCT_spectra)
	// {energySpectraLithium7BNCT();}

	_particleGun->SetParticleEnergy(particleEnergy);

	// Generate a momentum direction
	// G4cout << "direction: " << direction << G4endl;
	_particleGun->SetParticleMomentumDirection(direction);

	source->lineNumberPositionsDirectionsFile += 1;

	if (_population->writeInfoPrimariesTxt)
		writingInfoPrimariesTxt(g4ParticlePosition, direction, particleEnergy, _population->nameFilePrimaries);

	// Generate a primary vertex
	// G4cout << "Particle position: " << G4_particle_position << G4endl;
	_particleGun->GeneratePrimaryVertex(event);
	// Update the source
	source->Update();
}


void PGA_impl::writingInfoPrimariesTxt(
	G4ThreeVector position,
	G4ThreeVector direction,
	G4double energy,
	G4String name_file
) {
	std::ofstream file(name_file, std::fstream::app);

  if (file.is_open()) {
		// file << G4BestUnit(position, "Length") << " " << direction << " " <<
		// G4BestUnit(energy, "Energy")  << "\n";
		// file.close();}
		file << G4BestUnit(position, "Length") << " " << direction << " " << currentCellId  << "\n";
    file.close();
	}
}

void PGA_impl::readInfoPrimariesTxt_Hack(int i, G4String name_file) {
	std::ifstream infile(name_file);
	std::string line;

	// read the ith line
	for (int j = 0; j < i; j++) {
		if (!getline(infile, line)) {
			// error handling if i is greater than the number of lines in the file
			std::cerr << "Error: the file does not have " << i << " lines." << std::endl;
			return;
		}
	}

	// parse the line and create the two vectors
	std::istringstream parser(line);
	double x, y, z;
	std::string cell_id;
	char delimiter_char;
	std::string unit;
	parser >> x >> y >> z >> unit >> cell_id;

	if(unit == "um") {
		vecPosition = G4ThreeVector(x * 0.001, y * 0.001, z * 0.001);
	} else {
		std::cerr << "Error: converts the values to um in the txt file. WIP: doing it in code" << std::endl;
	}
}

void PGA_impl::readInfoPrimariesTxt(int i, G4String name_file) {
	std::ifstream infile(name_file);
	std::string line;

	// read the ith line
	for (int j = 0; j < i; j++) {
		if (!std::getline(infile, line)) {
			// error handling if i is greater than the number of lines in the file
			std::cerr << "Error: the file does not have " << i << " lines." << std::endl;
			return;
		}
	}

	// parse the line and create the two vectors
	std::istringstream parser(line);
	double x, y, z;
	std::string vec_direction_str;
	char delimiter_char;
	std::string unit;
	parser >> x >> y >> z >> unit >> vec_direction_str >> energyFromTxt;
	vec_direction_str = vec_direction_str.substr(1, vec_direction_str.length() - 2);

	std::istringstream vec_direction_parser(vec_direction_str);
	double u, v, w;
	vec_direction_parser >> u >> delimiter_char >> v >> delimiter_char >> w;

	if (unit == "um") {
		vecPosition = G4ThreeVector(x * 0.001, y * 0.001, z * 0.001);
	} else {
		std::cerr << "Error: converts the values to um in the txt file. WIP: doing it in code" << std::endl;
	}

	vecDirection = G4ThreeVector(u, v, w);
}

void PGA_impl::setPositionsDirections(G4String name_file, G4String name_method) {
	Source* source = selectSource();
	readInfoPrimariesTxt(source->lineNumberPositionsDirectionsFile, name_file);

	if (name_method == "SamePositions_OppositeDirections") {
		g4ParticlePosition = vecPosition;
		direction = - vecDirection;
	} else if (name_method == "SamePositions_SameDirections") {
		g4ParticlePosition = vecPosition;
		direction = vecDirection;
	} else {
		std::cerr << "Error: this method name is not correct" << std::endl;
	}
}

void PGA_impl::setPositions(G4String name_file) {
  Source* source = selectSource();
  readInfoPrimariesTxt_Hack(source->lineNumberPositionsDirectionsFile, name_file);
  g4ParticlePosition = vecPosition;
}

void PGA_impl::energySpectraLithium7BNCT() {
	// TODO fix fp-cmp
  if (energyFromTxt == 1.78/CLHEP::MeV)
    particleEnergy = 1.01/CLHEP::MeV;
  else if (energyFromTxt == 1.47/CLHEP::MeV)
    particleEnergy = 0.84/CLHEP::MeV;
}

bool PGA_impl::HasSource() const {
	return (_uniformSource.get() != nullptr) || (_distributedSource.get() != nullptr);
}

int PGA_impl::TotalEvent() const {
	int total = 0;

	if(_uniformSource)
		total += _uniformSource->total_particle();

	if(_distributedSource)
		total += _distributedSource->total_particle();

	return total;
}

void PGA_impl::Initialize() {
	std::lock_guard<std::mutex> lock(_initializeMutex);
	if(!_isInit) {
		if (_uniformSource) _uniformSource->Initialize();
		if (_distributedSource) _distributedSource->Initialize();
		_isInit = true;
	}
}

const Settings::nCell::t_Cell_3* PGA_impl::findCell(const Point_3 &point) {
	if(!_isInitialized) {
		std::vector<const Settings::nCell::t_Cell_3*> sampled_cells = _population->sampled_cells();
		std::vector<const Settings::nAgent::t_SpatialableAgent_3*> spatialables(sampled_cells.begin(), sampled_cells.end());

		int nbCellPerNode = 2000;
		_octree = std::make_unique<Octree<OctreeNodeForSpheroidalCell>>(
			Utils::getBoundingBox(spatialables.begin(), spatialables.end()),
			&spatialables,
			nbCellPerNode);
		_isInitialized = true;
	}
	auto const* lNearestAgent = _octree->getNearestSpatialableAgent(point);

	return dynamic_cast<const Settings::nCell::t_Cell_3*>(lNearestAgent);
}

void PGA_impl::checkBeamOn() const {
	auto manager = G4RunManager::GetRunManager();
	// Number of event specified with /run/beamOn
	int total_event = manager->GetCurrentRun()->GetNumberOfEventToBeProcessed();
	// Number of event by adding the source
	int expected_event = this->TotalEvent();

	G4cout << "Nb of events to be processed: " << total_event << G4endl;
	G4cout << "Nb of expected events: " << expected_event << G4endl;

	if(total_event != expected_event) { // Throw an exception with a nice output because we are good people
		std::stringstream error_msg;
		error_msg << "The number of event using /run/beamOn is different from the sum of the sources.\n";
		error_msg << "Read : /run/beamOn " << total_event << '\n';
		error_msg << "Computed from the sources : " << expected_event << '\n';
		error_msg << "Below indicates how the expected number of event has been computed. \n";

		int totalH = 0;
		if ( _uniformSource.get() == nullptr) {
			error_msg << "  No uniform source has been added\n";
		} else {
			totalH = _uniformSource->total_particle();
			error_msg << "  Number of event from the source " << _uniformSource->source_name() << " : " << totalH << '\n';
		}

		int nbSource = 0;
		int partPerSource = 0;
		if (_distributedSource.get() == nullptr) {
			error_msg << "  No distributed source has been added'\n";
		} else {
			nbSource = _distributedSource->number_distributed();
			partPerSource = _distributedSource->number_particles_per_source();
			error_msg << "  Number of distributed sources " << _distributedSource->source_name() << " : " << nbSource << '\n';
			error_msg << "  Number of secondaries for one source : " << partPerSource << '\n';
		}

		error_msg << "Expected number of event = NB_Uniform + NB_Source * ParticlesPerSource \n";
		error_msg << expected_event << " = " << totalH << " + " << nbSource << " * " << partPerSource << '\n';
		error_msg << "Try to change your macro with : /run/beamOn " << expected_event << '\n';

		throw std::runtime_error(error_msg.str());
	}
}

UniformSource &PGA_impl::addUniformSource(const std::string &source_name) {
	std::lock_guard<std::mutex> lock(_addUniformMutex);
	if (!_uniformSource)
		_uniformSource = std::make_unique<UniformSource>(source_name, *_population);

	return *_uniformSource;
}

UniformSource *PGA_impl::uniform_source() {
	return _uniformSource.get();
}

DistributedSource &PGA_impl::addDistributedSource(const std::string &source_name) {
	std::lock_guard<std::mutex> lock(_addDistributedMutex);
	if (!_distributedSource)
		_distributedSource = std::make_unique<DistributedSource>(source_name, *_population);
	return *_distributedSource;
}

DistributedSource *PGA_impl::distributed_source() {
	return _distributedSource.get();
}

Source *PGA_impl::selectSource() const {
	if (_uniformSource && _uniformSource->HasLeft()) {
		++nbUniform;
		return _uniformSource.get();
	}

	if (_distributedSource && _distributedSource->HasLeft()) {
		++nbDistributed;
		return _distributedSource.get();
	}

	return nullptr;
}

void PGA_impl::ActivateDiffusion(G4String diffusion_string, G4double half_life_arg) {
  if (diffusion_string == "yes") {
		G4cout<< "\n yes \n" << G4endl;
    diffusionBool=true;
    halfLife=half_life_arg;
	} else if (diffusion_string == "no") {
		G4cout<< "\n no \n" << G4endl;
    diffusionBool=false;
	}
}

double PGA_impl::PDF_RadioactiveDecay(double uniform_number, double lambda) {
  return -log(1-uniform_number)/lambda;
}

double PGA_impl::GenerateTimeBeforeDecay() {
  double lambda = log(2)/halfLife;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  return (PDF_RadioactiveDecay(dis(gen), lambda));
}

double PGA_impl::GenerateDistanceAfterDiffusion(double timeBeforeDecay) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  double R1 = dis(gen);
  double R2 = dis(gen);

  double Diffusion_Coefficient = 4.3 * pow(10,2); // µm²/s

  return abs(sqrt(2 * Diffusion_Coefficient * timeBeforeDecay)*sqrt(-2*log(R1))*cos(2*CLHEP::pi*R2));
}


G4ThreeVector PGA_impl::GenerateNewPositionAfterDiffusion(G4ThreeVector previousPosition, double diffusion_distance) {
  double x1=previousPosition[0]*1000;
  double y1=previousPosition[1]*1000;
  double z1=previousPosition[2]*1000;

  double x2;
  double y2;
  double z2;

  diffusion_distance = 12.0;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis_x(x1 - diffusion_distance , x1 + diffusion_distance);

  x2 = dis_x(gen);

  std::uniform_real_distribution<> dis_y(y1 - sqrt(square(diffusion_distance) - square(x2-x1)) , y1 + sqrt(square(diffusion_distance) - square(x2-x1)));

  y2 = dis_y(gen);

  std::uniform_real_distribution<> dis_uniform(0, 1);

  double uni_try = dis_uniform(gen);

  if (uni_try<0.5)
    z2 = z1 - sqrt(square(diffusion_distance) - square(x2-x1) - square(y2-y1));

  if (uni_try>=0.5)
    z2 = z1 + sqrt(square(diffusion_distance) - square(x2-x1) - square(y2-y1));

  newG4ParticlePosition = G4ThreeVector(x2*pow(10,-3),y2*pow(10,-3),z2*pow(10,-3));

  if (isnan(newG4ParticlePosition[1]))
    GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);

  if (isnan(newG4ParticlePosition[2]))
    GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);

  Point_3 new_CGAL_particle_position = Utils::myCGAL::to_CPOP(newG4ParticlePosition);

  auto cell = findCell(new_CGAL_particle_position);

  if (cell && cell->hasIn(new_CGAL_particle_position)) {
		nbEssaisDiffusion+=1;
		GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);
	} else {
		//G4cout<< "No hasIn" << G4endl;
	}

  assert(sqrt(pow(previousPosition[0]-newG4ParticlePosition[0],2) + pow(previousPosition[1]-newG4ParticlePosition[1],2) + pow(previousPosition[2]-newG4ParticlePosition[2],2))*1000 == diffusion_distance);

  return (newG4ParticlePosition);
}

std::string PGA_impl::findOrganelle(const Settings::nCell::t_Cell_3 *cell, const Point_3 &point) {
	std::vector<Settings::nCell::t_Nucleus_3*> nuclei = cell->getNuclei();

	for(auto const& itNuclei : nuclei) {
		if(itNuclei->hasIn(point) )
			return "nucleus";
	}

	// If the point is not in the nucleus, it is in the cytoplasm
	return "cytoplasm";
}

}
