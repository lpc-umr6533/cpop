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

#include <G4AnalysisManager.hh>

namespace cpop {

SteppingAction::SteppingAction(const Population &population, EventAction* eventAction, PGA_impl &pga_impl)
    :G4UserSteppingAction(),
      population_(&population),
      fEventAction(eventAction),
      fPGA_impl(&pga_impl)
{
}

void SteppingAction::UserSteppingAction(const G4Step * step)
{
    double real_spheroid_radius = population_->spheroid_radius();

    double edep = step->GetTotalEnergyDeposit()/CLHEP::keV;

    G4StepPoint * preStep = step->GetPreStepPoint();
    G4StepPoint * postStep = step->GetPostStepPoint();

    std::string PreOrganelle;

    G4String nameParticle = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();

    if(edep > 0) {
      G4ThreeVector pEdepPos = step->GetPreStepPoint()->GetPosition();

        Point_3 edep_pos = Utils::myCGAL::to_CPOP(pEdepPos);
        std::string organelle("");

        if(last_cell_ != nullptr && last_cell_->hasIn(edep_pos)) { // Avoid region search and findCell
            organelle = findOrganelle(last_cell_, edep_pos);
            addTupleRow(step, last_cell_->getID(), organelle, last_region_);
        } else {
            auto cell = findCell(edep_pos);
            if(cell && cell->hasIn(edep_pos)) {
                last_cell_ = cell;
                last_region_ = findRegion(last_cell_);
                organelle = findOrganelle(cell, edep_pos);
                addTupleRow(step, last_cell_->getID(), organelle, last_region_);
            }
        }

    }

    if((fPGA_impl->indice_if_diffusion)==1)
    {
      (fEventAction->indice_if_diffusion_event) = 1;
    }


    G4ThreeVector pEdepPos = step->GetPreStepPoint()->GetPosition();
    double check_distance = pEdepPos[0]*pEdepPos[0] + pEdepPos[1]*pEdepPos[1] + pEdepPos[2]*pEdepPos[2];


    if (check_distance<=real_spheroid_radius)
    {
      fEventAction->AddEdepSpheroid(edep);
    }

    Point_3 edep_pos = Utils::myCGAL::to_CPOP(pEdepPos);

    PreOrganelle = fEventAction->PreOrganelle ;

    auto cell = findCell(edep_pos);


    if (step->IsFirstStepInVolume() and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium") and ((fEventAction->compteur_first_appearance)==0) )
    {
      if (cell && cell->hasIn(edep_pos))
      {
      // Détecte le premier step de la particule dans le world et permet de renvoyer son volume et énergie d'émission

      fEventAction->FirstVolume = PreOrganelle;
      fEventAction->Energie_emission=preStep->GetKineticEnergy()/CLHEP::keV;
      fEventAction->ID_Cell_D_Emission = fPGA_impl->current_cell_id;
      fEventAction->compteur_first_appearance+=1;

    }

    else
    {
      const G4TouchableHandle& preStepTouch = preStep->GetTouchableHandle();
      fEventAction->FirstVolume = preStepTouch->GetVolume()->GetName();
      fEventAction->Energie_emission=preStep->GetKineticEnergy()/CLHEP::keV;
      fEventAction->ID_Cell_D_Emission = fPGA_impl->current_cell_id;
      fEventAction->compteur_first_appearance+=1;
    }

    }


    if(cell && cell->hasIn(edep_pos))
    {

    std::string PostOrganelle;
    PostOrganelle = findOrganelle(cell, edep_pos);

    fEventAction->PreOrganelle = PostOrganelle;

    G4String PreLVName ;
    PreLVName= step->GetPreStepPoint()->GetTouchable()->GetVolume()->GetLogicalVolume()->GetName();

    if ((PreOrganelle == "nucleus") and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium"))
    {
      // Get energy deposited by alphas in nucleus //
      G4double edepStepn = step->GetTotalEnergyDeposit()/CLHEP::keV;
      fEventAction->AddEdepNucl(edepStepn, cell->getID() - 3);
    }
    //
    if ((PreOrganelle == "cytoplasm") and (cell->hasIn(edep_pos)) and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium"))
    {
      // Get energy deposited by alphas in cytoplasm //
      G4double edepStepc = step->GetTotalEnergyDeposit()/CLHEP::keV;
      fEventAction->AddEdepCyto(edepStepc, cell->getID() - 3);

    }

    preCellID = cell->getID();


    if ((PostOrganelle == "nucleus") and step->IsFirstStepInVolume() and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium") )
    {
      // Détecte quand une particule rentre dans (ou est émise depuis) un noyau pour la première fois, et gère le cas où la particule s'arrête dans ce noyau après y avoir été émise

      Ei_He_temp=preStep->GetKineticEnergy()/CLHEP::keV ;

      if ((fEventAction->compteurEi_He_DansPremierNoyau_EnStock)==0)
      {
        Ei_He=Ei_He_temp;
        (fEventAction->compteurEi_He_DansPremierNoyau_EnStock)=1;

        fEventAction->compteurArretdsNoyauApresGenDansLeNoyau+=1;
        fEventAction->Ei_He_GenDansNoyau=Ei_He;

        fEventAction->ID_Cell_GenDansNoyau = cell->getID();

      }

      // G4cout << "\n\n\n Gen dans noyau "  << G4endl;

      fEventAction-> nameParticle=nameParticle;

    }


    if ((PreOrganelle != "nucleus") and (PostOrganelle == "nucleus") and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium"))
    {

      fEventAction->Ei.push_back(preStep->GetKineticEnergy()/CLHEP::keV);
      fEventAction->tailleEi += 1;
      fEventAction->compteurArretdsNoyauApresGenDansLeNoyau -= 1;

      fEventAction-> nameParticle = nameParticle;

      fEventAction->ID_Cellule_ArretDsNoyau = cell->getID();

    }


    if ((PreOrganelle == "nucleus") and (PostOrganelle == "cytoplasm") and (nameParticle=="alpha+" or nameParticle=="alpha" or nameParticle=="helium"))
    {

      fEventAction->Ef.push_back(postStep->GetKineticEnergy()/CLHEP::keV)  ;
      fEventAction->tailleEf+=1;


      if ((fEventAction->tailleEf)>(fEventAction->tailleEi))
        {
          //Ce test est utile dans le cas d'une particule qui sort du noyau en y ayant été générée

          fEventAction->Ei.push_back(Ei_He)  ;
          fEventAction->tailleEi+=1;

          fEventAction->compteurArretdsNoyauApresGenDansLeNoyau-=1;

        }

      fEventAction-> nameParticle=nameParticle;

      fEventAction->ID_Cellule.push_back(cell->getID());
    }


    }



}

const Settings::nCell::t_Cell_3* SteppingAction::findCell(const Point_3 &point)
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

std::string SteppingAction::findOrganelle(const Settings::nCell::t_Cell_3 *cell, const Point_3 &point)
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

std::string SteppingAction::findRegion(const Settings::nCell::t_Cell_3 *cell)
{
    std::vector<SpheroidRegion> regions = population_->regions();
    for(auto it = regions.begin(); it != regions.end() ; ++it) {
        const std::unordered_set<const Settings::nCell::t_Cell_3*>& samples = it->sample();
        auto found = samples.find(cell);

        if(found != samples.end()) {
            return it->name();
        }
    }

		// TODO check validity
		return {};
}

void SteppingAction::addTupleRow(const G4Step *step, int cellID, const std::string &organelle, const std::string &region)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    double edep = step->GetTotalEnergyDeposit();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4ThreeVector momDir = preStepPoint->GetMomentum();
    G4ThreeVector edepPos = preStepPoint->GetPosition();

    G4double eKin = preStepPoint->GetKineticEnergy()/CLHEP::keV;

    const Population* population = population_;

    if ((population->stepping_level_info_) == 1)
    {
    analysisManager->FillNtupleDColumn(0, edepPos.x());
    analysisManager->FillNtupleDColumn(1, edepPos.y());
    analysisManager->FillNtupleDColumn(2, edepPos.z());
    analysisManager->FillNtupleDColumn(3, momDir.x());
    analysisManager->FillNtupleDColumn(4, momDir.y());
    analysisManager->FillNtupleDColumn(5, momDir.z());
    analysisManager->FillNtupleDColumn(6, edep);
    analysisManager->FillNtupleDColumn(7, eKin); //in keV
    analysisManager->FillNtupleIColumn(8, cellID);
    analysisManager->FillNtupleSColumn(9, organelle);
    analysisManager->FillNtupleSColumn(10, region);
    analysisManager->FillNtupleIColumn(11, fEventAction->eventID_for_stepping_action);
    analysisManager->AddNtupleRow();
  }
}

}
