#include "EventAction.hh"

#include "analysis.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "RunAction.hh"

#include "G4SystemOfUnits.hh"
#include "UnitSystemManager.hh"
#include <G4UnitsTable.hh>
#include <G4AnalysisManager.hh>

namespace cpop {

EventAction::EventAction(const Population &population, RunAction* runAction)
    :G4UserEventAction(),
    population_(&population),
    fRunAction(runAction),
    fEdepn(0.),
    fEdepc(0.)
{

}

void EventAction::BeginOfEventAction(const G4Event * evt)
{
    G4int evt_id = evt->GetEventID();
    G4int print_modulo = G4RunManager::GetRunManager()->GetPrintProgress();
    eventID_for_stepping_action = evt->GetEventID() ;

    PreOrganelle = "";

    if (print_modulo > 0 && evt_id%print_modulo == 0) {
        G4int total_evt = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
        double progress = ((double)evt_id/ total_evt)*100;
        std::cout << "\n---> Begin of Event: " << evt_id  << " (" << progress << "%)\n";
    }

    //// Data initialized to get info at the event level with info stored during SteppingAction ////

    Ei.clear();
    Ef.clear();
    ID_Cellule.clear();
    fEdepn.clear();
    fEdepc.clear();

    fEdep_sph = 0;

    compteur_first_appearance=0.;

    tailleEi=-1;
    tailleEf=-1;
    compteurEi_He_DansPremierNoyau_EnStock=0;
    compteurArretdsNoyauApresGenDansLeNoyau=0;


    const Population* population = population_;
    std::vector<SpheroidRegion> regions = population->regions();
    int nb_cell = 0;
    for(const SpheroidRegion& region : regions)
    {
      nb_cell += (region.cells_in_region()).size();
    }

    for (int id_cell=0; id_cell<(population->nb_cell_xml); ++id_cell)
    {
      fEdepn.push_back(0);
      fEdepc.push_back(0);
    }

    indice_if_diffusion_event = 0;




}

void EventAction::EndOfEventAction(const G4Event* Event)
{

  G4int event_id = Event->GetEventID();

  const Population* population = population_;
  std::vector<SpheroidRegion> regions = population->regions();
  int nb_cell = 0;
  for(const SpheroidRegion& region : regions)
  {
    nb_cell += (region.cells_in_region()).size();
  }

  /////// Collect energy deposited in each cell for RunAction //////////

  for (int id_cell=0; id_cell<(population->nb_cell_xml); ++id_cell)
  {
    fRunAction->AddEdepNucl(fEdepn[id_cell], id_cell);
    fRunAction->AddEdepCyto(fEdepc[id_cell], id_cell);
  }

  fRunAction->AddEdepSpheroid(fEdep_sph);

  fEdepn.clear();
  fEdepc.clear();

  if (tailleEi>tailleEf)
    {
      //// This 'if' is entered if the particle is stopped in a cell /////

      Ef.push_back(0) ;
      tailleEf+=1 ;
      ID_Cellule.push_back(ID_Cellule_ArretDsNoyau);
    }


  if (compteurArretdsNoyauApresGenDansLeNoyau==1)
    {
        ///// This 'if' is entered if the particle is stopped in a cell, after being generated in that one  /////

      Ei.push_back(Ei_He_GenDansNoyau) ;
      Ef.push_back(0) ;
      tailleEi+=1 ;
      ID_Cellule.push_back(ID_Cell_GenDansNoyau);
    }

  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  ////////// Allow to store informations on particles at the event level, in a root file /////////


  if ((population->event_level_info_) == 1)
  {
    for (int i=0; i<(tailleEi+1); ++i)
    {
       analysisManager->FillNtupleSColumn(1, 0, nameParticle);
       analysisManager->FillNtupleDColumn(1, 1, Ei[i]);
       analysisManager->FillNtupleDColumn(1, 2, Ef[i]);
       analysisManager->FillNtupleDColumn(1, 3, ID_Cellule[i]);
       analysisManager->FillNtupleDColumn(1, 4, event_id);
       analysisManager->FillNtupleDColumn(1, 5, ID_Cell_D_Emission);
       analysisManager->FillNtupleSColumn(1, 6, FirstVolume);
       analysisManager->FillNtupleDColumn(1, 7, 0);
       analysisManager->FillNtupleDColumn(1, 8, 0);
       analysisManager->FillNtupleDColumn(1, 9, fEdep_sph); // dans python --> supprimer les doubles en triant suivant eventID
       analysisManager->FillNtupleDColumn(1, 10, indice_if_diffusion_event);

       analysisManager->AddNtupleRow(1);
    }
  }

      G4int evt_id = Event->GetEventID();




 ////////////////


}

}
