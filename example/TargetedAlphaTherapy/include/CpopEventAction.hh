#ifndef TARGETEDALPHATHERAPY_EVENTACTION_HH
#define TARGETEDALPHATHERAPY_EVENTACTION_HH

#include "G4UserEventAction.hh"
#include <vector>
#include "Population.hh"
#include "CpopRunAction.hh"


namespace cpop {

class CpopRunAction;

class CpopEventAction : public G4UserEventAction {
	/// Victor Levrague : modification of begin and end of Event Action in order collect info at the event level
public:
	CpopEventAction(const Population& population, CpopRunAction* runAction);

	std::string PreOrganelle;

	void BeginOfEventAction(const G4Event*evt) override;
	void EndOfEventAction(const G4Event*) override;

	void AddEdepNucl(G4double edepn, G4int id_cell) { fEdepn[id_cell] += edepn; }
	void AddEdepCyto(G4double edepc, G4int id_cell) { fEdepc[id_cell] += edepc; }
	void AddEdepSpheroid(G4double edep_sph) { fEdepSph += edep_sph; }

	std::vector<double> Ei;
	std::vector<double> Ef;
	std::vector<int> idCell;

	G4int sizeEi;
	G4int sizeEf;
	G4String nameParticle;

	G4int countEiHeDansPremierNoyauEnStock;
	G4int countArretdsNoyauApresGenDansLeNoyau;
	G4double countFirstAppearance;

	G4String firstVolume;
	G4double energyEmission;

	G4double EiHeGenDansNoyau;
	G4int idCellGenDansNoyau;
	G4int idCelluleArretDsNoyau;

	G4int idCellDEmission;

	G4int eventId;

	G4int eventIDForSteppingAction;

	std::vector<double> fEdepn;
	std::vector<double> fEdepc;
	G4double fEdepSph;

	G4int indiceIfDiffusionEvent = 0;

private:

	const Population* _population;
	CpopRunAction* fRunAction;
};

}

#endif
