#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include <memory>

#include "G4UserSteppingAction.hh"

#include "Octree.hh"
#include "OctreeNodeForSpheroidalCell.hh"
#include "Population.hh"
#include "Cell_Utils.hh"

namespace cpop {

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(const Population& population);
    ~SteppingAction() = default;

    virtual void UserSteppingAction(const G4Step*);

    /// \brief point in CPOP unit
    const Settings::nCell::t_Cell_3 *findCell(const Settings::Geometry::Point_3& point);
    std::string findOrganelle(const Settings::nCell::t_Cell_3* cell, const Settings::Geometry::Point_3& point);
    std::string findRegion(const Settings::nCell::t_Cell_3* cell);

private:
    /// \brief Octree containing SAMPLED cells
    std::unique_ptr<Octree<OctreeNodeForSpheroidalCell>> octree_;
    /// \brief Cell population
    const Population* population_;
    /// \brief The last sampled cell where a step occured
    const Settings::nCell::t_Cell_3* last_cell_ = nullptr;
    /// \brief The region containing the last sampled cell
    std::string last_region_ = "";

    bool is_initialized_ = false;

};

void addTupleRow(const G4Step* step, int cellID, const std::string& organelle, const std::string& region);

}

#endif // STEPPINGACTION_HH
