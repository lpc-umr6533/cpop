#include "CpopEventAction.hh"

#include "analysis.hh"
#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

#include "G4SystemOfUnits.hh"
#include "UnitSystemManager.hh"
#include <G4UnitsTable.hh>
#include <G4AnalysisManager.hh>

namespace cpop {

CpopEventAction::CpopEventAction(const Population &population, CpopRunAction* runAction):
	fEdepn(0.),
	fEdepc(0.),
	_population(&population),
	fRunAction(runAction)
{
}

void CpopEventAction::BeginOfEventAction(const G4Event * evt) {
	G4int evt_id = evt->GetEventID();
	G4int print_modulo = G4RunManager::GetRunManager()->GetPrintProgress();
	eventIDForSteppingAction = evt->GetEventID() ;

	PreOrganelle = "";

	if (print_modulo > 0 && evt_id%print_modulo == 0) {
		G4int total_evt = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
		double progress = ((double)evt_id/ total_evt)*100;
		std::cout << "\n---> Begin of Event: " << evt_id  << " (" << progress << "%)\n";
	}

	//// Data initialized to get info at the event level with info stored during SteppingAction ////

	Ei.clear();
	Ef.clear();
	idCell.clear();
	fEdepn.clear();
	fEdepc.clear();

	fEdepSph = 0;

	countFirstAppearance=0.;

	sizeEi=-1;
	sizeEf=-1;
	countEiHeDansPremierNoyauEnStock=0;
	countArretdsNoyauApresGenDansLeNoyau=0;

	const Population* population = _population;
	std::vector<SpheroidRegion> regions = population->regions();

	for (int id_cell=0; id_cell<(population->nbCellXml); ++id_cell) {
		fEdepn.push_back(0);
		fEdepc.push_back(0);
	}

	indiceIfDiffusionEvent = 0;
}

void CpopEventAction::EndOfEventAction(const G4Event* Event) {
	G4int event_id = Event->GetEventID();

	const Population* population = _population;
	std::vector<SpheroidRegion> regions = population->regions();

	/////// Collect energy deposited in each cell for RunAction //////////

	for (int id_cell=0; id_cell<(population->nbCellXml); ++id_cell) {
		fRunAction->AddEdepNucl(fEdepn[id_cell], id_cell);
		fRunAction->AddEdepCyto(fEdepc[id_cell], id_cell);
	}

	fRunAction->AddEdepSpheroid(fEdepSph);

	fEdepn.clear();
	fEdepc.clear();

	if (sizeEi > sizeEf) {
		//// This 'if' is entered if the particle is stopped in a cell /////

		Ef.push_back(0) ;
		sizeEf+=1 ;
		idCell.push_back(idCelluleArretDsNoyau);
	}

	if (countArretdsNoyauApresGenDansLeNoyau==1) {
		///// This 'if' is entered if the particle is stopped in a cell, after being generated in that one  /////

		Ei.push_back(EiHeGenDansNoyau) ;
		Ef.push_back(0) ;
		sizeEi+=1 ;
		idCell.push_back(idCellGenDansNoyau);
	}

	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	////////// Allow to store informations on particles at the event level, in a root file /////////

	if ((population->eventLevelInfo) == 1) {
		for (int i=0; i<(sizeEi+1); ++i) {
			analysisManager->FillNtupleSColumn(1, 0, nameParticle);
			analysisManager->FillNtupleDColumn(1, 1, Ei[i]);
			analysisManager->FillNtupleDColumn(1, 2, Ef[i]);
			analysisManager->FillNtupleDColumn(1, 3, idCell[i]);
			analysisManager->FillNtupleDColumn(1, 4, event_id);
			analysisManager->FillNtupleDColumn(1, 5, idCellDEmission);
			analysisManager->FillNtupleSColumn(1, 6, firstVolume);
			analysisManager->FillNtupleDColumn(1, 7, indiceIfDiffusionEvent);

			analysisManager->AddNtupleRow(1);
		}
	}
}

}
