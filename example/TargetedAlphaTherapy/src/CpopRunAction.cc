#include "CpopRunAction.hh"

#include <G4Run.hh>

#include <G4AnalysisManager.hh>

#include "analysis.hh"
#include <algorithm>

namespace cpop {

CpopRunAction::CpopRunAction(const Population &population):
    population_(&population),
    fEdepn_tot(0.),
    fEdepc_tot(0.)
{
	// The choice of analysis technology is done via selection of a namespace
	// in analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetNtupleMerging(false); //switch to true when merging works ...
}

void CpopRunAction::BeginOfRunAction(const G4Run * /*run*/)
{
	CpopRunAction::CreateHistogram();

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

void CpopRunAction::EndOfRunAction(const G4Run * /*run*/)
{
  labeled_cells = population_-> labeled_cells;

  std::unordered_set<int> labeled_cells_id;
  for (const Settings::nCell::t_Cell_3* cell : labeled_cells)
  { G4cout << "Cell nb°" << cell->getID() << " is labeled" << G4endl;
    labeled_cells_id.insert(cell->getID());}

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

	// if (population->event_level_info_ == 1)
	// {
    for (int id_cell=0; id_cell<(population->nb_cell_xml); ++id_cell)
    {
		///// WARNING : cells are identified with ids from 3 to total number of cells+2, which are not their real id
		//for analysis purposes, the IDCell.txt must be used to associate the energy deposited here to the real cells ///////
		analysisManager->FillNtupleDColumn(2, 0, id_cell+3);
		analysisManager->FillNtupleDColumn(2, 1, fEdepn_tot[id_cell]);
		analysisManager->FillNtupleDColumn(2, 2, fEdepc_tot[id_cell]);
		analysisManager->FillNtupleDColumn(2, 3, fEdep_sph_tot);
		analysisManager->FillNtupleIColumn(2, 4, labeled_cells_id.count(id_cell+3));
		analysisManager->FillNtupleSColumn(2, 5, determine_cell_region_by_id(G4int cell_id));

		analysisManager->AddNtupleRow(2);
	}
    //G4cout << "fEdep_sph_tot: " << fEdep_sph_tot << G4endl;
	// }

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	analysisManager->Write();
	analysisManager->CloseFile();
}

std::string CpopRunAction::determine_cell_region_by_id(G4int cell_id)
{
	std::vector<SpheroidRegion> regions = population_->regions();
	for(const SpheroidRegion& region : regions)
	{
		std::vector<const Settings::nCell::t_Cell_3 *> cells_in_region = region.cells_in_region();
        auto it = std::find_if(cells_in_region.begin(), cells_in_region.end(),
            [cell_id](const Settings::nCell::t_Cell_3* cell) { return cell->getID() == cell_id; });

        if (it != cells_in_region.end())
        {
            return region.name();
        }
	}
	return "Unknown";
}


std::string CpopRunAction::file_name() const
{
    return file_name_;
}

void CpopRunAction::setFile_name(const std::string &file_name)
{
    file_name_ = file_name;
}

void CpopRunAction::AddEdepNucl(G4double edepn, G4int id_cell)
{
  fEdepn_tot[id_cell]  += edepn;
}

void CpopRunAction::AddEdepCyto(G4double edepc, G4int id_cell)
{
  fEdepc_tot[id_cell]  += edepc;
}

void CpopRunAction::AddEdepSpheroid(G4double edepsph)
{
  fEdep_sph_tot  += edepsph;
}

void CpopRunAction::CreateHistogram()
{
	// The choice of analysis technology is done via selection of a namespace
	// in analysis.hh
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetNtupleMerging(false); //switch to true when merging works ...

	// Creating ntuple
	const Population* population = population_;

	analysisManager->CreateNtuple("Step", "SteppingAction level");
	analysisManager->CreateNtupleDColumn(0, "posX");
	analysisManager->CreateNtupleDColumn(0, "posY");
	analysisManager->CreateNtupleDColumn(0, "posZ");
	analysisManager->CreateNtupleDColumn(0, "momDirX");
	analysisManager->CreateNtupleDColumn(0, "momDirY");
	analysisManager->CreateNtupleDColumn(0, "momDirZ");
	analysisManager->CreateNtupleDColumn(0, "edep");
	analysisManager->CreateNtupleDColumn(0, "eKin");
	analysisManager->CreateNtupleIColumn(0, "cellID");
	analysisManager->CreateNtupleSColumn(0, "organelle");
	analysisManager->CreateNtupleSColumn(0, "region");
	analysisManager->CreateNtupleIColumn(0, "eventID");
	analysisManager->FinishNtuple();
  //
	analysisManager->CreateNtuple("Event", "EventAction level");
	analysisManager->CreateNtupleSColumn(1, "nameParticle");
	analysisManager->CreateNtupleDColumn(1, "Ei");
	analysisManager->CreateNtupleDColumn(1, "Ef");
	analysisManager->CreateNtupleDColumn(1, "ID_Cell");
	analysisManager->CreateNtupleDColumn(1, "eventID");
	analysisManager->CreateNtupleDColumn(1, "Cellule_D_Emission");
	analysisManager->CreateNtupleSColumn(1, "Organelle_emission");
	analysisManager->CreateNtupleDColumn(1, "indice_if_diffusion");
	analysisManager->FinishNtuple();
  //
  analysisManager->CreateNtuple("Run", "RunAction level");
  analysisManager->CreateNtupleDColumn(2, "ID_Cell");
  analysisManager->CreateNtupleDColumn(2, "fEdepn");
  analysisManager->CreateNtupleDColumn(2, "fEdepc");
  analysisManager->CreateNtupleDColumn(2, "fEdep_sph");
  analysisManager->CreateNtupleIColumn(2, "is_labeled");
  analysisManager->CreateNtupleSColumn(2, "region");
  analysisManager->FinishNtuple();

}

}
