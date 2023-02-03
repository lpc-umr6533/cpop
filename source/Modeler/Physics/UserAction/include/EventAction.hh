#ifndef EVENTACTION_HH
#define EVENTACTION_HH

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include <iostream>
#include "Population.hh"
#include "RunAction.hh"


namespace cpop {

class RunAction;

class EventAction : public G4UserEventAction
{
  /// Victor Levrague : modification of begin and end of Event Action in order collect info at the event level
public:
    EventAction(const Population& population, RunAction* runAction);
    ~EventAction() = default;

    std::string PreOrganelle;

    virtual void BeginOfEventAction(const G4Event*evt);
    virtual void EndOfEventAction(const G4Event*);

    void AddEdepNucl(G4double edepn, G4int id_cell) { fEdepn[id_cell] += edepn; }
    void AddEdepCyto(G4double edepc, G4int id_cell) { fEdepc[id_cell] += edepc; }
    void AddEdepSpheroid(G4double edep_sph) { fEdep_sph += edep_sph; }

    std::vector<double> Ei;
    std::vector<double> Ef;
    std::vector<int> ID_Cellule;

    G4int tailleEi;
    G4int tailleEf;
    G4String nameParticle;

    G4int compteurEi_He_DansPremierNoyau_EnStock;
    G4int compteurArretdsNoyauApresGenDansLeNoyau;
    G4double compteur_first_appearance;

    G4String FirstVolume;
    G4double Energie_emission;

    G4double Ei_He_GenDansNoyau;
    G4int ID_Cell_GenDansNoyau;
    G4int ID_Cellule_ArretDsNoyau;

    G4int ID_Cell_D_Emission;

    G4int evt_id;

    G4int eventID_for_stepping_action;

    std::vector<double> fEdepn;
    std::vector<double> fEdepc;
    G4double fEdep_sph;

    G4int indice_if_diffusion_event = 0;

private:

    const Population* population_;
    RunAction* fRunAction;

};

}

#endif // EVENTACTION_HH
