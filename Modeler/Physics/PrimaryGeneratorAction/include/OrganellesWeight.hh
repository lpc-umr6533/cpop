#ifndef ORGANELLE_WEIGHT_HH
#define ORGANELLE_WEIGHT_HH

#include "RandomEngineManager.hh"
#include "Randomize.hh"

#include <map>
#include "ECellComposition.hh"

#include <assert.h>

using namespace CellComposition;
using namespace std;
//////////////////////////////////////////////////////////////////////////////
/// \brief the ratio of each organelles.
struct OrganellesWeight
{
	map<double, Organelle> organelleRatios;

	OrganellesWeight(
			double pCellMembrane,
			double pNucleoplasm,
			double pNuclearMembrane,
			double pCytoplasm )
	{
		double lSum = pCellMembrane + pNucleoplasm + pNuclearMembrane + pCytoplasm;
		double weight = pCellMembrane;
			//G4cout << "/n/n/n weight CELL_MEMBRANE Ratios : " <<  weight/lSum << G4endl;
    	organelleRatios.insert(make_pair(weight/lSum, _CELL_MEMBRANE ) );
    	weight += pNucleoplasm;
			//G4cout << "/n/n/n weight NUCLEOPLASM Ratios : " <<  weight/lSum << G4endl;
    	organelleRatios.insert(make_pair(weight/lSum, _NUCLEOPLASM ) );
    	weight += pCytoplasm;
			//G4cout << "/n/n/n weight CYTOPLASM  Ratios : " <<  weight/lSum << G4endl;
    	organelleRatios.insert(make_pair(weight/lSum, _CYTOPLASM ) );
    	weight += pNuclearMembrane;
			//G4cout << "/n/n/n weight NUCLEAR_MEMBRANE Ratios : " <<  weight/lSum << G4endl;
    	organelleRatios.insert(make_pair(weight/lSum, _NUCLEAR_MEMBRANE ) );


	}

	/// \brief return the organelle corresponding to the given probability
	Organelle getOrganelle(double pProba) const
	{
		assert(pProba >= 0);
		assert(pProba <= 1);
		map<double, Organelle>::const_iterator itOrg = organelleRatios.lower_bound(pProba);
		assert(itOrg != organelleRatios.end());
		return itOrg->second;
	}

    /// \brief return a random organelle using a uniform distribution
    Organelle getRandomOrganelle() const {
        map<double, Organelle>::const_iterator itOrg;
        do {
            itOrg = organelleRatios.lower_bound(G4UniformRand());
        } while(itOrg == organelleRatios.end());

        return itOrg->second;
    }

};

#endif // ORGANELLE_WEIGHT_HH
