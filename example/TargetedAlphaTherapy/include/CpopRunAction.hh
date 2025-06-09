#ifndef TARGETEDALPHATHERAPY_RUN_ACTION_HH
#define TARGETEDALPHATHERAPY_RUN_ACTION_HH

#include "G4UserRunAction.hh"
#include "Population.hh"

namespace cpop {

class CpopRunAction : public G4UserRunAction {
	/// Victor Levrague : collect energy deposited in all cells///
public:
	CpopRunAction(const Population& population);

	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

	void AddEdepNucl(G4double edepn, G4int id_cell);
	void AddEdepCyto(G4double edepc, G4int id_cell);
	void AddEdepSpheroid(G4double edepsph);

	std::vector<G4double> fEdepn_tot;
	std::vector<G4double> fEdepc_tot;
	G4double fEdep_sph_tot;

	[[nodiscard]] std::string file_name() const;
	void setFile_name(const std::string &file_name);
	void CreateHistogram();

	std::vector<const Settings::nCell::t_Cell_3 *> labeled_cells;

	std::string determine_cell_region_by_id(G4int cell_id);

private:
	std::string _filename = "";
	const Population* _population;
};

}

#endif
