// #ifndef STEPPINGACTION_HH
// #define STEPPINGACTION_HH

#include <memory>

#include "G4UserSteppingAction.hh"

#include "Octree.hh"
#include "OctreeNodeForSpheroidalCell.hh"
#include "Population.hh"
#include "Cell_Utils.hh"
#include "PrimaryGeneratorAction.hh"



namespace cpop {

class CpopEventAction;

class CpopSteppingAction : public G4UserSteppingAction
{
  ///Victor Levrague : modification of SteppingAction in order to get entrance and exit enrgies of alpha particles in nuclei///
public:
    CpopSteppingAction(const Population& population, CpopEventAction* eventAction, PGA_impl &pga_impl);
    ~CpopSteppingAction() = default;

    virtual void UserSteppingAction(const G4Step*);

    /// \brief point in CPOP unit
    const Settings::nCell::t_Cell_3 *findCell(const Settings::Geometry::Point_3& point);
    std::string findOrganelle(const Settings::nCell::t_Cell_3* cell, const Settings::Geometry::Point_3& point);
    std::string findRegion(const Settings::nCell::t_Cell_3* cell);

    G4double Ei_He;
    G4double Ei_He_temp;

    void addTupleRow(const G4Step* step, int cellID, const std::string& organelle, const std::string& region);

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

    G4int eventID;
    CpopEventAction*  fEventAction;

    G4int preCellID;

    PGA_impl* fPGA_impl;

};

}

// #endif // STEPPINGACTION_HH
