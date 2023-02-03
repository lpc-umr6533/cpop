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

#define square(a)  (a)*(a)

namespace cpop {

std::atomic<int> PGA_impl::nbHomogeneous{0};
std::atomic<int> PGA_impl::nbNanoparticle{0};

std::mutex PGA_impl::generate_primaries_mutex_;
std::mutex PGA_impl::add_homogeneous_mutex_;
std::mutex PGA_impl::add_nanoparticle_mutex_;
std::mutex PGA_impl::initialize_mutex_;
bool PGA_impl::is_init_{false};

PGA_impl::PGA_impl(const Population &population)
    :population_(&population),
      particle_gun_(std::make_unique<G4ParticleGun>(1)),
      messenger_(std::make_unique<PGA_implMessenger>(this))
{

}

void PGA_impl::GeneratePrimaries(G4Event *event)
{
    // Since the source may have an internal state, we protect the whole method
    std::lock_guard<std::mutex> lock(generate_primaries_mutex_);

    if(!is_checked) {
        checkBeamOn();
        is_checked = true;
    }

    indice_if_diffusion=0;
    nb_essais_diffusion=0;

    // (nanoparticle_source()->organelle_weight_vector).clear();


    // We do not need to check if source is nullptr because checkPrecondition() would have thrown an exception
    Source* source = selectSource();
    // Get the particle from the source
    particle_gun_->SetParticleDefinition(source->particle());
    // Generate an energy
    G4double particleEnergy = source->GetEnergy();
    particle_gun_->SetParticleEnergy(particleEnergy);
    // Generate a position
    G4ThreeVector G4_particle_position = (source->GetPosition()).front();

    //TODO : debug the (source->GetPosition()) call when multiple sources are used in one simulation
    //(source->GetPosition()) value is well attributed in NanoparticleSource GetPosition(), but the call is PGA_impl.cc fails.

    if (nanoparticle_source_)
    {

    if (nanoparticle_source_->only_one_position_for_all_particles_on_a_cell == 0)
      {nanoparticle_source_->eraseFront_position_cell();}

    G4String name_radionuclide = "At211"; //TODO : create a macro command with radionuclide name and associate corresponding half life and energies

    if (name_radionuclide.compare("At211")==0)
      { half_life = 0.516;
      if_Energy_diffusion_vector = {7.4502, 6.8912, 6.5684};}

    if (diffusion_bool && ((nanoparticle_source()->getOrganelle_weight()).at(0) == 1) && (std::find(if_Energy_diffusion_vector.begin(), if_Energy_diffusion_vector.end(), particleEnergy) != if_Energy_diffusion_vector.end()))
    //if (diffusion_bool && ((nanoparticle_source()->getOrganelle_weight()).at(0) == 1))
    {
      // G4cout << "\n Diffusion activated \n" << G4endl;
      double distance_after_decay = GenerateDistanceAfterDiffusion(GenerateTimeBeforeDecay());
      // G4cout << "\n Distance after diffusion : " << distance_after_decay << G4endl;
      if (distance_after_decay>1)
      {G4_particle_position = GenerateNewPositionAfterDiffusion(G4_particle_position, distance_after_decay);
       indice_if_diffusion=1;}
    }

    current_cell_id = nanoparticle_source()->getID_OfCell();

   }

    particle_gun_->SetParticlePosition(G4_particle_position);

    // Generate a momentum direction
    particle_gun_->SetParticleMomentumDirection(source->GetMomentum());
    // Generate a primary vertex
    particle_gun_->GeneratePrimaryVertex(event);
    // Update the source
    source->Update();
}

bool PGA_impl::HasSource() const
{
    return (homogeneous_source_.get() != nullptr) || (nanoparticle_source_.get() != nullptr);
}

int PGA_impl::TotalEvent() const
{
    int total = 0;

    if(homogeneous_source_)
        total += homogeneous_source_->total_particle();

    if(nanoparticle_source_)
        total += nanoparticle_source_->total_particle();

    return total;
}

void PGA_impl::Initialize()
{
    std::lock_guard<std::mutex> lock(initialize_mutex_);
    if(!is_init_) {
        if (homogeneous_source_) homogeneous_source_->Initialize();
        if (nanoparticle_source_) nanoparticle_source_->Initialize();
        is_init_ = true;
    }
}

const Settings::nCell::t_Cell_3* PGA_impl::findCell(const Point_3 &point)
{
    if(!is_initialized_) {
        std::vector<const Settings::nCell::t_Cell_3*> sampled_cells = population_->sampled_cells();
        std::vector<const Settings::nAgent::t_SpatialableAgent_3*> spatialables(sampled_cells.begin(), sampled_cells.end());

        int nbCellPerNode = 2000;
        octree_ = std::make_unique<Octree<OctreeNodeForSpheroidalCell>>(
                                                                      Utils::getBoundingBox(spatialables.begin(), spatialables.end()),
                                                                      &spatialables,
                                                                      nbCellPerNode);
        is_initialized_ = true;
    }
    const t_SpatialableAgent_3* lNearestAgent = octree_->getNearestSpatialableAgent(point);

    return dynamic_cast<const Settings::nCell::t_Cell_3*>(lNearestAgent);
}

void PGA_impl::checkBeamOn() const
{
    auto manager = G4RunManager::GetRunManager();
    // Number of event specified with /run/beamOn
    int total_event = manager->GetCurrentRun()->GetNumberOfEventToBeProcessed();
    // Number of event by adding the source
    int expected_event = this->TotalEvent();

    if(total_event != expected_event) { // Throw an exception with a nice output because we are good people
        std::stringstream error_msg;
        error_msg << "The number of event using /run/beamOn is different from the sum of the sources.\n";
        error_msg << "Read : /run/beamOn " << total_event << '\n';
        error_msg << "Computed from the sources : " << expected_event << '\n';
        error_msg << "Below indicates how the expected number of event has been computed. \n";

        int totalH = 0;
        if ( homogeneous_source_.get() == nullptr) {
            error_msg << "  No homogeneous source has been added\n";
        } else {
            totalH = homogeneous_source_->total_particle();
            error_msg << "  Number of event from the source " << homogeneous_source_->source_name() << " : " << totalH << '\n';
        }

        int nbNano = 0;
        int secondPerNano = 0;
        if ( nanoparticle_source_.get() == nullptr) {
            error_msg << "  No nanoparticle source has been added'\n";
        } else {
            nbNano = nanoparticle_source_->number_nanoparticle();
            secondPerNano = nanoparticle_source_->number_secondary_per_nano();
            error_msg << "  Number of nanoparticle from the source " << nanoparticle_source_->source_name() << " : " << nbNano << '\n';
            error_msg << "  Number of secondaries for one nanoparticle : " << secondPerNano << '\n';
        }

        error_msg << "Expected number of event = NB_Homogeneous + NB_Nanoparticle * Secondaries_Per_Nano \n";
        error_msg << expected_event << " = " << totalH << " + " << nbNano << " * " << secondPerNano << '\n';
        error_msg << "Try to change your macro with : /run/beamOn " << expected_event << '\n';

        throw std::runtime_error(error_msg.str());
    }
}

HomogeneousSource &PGA_impl::addHomogeneousSource(const std::string &source_name)
{
    std::lock_guard<std::mutex> lock(add_homogeneous_mutex_);
    if (!homogeneous_source_)
        homogeneous_source_ = std::make_unique<HomogeneousSource>(source_name, *population_);

    return *homogeneous_source_;
}

HomogeneousSource *PGA_impl::homogeneous_source()
{
    return homogeneous_source_.get();
}

NanoparticleSource &PGA_impl::addNanoparticleSource(const string &source_name)
{
    std::lock_guard<std::mutex> lock(add_nanoparticle_mutex_);
    if (!nanoparticle_source_)
        nanoparticle_source_ = std::make_unique<NanoparticleSource>(source_name, *population_);
    return *nanoparticle_source_;
}

NanoparticleSource *PGA_impl::nanoparticle_source()
{
    return nanoparticle_source_.get();
}

Source *PGA_impl::selectSource() const
{
    if (homogeneous_source_ && homogeneous_source_->HasLeft()) {
        ++nbHomogeneous;
        return homogeneous_source_.get();
    }

    if (nanoparticle_source_ && nanoparticle_source_->HasLeft()) {
        ++nbNanoparticle;
        return nanoparticle_source_.get();
    }

    return nullptr;
}

void PGA_impl::ActivateDiffusion(G4String diffusion_string)
{
  if (diffusion_string.compare("yes")==0)
  { G4cout<< "\n yes \n" << G4endl;
    diffusion_bool=true;}
  else if (diffusion_string.compare("no")==0)
  {G4cout<< "\n no \n" << G4endl;
    diffusion_bool=false;}
}

double PGA_impl::PDF_RadioactiveDecay(double uniform_number, double lambda)
{
  return -log(1-uniform_number)/lambda;
}

double PGA_impl::GenerateTimeBeforeDecay()
{

  double lambda = log(2)/half_life;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  // G4cout << "timeBeforedecay = " << (PDF_RadioactiveDecay(dis(gen),lambda)) << G4endl;

  return (PDF_RadioactiveDecay(dis(gen),lambda));

}

double PGA_impl::GenerateDistanceAfterDiffusion(double timeBeforeDecay)
{
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(0.0, 1.0);

  double R1=dis(gen);
  double R2=dis(gen);

  double Diffusion_Coefficient = 4.3 * pow(10,2); //µm²/s

  return abs(sqrt(2 * Diffusion_Coefficient * timeBeforeDecay)*sqrt(-2*log(R1))*cos(2*CLHEP::pi*R2));
}


G4ThreeVector PGA_impl::GenerateNewPositionAfterDiffusion(G4ThreeVector previousPosition, double diffusion_distance)
{
  double x1=previousPosition[0]*1000;
  double y1=previousPosition[1]*1000;
  double z1=previousPosition[2]*1000;

  double x2;
  double y2;
  double z2;

  diffusion_distance = 12.0;

  // G4cout << "\n\n previousPosition in G4 unit : " << previousPosition << G4endl;
  //
  // G4cout << "\n\n previousPosition : " << previousPosition*1000 << G4endl;

  // G4cout << "\n \n diffusion_distance \n \n" << diffusion_distance << "\n \n" << G4endl;

  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis_x(x1 - diffusion_distance , x1 + diffusion_distance);

  x2 = dis_x(gen);

  // G4cout << "\n x1 : " << x1 << G4endl;
  // G4cout << "\n x2 : " << x2 << G4endl;

  std::uniform_real_distribution<> dis_y(y1 - sqrt(square(diffusion_distance) - square(x2-x1)) , y1 + sqrt(square(diffusion_distance) - square(x2-x1)));

  y2 = dis_y(gen);

  // G4cout << "\n y1 : " << y1 << G4endl;
  // G4cout << "\n y2 : " << y2 << G4endl;

  std::uniform_real_distribution<> dis_uniform(0, 1);

  double uni_try = dis_uniform(gen);

  if (uni_try<0.5)
  {
    z2 = z1 - sqrt(square(diffusion_distance) - square(x2-x1) - square(y2-y1));
  }

  if (uni_try>=0.5)
  {
    z2 = z1 + sqrt(square(diffusion_distance) - square(x2-x1) - square(y2-y1));
  }

  // G4cout << "\n z1 : " << z1 << G4endl;
  // G4cout << "\n z2 : " << z2 << G4endl;

  new_G4_particle_position = G4ThreeVector(x2*pow(10,-3),y2*pow(10,-3),z2*pow(10,-3));

  // G4cout << "Previous particle position : " << previousPosition << G4endl;
  // G4cout << "Diffusion_distance : " << diffusion_distance << G4endl;
  // G4cout << "New particle position : " << new_G4_particle_position << G4endl;
  if (isnan(new_G4_particle_position[1]))
  {
    // G4cout << "square(diffusion_distance) : " << square(diffusion_distance) << G4endl;
    // G4cout << "square(x2-x1) : " << square(x2-x1) << G4endl;
    GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);
  }
  if (isnan(new_G4_particle_position[2]))
  {
    // G4cout << "square(diffusion_distance) : " << square(diffusion_distance) << G4endl;
    // G4cout << "square(x2-x1) : " << square(x2-x1) << G4endl;
    // G4cout << "square(y2-y1) : " << square(y2-y1) << G4endl;
    GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);
  }
  // G4cout << "Check distance entre positions :" << sqrt(square(x2-x1)+square(y2-y1)+square(z2-z1)) << G4endl;

  Point_3 new_CGAL_particle_position = Utils::myCGAL::to_CPOP(new_G4_particle_position);

  auto cell = findCell(new_CGAL_particle_position);

  if (cell && cell->hasIn(new_CGAL_particle_position))
   {
   //G4cout<< "cell->hasIn(edep_pos)" << G4endl;
   // G4cout << "\n need to generate a new position \n" << G4endl;
   nb_essais_diffusion+=1;
   // G4cout << "\n nb_essais_diffusion \n" << nb_essais_diffusion << G4endl;
   GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);
   }
  else
    {//G4cout<< "No hasIn" << G4endl;
    }


  // if (findCell(new_CGAL_particle_position))
  // {G4cout << "\n need to generate a new position \n" << G4endl;
  //  nb_essais_diffusion+=1;
  //  G4cout << "\n nb_essais_diffusion \n" << nb_essais_diffusion << G4endl;
  //  GenerateNewPositionAfterDiffusion(previousPosition, diffusion_distance);}

  // G4cout << "\n newPosition : " << new_G4_particle_position*1000 << G4endl;
  //
  // G4cout << "\n newPosition in G4 unit : " << new_G4_particle_position << G4endl;
  //
  // G4cout << "\n distance between both previous and new positions : " << sqrt(pow(previousPosition[0]-new_G4_particle_position[0],2) + pow(previousPosition[1]-new_G4_particle_position[1],2) + pow(previousPosition[2]-new_G4_particle_position[2],2))*1000 << G4endl;

  assert(sqrt(pow(previousPosition[0]-new_G4_particle_position[0],2) + pow(previousPosition[1]-new_G4_particle_position[1],2) + pow(previousPosition[2]-new_G4_particle_position[2],2))*1000 == diffusion_distance);

  return (new_G4_particle_position);



}

std::string PGA_impl::findOrganelle(const Settings::nCell::t_Cell_3 *cell, const Point_3 &point)
{
    std::vector<Settings::nCell::t_Nucleus_3*> nuclei = cell->getNuclei();
    std::vector<Settings::nCell::t_Nucleus_3*>::const_iterator itNuclei;
    for(itNuclei = nuclei.begin(); itNuclei != nuclei.end(); ++itNuclei)
    {
        if((*itNuclei)->hasIn(point) )
        {
            return "nucleus";
        }
    }
    // If the point is not in the nucleus, it is in the cytoplasm
    return "cytoplasm";
}


}
