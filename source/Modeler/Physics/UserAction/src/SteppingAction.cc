#include "SteppingAction.hh"

#include <vector>

#include "analysis.hh"
#include "G4Step.hh"
#include <G4AnalysisManager.hh>

#include "CGAL_Utils.hh"
#include "CellSettings.hh"
#include "AgentSettings.hh"
#include "BoundingBox.hh"
#include "RunAction.hh"

namespace cpop {

SteppingAction::SteppingAction(const Population &population):
	_population(&population)
{
}

void SteppingAction::UserSteppingAction(const G4Step * step) {
	double edep = step->GetTotalEnergyDeposit();

	if(edep > 0) {
		G4ThreeVector pEdepPos = step->GetPreStepPoint()->GetPosition();

		Point_3 edep_pos = Utils::myCGAL::to_CPOP(pEdepPos);
		std::string organelle("");

		if(_lastCell != nullptr && _lastCell->hasIn(edep_pos)) { // Avoid region search and findCell
			organelle = findOrganelle(_lastCell, edep_pos);
			addTupleRow(step, _lastCell->getID(), organelle, _lastRegion);
		} else {
			auto cell = findCell(edep_pos);
			if(cell && cell->hasIn(edep_pos)) {
				_lastCell = cell;
				_lastRegion = findRegion(_lastCell);
				organelle = findOrganelle(cell, edep_pos);
				addTupleRow(step, _lastCell->getID(), organelle, _lastRegion);
			}
		}
	}
}

const Settings::nCell::t_Cell_3* SteppingAction::findCell(const Point_3 &point) {
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
	const t_SpatialableAgent_3* lNearestAgent = _octree->getNearestSpatialableAgent(point);

	return dynamic_cast<const Settings::nCell::t_Cell_3*>(lNearestAgent);
}

std::string SteppingAction::findOrganelle(const Settings::nCell::t_Cell_3 *cell, const Point_3 &point) {
	std::vector<Settings::nCell::t_Nucleus_3*> nuclei = cell->getNuclei();
	for(auto const& itNuclei : nuclei) {
		if(itNuclei->hasIn(point))
			return "nucleus";
	}

	// If the point is not in the nucleus, it is in the cytoplasm
	return "cytoplasm";
}

std::string SteppingAction::findRegion(const Settings::nCell::t_Cell_3 *cell) {
	std::vector<SpheroidRegion> regions = _population->regions();
	for(auto& region : regions) {
		const std::unordered_set<const Settings::nCell::t_Cell_3*>& samples = region.sample();
		auto found = samples.find(cell);

		if(found != samples.end())
			return region.name();
	}

	throw "TODO";
}

void addTupleRow(const G4Step *step, int cellID, const std::string &organelle, const std::string &region) {
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	double edep = step->GetTotalEnergyDeposit();
	G4StepPoint* preStepPoint = step->GetPreStepPoint();
	G4ThreeVector momDir = preStepPoint->GetMomentum();
	G4ThreeVector edepPos = preStepPoint->GetPosition();

	analysisManager->FillNtupleDColumn(0, edepPos.x());
	analysisManager->FillNtupleDColumn(1, edepPos.y());
	analysisManager->FillNtupleDColumn(2, edepPos.z());
	analysisManager->FillNtupleDColumn(3, momDir.x());
	analysisManager->FillNtupleDColumn(4, momDir.y());
	analysisManager->FillNtupleDColumn(5, momDir.z());
	analysisManager->FillNtupleDColumn(6, edep);
	analysisManager->FillNtupleIColumn(7, cellID);
	analysisManager->FillNtupleSColumn(8, organelle);
	analysisManager->FillNtupleSColumn(9, region);
	analysisManager->AddNtupleRow();
}

}
