#include "DistributedSource.hh"

#include "Population.hh"
#include "RandomEngineManager.hh"
#include "Randomize.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "analysis.hh"

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>
#include <cmath>


namespace cpop {

DistributedSource::DistributedSource(const string &name, const Population &population)
    :Source(name,population),
       messenger_(std::make_unique<DistributedSourceMessenger>(this))
{
}

std::vector<G4ThreeVector> DistributedSource::GetPosition()
{
    // If this method is called, we know we have something to generate
    // because a source is only selected if HasLeft() returned true
    return getPositionInCell();
}

void DistributedSource::Update()
{
    current_cell_->second.Update();
    // Check if we need to go to the next cell
    if(!current_cell_->second.HasLeft())
        ++current_cell_;
}

bool DistributedSource::HasLeft()
{
    if (!is_initialized_) return false;

    return current_cell_ != cell_nano_.end();
}

void DistributedSource::Initialize()
{
    if(!is_initialized_) {
        const Population* population = this->population();
        std::vector<SpheroidRegion> regions = population->regions();

        int number_nano = 0;
        for(const SpheroidRegion& region : regions) {
            if (population->verbose_level() > 0)
                std::cout << "Distributing sources in region : " << region.name() << std::endl;
            number_nano = nanoparticle_in_region(region);
            distribute(number_nano, region);
        }

        current_cell_ = cell_nano_.begin();
        is_initialized_ = true;
        std::cout << "In initialized of is_initialized : " << std::boolalpha << is_initialized_ << '\n';
    }
}

int DistributedSource::nanoparticle_in_region(const SpheroidRegion &region) const
{
    int res = 0;
    if (region.name() == "Necrosis")
        res = number_nanoparticle_necrosis_;
    else if (region.name() == "Intermediary")
        res = number_nanoparticle_intermediary_;
    else if (region.name() == "External")
        res = number_nanoparticle_external_;

    return res;
}

void DistributedSource::distribute(int number_nano, const SpheroidRegion &region)
{

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 1.0);

    std::vector<const Settings::nCell::t_Cell_3 *> cells_in_region = region.cells_in_region();

    int cells_in_region_size = cells_in_region.size();

    std::vector<int> labeled_cells_id;

    int max_number_nanoparticle_per_cell = 1000000;
    double cell_labeling_percentage;

    if (region.name() == "Necrosis")
      {max_number_nanoparticle_per_cell = max_number_nanoparticle_per_cell_necrosis;
       cell_labeling_percentage = cell_labeling_percentage_necrosis_;}
    if (region.name() == "Intermediary")
      {max_number_nanoparticle_per_cell = max_number_nanoparticle_per_cell_intermediary;
       cell_labeling_percentage = cell_labeling_percentage_intermediary_;}
    if (region.name() == "External")
      {max_number_nanoparticle_per_cell = max_number_nanoparticle_per_cell_external;
       cell_labeling_percentage = cell_labeling_percentage_external_;}

   G4cout << "Max number nano per cell : " << max_number_nanoparticle_per_cell << G4endl;

   if(max_number_nanoparticle_per_cell*cells_in_region.size() < number_nano) {
        throw std::invalid_argument("Number of particles > Max number of particles per cell * Number of cells."
                        "Maybe macro command is missing : /cpop/source/gadolinium/maxSourcesPerCell");
    }

   int nb_nano_per_cell[cells_in_region_size];
   memset(nb_nano_per_cell, 0, cells_in_region_size*sizeof(int));

   int max_nb_nano_per_cell[cells_in_region_size];

   std::vector<float> randomNumbers(cells_in_region_size);

   if (cells_in_region_size > 0)
   {
     if (is_log_norm_distribution)
        {
          G4cout << "In log normal" << G4endl;
          for (int i = 0; i < cells_in_region_size; ++i)
          {
             randomNumbers[i] = dis(gen);
          }

          vector<int> log_norm_distrib_particles = inverse_cdf_log_normal_distribution(randomNumbers, shape_factor, mean_ppc);
          while ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) != mean_ppc)
           {
             for (int i = 0; i < cells_in_region_size; ++i)
             {
                randomNumbers[i] = dis(gen);
             }
             log_norm_distrib_particles = inverse_cdf_log_normal_distribution(randomNumbers, shape_factor, mean_ppc);
           }

           if ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * cells_in_region_size != number_nano)
           {
               std::stringstream error_msg;
               error_msg << "Expected number of event = MeanNbPartPerCell * NbCells \n";
               error_msg << "Change number of events to" << ": " << (sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * cells_in_region_size << '\n';
               throw std::runtime_error(error_msg.str());
           }
          std::copy(log_norm_distrib_particles.begin(), log_norm_distrib_particles.end(), max_nb_nano_per_cell);
        }
     else
        {memset(max_nb_nano_per_cell, max_number_nanoparticle_per_cell,
               cells_in_region_size*sizeof(int));}
    }


  //  if (cells_in_region_size > 0)
  //  {
  //    while ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) != mean_ppc)
  //     {
  //       for (int i = 0; i < cells_in_region_size; ++i)
  //       {
  //          randomNumbers[i] = dis(gen);
  //       }
  //       log_norm_distrib_particles = inverse_cdf_log_normal_distribution(randomNumbers, shape_factor, mean_ppc);
  //     }
  //
  //     G4cout << "test 2: " << ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * cells_in_region_size == number_nano) << G4endl;
  //
  //     if ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * cells_in_region_size != number_nano) {
  //         std::stringstream error_msg;
  //
  //         error_msg << "Expected number of event = MeanNbPartPerCell * NbCells \n";
  //         error_msg << "Change number of events to" << ": " << (sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * cells_in_region_size << '\n';
  //
  //         throw std::runtime_error(error_msg.str());
  //     }
  //
  // }

   // Particules are distributed on cells following :
   // - the % of labeled cells
   // - the max number of particules per cell

    int indexCell = 0;
    for(int i = 0 ; i < number_nano; ++i)
    {
      if((labeled_cells_id.size())<(cell_labeling_percentage*cells_in_region_size))
      {
        //Labeled cells are chosen randomly in the region
        indexCell = RandomEngineManager::getInstance()->randi(0, cells_in_region_size -1);

        while (nb_nano_per_cell[indexCell]>=max_nb_nano_per_cell[indexCell])
         {indexCell = RandomEngineManager::getInstance()->randi(0, cells_in_region_size -1);}

        labeled_cells_id.push_back(indexCell);

        const Settings::nCell::t_Cell_3 * selected_cell = cells_in_region[indexCell];

        auto already_selected = cell_nano_.find(selected_cell);
        if (already_selected == cell_nano_.end())
        {
          if (this->population()->verbose_level() > 0)
              std::cout << " Inserting nanoparticle in cell with id " << selected_cell->getID()  <<'\n';

          cell_nano_.insert({selected_cell, {selected_cell, 1, number_particles_per_source_, *organelle_weight_} });
          nb_nano_per_cell[indexCell] +=1 ;
        }
        else
        {
          if (this->population()->verbose_level() > 0)
              std::cout << "  Adding nanoparticle to cell with id  " << selected_cell->getID() << '\n';
          already_selected->second.addDistributedSource();
          if (only_one_position_for_all_particles_on_a_cell == 0)
          {already_selected->second.AddParticlePosition(*organelle_weight_);}
          nb_nano_per_cell[indexCell] +=1 ;
        }
      }
      else
      {
        //When all cells are labeled, particles are added onmy in those cells
        indexCell = rand() % (labeled_cells_id.size());

        while (nb_nano_per_cell[indexCell]>=max_nb_nano_per_cell[indexCell])
         {indexCell = rand() % (labeled_cells_id.size());}

        const Settings::nCell::t_Cell_3 * selected_cell = cells_in_region[indexCell];

        auto already_selected = cell_nano_.find(selected_cell);
        if (already_selected == cell_nano_.end())
        {
          if (this->population()->verbose_level() > 0)
              std::cout << "  Inserting nanoparticle in cell with id " << selected_cell->getID()  <<'\n';

          cell_nano_.insert({selected_cell, {selected_cell, 1, number_particles_per_source_, *organelle_weight_} });
          nb_nano_per_cell[indexCell] +=1 ;
        }
        else
        {
          if (this->population()->verbose_level() > 0)
              std::cout << "  Adding nanoparticle to cell with id  " << selected_cell->getID() << '\n';
          already_selected->second.addDistributedSource();
          if (only_one_position_for_all_particles_on_a_cell == 0)
          {already_selected->second.AddParticlePosition(*organelle_weight_);}
          nb_nano_per_cell[indexCell] +=1 ;
        }

      }

    }

    std::cout << " Number of labeled cells in region " << region.name() << " = " << labeled_cells_id.size()  <<'\n';

    double cells_in_region_size_double = cells_in_region_size;
    double labeled_cells_size_double = labeled_cells_id.size();
    if (cells_in_region_size!=0)
    {std::cout << " Labeled cells percentage in region " << region.name() << " = " << (labeled_cells_size_double/cells_in_region_size_double)*100 << "%" <<'\n';}
    else
    {std::cout << " Zero cells in region " << region.name()  <<'\n';}

}

std::vector<G4ThreeVector> DistributedSource::getPositionInCell() const
{
    return (current_cell_->second.GetPosition());
}

int DistributedSource::getID_OfCell()
{
  return current_cell_->second.getID_NanoInfo();
}




int DistributedSource::number_nanoparticle_external() const
{
    return number_nanoparticle_external_;
}

void DistributedSource::setNumber_source_external(int number_nanoparticle_external)
{
    number_nanoparticle_external_ = number_nanoparticle_external;
}

void DistributedSource::setOrganelle_weight(double pCellMembrane, double pNucleoplasm, double pNuclearMembrane, double pCytoplasm)
{
    organelle_weight_ = std::make_unique<OrganellesWeight>(pCellMembrane, pNucleoplasm, pNuclearMembrane, pCytoplasm);
}

std::vector<double> DistributedSource::getOrganelle_weight()
{
  organelle_weight_vector = {emission_in_membrane_, emission_in_nucleus_, emission_in_nucleus_membrane_, emission_cytoplasm_};
  return organelle_weight_vector;
}

DistributedSourceMessenger &DistributedSource::messenger()
{
    return *messenger_;
}

int DistributedSource::number_nanoparticle_intermediary() const
{
    return number_nanoparticle_intermediary_;
}

void DistributedSource::setNumber_source_intermediary(int number_nanoparticle_intermediary)
{
    number_nanoparticle_intermediary_ = number_nanoparticle_intermediary;
}

int DistributedSource::number_nanoparticle_necrosis() const
{
    return number_nanoparticle_necrosis_;
}

void DistributedSource::setNumber_source_necrosis(int number_nanoparticle_necrosis)
{
    number_nanoparticle_necrosis_ = number_nanoparticle_necrosis;
}

int DistributedSource::number_particles_per_source() const
{
    return number_particles_per_source_;
}

void DistributedSource::setNumber_particles_per_source(int number_particles)
{
    number_particles_per_source_ = number_particles;
}

int DistributedSource::number_distributed() const
{
    return number_nanoparticle_;
}

void DistributedSource::setNumber_source(int number_nanoparticle)
{
    number_nanoparticle_ = number_nanoparticle;
}

void DistributedSource::setMax_number_source_per_cell_necrosis(int number_nanoparticle_per_cell_arg)
{
    max_number_nanoparticle_per_cell_necrosis = number_nanoparticle_per_cell_arg;
}

void DistributedSource::setMax_number_source_per_cell_intermediary(int number_nanoparticle_per_cell_arg)
{
    max_number_nanoparticle_per_cell_intermediary = number_nanoparticle_per_cell_arg;
}

void DistributedSource::setMax_number_source_per_cell_external(int number_nanoparticle_per_cell_arg)
{
    max_number_nanoparticle_per_cell_external = number_nanoparticle_per_cell_arg;
}

void DistributedSource::setCell_Labeling_Percentage_necrosis(double cell_labeling_percentage_arg)
{
  cell_labeling_percentage_necrosis_ = cell_labeling_percentage_arg/100;
}

void DistributedSource::setCell_Labeling_Percentage_intermediary(double cell_labeling_percentage_arg)
{
  cell_labeling_percentage_intermediary_ = cell_labeling_percentage_arg/100;

}

void DistributedSource::setCell_Labeling_Percentage_external(double cell_labeling_percentage_arg)
{
  cell_labeling_percentage_external_ = cell_labeling_percentage_arg/100;
}

vector<int> DistributedSource::inverse_cdf_log_normal_distribution(const vector<float>& u,
                                                float shape_param, float mean_output_value)
{
    vector<int> results;

    for (float i : u)
    {
        float result = exp(boost::math::erf_inv(2 * i - 1) * shape_param * sqrt(2) +
                                log(mean_output_value) - 0.5 * pow(shape_param, 2));
        results.push_back(round(result));
    }
    return results;
}


}
