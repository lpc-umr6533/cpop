#include "RunAction.hh"

#include <G4Run.hh>

#include <G4AnalysisManager.hh>

#include "analysis.hh"

namespace cpop {

RunAction::RunAction(const Population &population):
    population_(&population),
    fEdepn_tot(0.),
    fEdepc_tot(0.)
{
	// The choice of analysis technology is done via selection of a namespace
	// in analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetNtupleMerging(false); //switch to true when merging works ...
}

void RunAction::BeginOfRunAction(const G4Run * /*run*/)
{
	RunAction::CreateHistogram();

	// Get analysis manager
	auto analysisManager = G4AnalysisManager::Instance();
	if(!file_name_.empty()) { // If you are setting the output file name using the setter instead of G4 macro
		analysisManager->OpenFile(file_name_);
	} else {
		analysisManager->OpenFile();
	}

	fEdepn_tot.clear();
	fEdepc_tot.clear();

	fEdep_sph_tot=0;

	const Population* population = population_;
	std::vector<SpheroidRegion> regions = population->regions();
	int nb_cell = 0;
	for(const SpheroidRegion& region : regions)
	{
		nb_cell += (region.cells_in_region()).size();
	}

	for (int id_cell=0; id_cell<(population->nb_cell_xml); ++id_cell)
	{
		fEdepn_tot.push_back(0);
		fEdepc_tot.push_back(0);
	}
}

void RunAction::EndOfRunAction(const G4Run * /*run*/)
{
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	const Population* population = population_;
	std::vector<SpheroidRegion> regions = population->regions();
	int nb_cell = 0;
	for(const SpheroidRegion& region : regions)
	{
		nb_cell += (region.cells_in_region()).size();
	}

	G4cout << "******************* Number of cells : " << nb_cell << G4endl;

	//Allows to get total energy deposited in each nucleus and cell //

	if (population->event_level_info_ == 1)
	{
    for (int id_cell=0; id_cell<(population->nb_cell_xml); ++id_cell)
    {
       ///// The tag 'EndOfRun' in the nameParticle category allows to identify that these data are stocked at the RunAction level /////
       analysisManager->FillNtupleSColumn(0, "EndOfRun");
       analysisManager->FillNtupleDColumn(1, 0);
       analysisManager->FillNtupleDColumn(2, 0);
       ///// WARNING : with this method, cells are identified with ids from 3 to total number of cells+2, which are not their real id
       //for analysis purposes, the IDCell.txt must be used to associate the energy deposited here to the real cells ///////
       analysisManager->FillNtupleDColumn(3, id_cell+3);
       analysisManager->FillNtupleDColumn(4, 0);
       analysisManager->FillNtupleDColumn(5, 0);
       analysisManager->FillNtupleSColumn(6, "EndOfRun");
       analysisManager->FillNtupleDColumn(7, fEdepn_tot[id_cell]);
       analysisManager->FillNtupleDColumn(8, fEdepc_tot[id_cell]);
       analysisManager->FillNtupleDColumn(9, fEdep_sph_tot);
       analysisManager->FillNtupleDColumn(10, 2);
       /*G4cout << "Cell n°" << id_cell+3 << G4endl;
       G4cout << "Edep nucleus: " << fEdepn_tot[id_cell] << G4endl;*/

			analysisManager->AddNtupleRow();
		}
    //G4cout << "fEdep_sph_tot: " << fEdep_sph_tot << G4endl;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	analysisManager->Write();
	analysisManager->CloseFile();
}

std::string RunAction::file_name() const
{
    return file_name_;
}

void RunAction::setFile_name(const std::string &file_name)
{
    file_name_ = file_name;
}

void RunAction::AddEdepNucl(G4double edepn, G4int id_cell)
{
  fEdepn_tot[id_cell]  += edepn;
}

void RunAction::AddEdepCyto(G4double edepc, G4int id_cell)
{
  fEdepc_tot[id_cell]  += edepc;
}

void RunAction::AddEdepSpheroid(G4double edepsph)
{
  fEdep_sph_tot  += edepsph;
}

void RunAction::CreateHistogram()
{
	// The choice of analysis technology is done via selection of a namespace
	// in analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetNtupleMerging(false); //switch to true when merging works ...

	// Creating ntuple
	const Population* population = population_;

	if ((population->stepping_level_info_) == 1)
	{
		analysisManager->CreateNtuple("Edep", "Energy deposition by cell");
		analysisManager->CreateNtupleDColumn("posX");
		analysisManager->CreateNtupleDColumn("posY");
		analysisManager->CreateNtupleDColumn("posZ");
		analysisManager->CreateNtupleDColumn("momDirX");
		analysisManager->CreateNtupleDColumn("momDirY");
		analysisManager->CreateNtupleDColumn("momDirZ");
		analysisManager->CreateNtupleDColumn("edep");
		analysisManager->CreateNtupleDColumn("eKin");
		analysisManager->CreateNtupleIColumn("cellID");
		analysisManager->CreateNtupleSColumn("organelle");
		analysisManager->CreateNtupleSColumn("region");
		analysisManager->CreateNtupleIColumn("eventID");
		analysisManager->FinishNtuple();
	}
	else if ((population->event_level_info_) == 1)
	{
		analysisManager->CreateNtuple("cell", "physics");
		analysisManager->CreateNtupleSColumn("nameParticle");
		analysisManager->CreateNtupleDColumn("Ei");
		analysisManager->CreateNtupleDColumn("Ef");
		analysisManager->CreateNtupleDColumn("ID_Cell");
		analysisManager->CreateNtupleDColumn("eventID");
		analysisManager->CreateNtupleDColumn("Cellule_D_Emission");
		analysisManager->CreateNtupleSColumn("Organelle_emission");
		analysisManager->CreateNtupleDColumn("fEdepn");
		analysisManager->CreateNtupleDColumn("fEdepc");
		analysisManager->CreateNtupleDColumn("fEdep_sph");
		analysisManager->CreateNtupleDColumn("indice_if_diffusion");
		analysisManager->FinishNtuple();
	}
}

}
