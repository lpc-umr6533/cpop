#include "EventAction.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

namespace cpop {

void EventAction::BeginOfEventAction(const G4Event * evt) {
	G4int evt_id = evt->GetEventID();
	G4int print_modulo = G4RunManager::GetRunManager()->GetPrintProgress();

	if (print_modulo > 0 && evt_id%print_modulo == 0) {
		G4int total_evt = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();
		double progress = ((double)evt_id/ total_evt)*100;
		std::cout << "\n---> Begin of Event: " << evt_id  << " (" << progress << "%)\n";
	}
}

}
