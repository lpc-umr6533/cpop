#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "Population.hh"

namespace cpop {

class RunAction : public G4UserRunAction
{
/// Victor Levrague : collect energy deposited in all cells with EndOfRun tag///
public:
    RunAction(const Population& population);
    ~RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void AddEdepNucl (G4double edepn, G4int id_cell);
    void AddEdepCyto (G4double edepc, G4int id_cell);
    void AddEdepSpheroid (G4double edepsph);

    std::vector<G4double> fEdepn_tot;
    std::vector<G4double> fEdepc_tot;
    G4double fEdep_sph_tot;

    std::string file_name() const;
    void setFile_name(const std::string &file_name);
    void CreateHistogram();

private:

    std::string file_name_ = "";
    const Population* population_;
};

}

#endif // RUNACTION_HH
