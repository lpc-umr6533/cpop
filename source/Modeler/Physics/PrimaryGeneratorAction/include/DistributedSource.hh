#ifndef DISTRIBUTEDSOURCE_HH
#define DISTRIBUTEDSOURCE_HH

#include <memory>
#include <unordered_map>

#include "CGAL_Utils.hh"
#include "Source.hh"
#include "CellSettings.hh"
#include "OrganellesWeight.hh"
#include "DistributedSourceMessenger.hh"

#include <boost/math/special_functions/erf.hpp>
#include <math.h>
#include <iostream>
#include <vector>

#include <random>
#include "RandomEngineManager.hh"
#include "Randomize.hh"

namespace cpop {

class SpheroidRegion;

/// \brief This class is used to store particle source information for each cell
class SourceInfo {
/// Victor Levrague : functions to allow random positions for each different particle generated on a cell ///

public:
    SourceInfo(const Settings::nCell::t_Cell_3* cell, int nb_source, int part_per_source, const OrganellesWeight& organelle_weight)
        : cell_(cell),
          number_source_(nb_source),
          number_particles_per_source(part_per_source)
    {
        // Pick a random organelle
        auto organelle = organelle_weight.getRandomOrganelle();

        Point_3 pos = Utils::myCGAL::to_G4(cell_->getSpotOnOrganelle(organelle));

        position_in_cell_.push_back(G4ThreeVector(pos.x(), pos.y(), pos.z()));
    }

    bool HasLeft() const { return already_generated_ < totalSecondary(); }
    void addDistributedSource(int inc = 1) { number_source_ += inc; }
    void AddParticlePosition(OrganellesWeight& organelle_weight)
    { auto organelle = organelle_weight.getRandomOrganelle();

    Point_3 pos = Utils::myCGAL::to_G4(cell_->getSpotOnOrganelle(organelle));

    position_in_cell_.push_back(G4ThreeVector(pos.x(), pos.y(), pos.z()));}

    void Update() { ++already_generated_; }
    int getID_SourceInfo() {return cell_->getID();}

    std::vector<G4ThreeVector> GetPosition() const {
      // G4cout<< "GetPosition called in NanoSource.hh" << G4endl;
      // G4cout<< "position in cell x : " << position_in_cell_[0].getX() << G4endl;
      // G4cout<< "position in cell y : " << position_in_cell_[0].getY() << G4endl;
      // G4cout<< "position in cell z: " << position_in_cell_[0].getZ() << G4endl;
      return position_in_cell_;}

    /// \brief Position in cell
    std::vector<G4ThreeVector> position_in_cell_;


private:

    int totalSecondary() const { return number_source_*number_particles_per_source; }

    /// \brief Cell containing particle sources
    const Settings::nCell::t_Cell_3* cell_;
    /// \brief Number of source in the cell
    int number_source_ = 0;
    /// \brief Number of secondary particle to generate for one source
    int number_particles_per_source = 0;
    /// \brief Number of secondary particle already generated
    int already_generated_ = 0;

};

class DistributedSource : public Source
{
 /// Victor Levrague : modification of the distribute function in order to set a maximum number of particles per cell, and a % of labeled cells ///
public:
    DistributedSource(const std::string& name, const Population& population);

    std::vector<G4ThreeVector> GetPosition() override;
    int getID_OfCell();
    void Update() override;
    bool HasLeft() override;

    /// \brief Distribute all the particle sources in a cell
    void Initialize() override;

    int number_distributed() const;
    void setNumber_source(int number_source);
    void setMax_number_source_per_cell_necrosis(int number_source_per_cell_arg);
    void setMax_number_source_per_cell_intermediary(int number_source_per_cell_arg);
    void setMax_number_source_per_cell_external(int number_source_per_cell_arg);
    void setCell_Labeling_Percentage_necrosis(double cell_labeling_percentage_arg);
    void setCell_Labeling_Percentage_intermediary(double cell_labeling_percentage_arg);
    void setCell_Labeling_Percentage_external(double cell_labeling_percentage_arg);

    int number_particles_per_source() const;
    void setNumber_particles_per_source(int number_particles_per_source);

    int number_source_necrosis() const;
    void setNumber_source_necrosis(int number_source_necrosis);

    int number_source_intermediary() const;
    void setNumber_source_intermediary(int number_source_intermediary);

    int number_source_external() const;
    void setNumber_source_external(int number_source_external);

    void setOrganelle_weight(double pCellMembrane, double pNucleoplasm, double pNuclearMembrane, double pCytoplasm );

    std::vector<double> getOrganelle_weight();

    DistributedSourceMessenger& messenger();

    int total_particle() const { return number_source_*number_particles_per_source_;}

    void eraseFront_position_cell()
    {
      ((current_cell_->second).position_in_cell_).erase(((current_cell_->second).position_in_cell_).begin());
    }

    vector<const Settings::nCell::t_Cell_3 *> chooseLabeledCells(float cell_labeling_percentage,
                                                                              const SpheroidRegion &region);

    vector<int> applyMethodDistributionNbParticlesInCells(vector<const Settings::nCell::t_Cell_3 *> labeled_cells,
                                                    int number_source,
                                                    int max_number_source_per_cell);

    vector<const Settings::nCell::t_Cell_3 *> labeled_cells;

    /// \brief particle source repartition in each cell
    std::unordered_map<const Settings::nCell::t_Cell_3*, SourceInfo> cell_source_;

    double emission_in_membrane_;
    double emission_in_nucleus_;
    double emission_in_nucleus_membrane_;
    double emission_cytoplasm_;

    int only_one_position_for_all_particles_on_a_cell = 0;

    bool is_log_norm_distribution = false;
    int mean_ppc;
    float shape_factor;

    std::vector<double> organelle_weight_vector;

    vector<int> max_nb_part_per_cell;

    vector<int> inverse_cdf_log_normal_distribution(const vector<float>& uniform_numbers,
       float shape_param, float mean_output_value);

    int nb_elements_array(const vector<int>& u)
    {
      int nb_elements = 0;
      for (float i : u)
      {
        nb_elements += 1;
      }
      return nb_elements;
    }

    double sum_array(const vector<int>& u)
    {
      int sum = 0;
      for (float i : u)
      {
        sum += i;
      }
      return static_cast<double>(sum);
    }

private:
    bool is_initialized_ = false;

    /// \brief Return the number of particle source to distribute in region
    int source_in_region(const SpheroidRegion& region) const;

    /// \brief Distribute number_source particle sources inside region and update the unordered map
    void distribute(int number_source, const SpheroidRegion& region);

    /// \brief Distribute number_source particle sources inside cells, with a
    /// maximum number per source and update the unordered map
    void distribute_in_cells_with_maximum_nb(int total_nb_source,
                                            int* nb_source_per_cell,
                                            vector<const Settings::nCell::t_Cell_3 *> labeled_cells,
                                            vector<int> max_nb_part_per_cell);

    /// \brief Return a position in the cell in G4 unit according to the organelle_weight probablity
    std::vector<G4ThreeVector> getPositionInCell() const;

    /// \brief Number of particle sources to be simulated
    int number_source_ = 0;
    /// \brief Maximum number of source per cell to be generated, in necrosis region
    int max_number_source_per_cell_necrosis = 0;
    /// \brief Maximum number of source per cell to be generated, in intermediary region
    int max_number_source_per_cell_intermediary = 0;
    /// \brief Maximum number of source per cell to be generated, in external region
    int max_number_source_per_cell_external = 0;
    /// \brief Number of particles to generate from one source
    int number_particles_per_source_ = 0;
    /// \brief Number of particle source in the necrosis region
    int number_source_necrosis_ = 0;
    /// \brief Number of particle source in the intermediary region
    int number_source_intermediary_ = 0;
    /// \brief Number of particle source in the external region
    int number_source_external_ = 0;
    /// \brief Current cell generating particle source
    std::unordered_map<const Settings::nCell::t_Cell_3*, SourceInfo>::iterator current_cell_;
    /// \brief Secondary distribution in a cell
    std::unique_ptr<OrganellesWeight> organelle_weight_;
    /// \brief Cell labeling percentage, in necrosis region
    double cell_labeling_percentage_necrosis_;
    /// \brief Cell labeling percentage, in intermediary region
    double cell_labeling_percentage_intermediary_;
    /// \brief Cell labeling percentage, in external region
    double cell_labeling_percentage_external_;

    /// \brief Messenger
    std::unique_ptr<DistributedSourceMessenger> messenger_;
};

}



#endif // DISTRIBUTEDSOURCE_HH
