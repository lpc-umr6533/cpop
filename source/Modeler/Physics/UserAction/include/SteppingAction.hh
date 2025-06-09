#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <memory>

#include "G4UserSteppingAction.hh"

#include "Octree.hh"
#include "OctreeNodeForSpheroidalCell.hh"
#include "Population.hh"
#include "Cell_Utils.hh"

namespace cpop {

class SteppingAction : public G4UserSteppingAction {
public:
	SteppingAction(const Population& population);

	void UserSteppingAction(const G4Step*) override;

	/// \brief point in CPOP unit
	const Settings::nCell::t_Cell_3 *findCell(const Settings::Geometry::Point_3& point);
	std::string findOrganelle(const Settings::nCell::t_Cell_3* cell, const Settings::Geometry::Point_3& point);
	std::string findRegion(const Settings::nCell::t_Cell_3* cell);

private:
	/// \brief Octree containing SAMPLED cells
	std::unique_ptr<Octree<OctreeNodeForSpheroidalCell>> _octree;
	/// \brief Cell population
	const Population* _population;
	/// \brief The last sampled cell where a step occured
	const Settings::nCell::t_Cell_3* _lastCell = nullptr;
	/// \brief The region containing the last sampled cell
	std::string _lastRegion = "";

	bool _isInitialized = false;
};

void addTupleRow(const G4Step* step, int cellID, const std::string& organelle, const std::string& region);

}

#endif
