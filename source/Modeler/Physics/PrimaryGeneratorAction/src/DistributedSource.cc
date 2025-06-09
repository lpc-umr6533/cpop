#include "DistributedSource.hh"

#include "Population.hh"
#include "RandomEngineManager.hh"
#include "Randomize.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "analysis.hh"

#include <cstdio>
#include <cstdlib>

#include <iostream>
#include <vector>
#include <cmath>


namespace cpop {

DistributedSource::DistributedSource(const std::string &name, const Population &population):
	Source(name,population),
	_messenger(std::make_unique<DistributedSourceMessenger>(this))
{
}

std::vector<G4ThreeVector> DistributedSource::GetPosition() {
	// If this method is called, we know we have something to generate
	// because a source is only selected if HasLeft() returned true
	return getPositionInCell();
}

void DistributedSource::Update() {
	_currentCell->second.Update();
	// Check if we need to go to the next cell
	if(!_currentCell->second.HasLeft())
		++_currentCell;
}

bool DistributedSource::HasLeft() {
	if (!_isInitialized) return false;
	return _currentCell != cellSource.end();
}

void DistributedSource::Initialize() {
	if(!_isInitialized) {
		auto const* population = DistributedSource::population();
		auto const& regions = population->regions();

		int number_source = 0;
		for(const SpheroidRegion& region : regions) {
			if (population->verbose_level() > 0)
				std::cout << "Distributing sources in region : " << region.name() << std::endl;
			number_source = source_in_region(region);
			distribute(number_source, region);
		}

		_currentCell = cellSource.begin();
		_isInitialized = true;
		std::cout << "In initialized of is_initialized : " << std::boolalpha << _isInitialized << '\n';
	}
}

int DistributedSource::source_in_region(const SpheroidRegion &region) const {
	int res = 0;
	if (region.name() == "Necrosis")
		res = _numberSourceNecrosis;
	else if (region.name() == "Intermediary")
		res = _numberSourceIntermediary;
	else if (region.name() == "External")
		res = _numberSourceExternal;

	return res;
}

void DistributedSource::distribute(int number_source, const SpheroidRegion &region) {
	auto const& cells_in_region = region.cells_in_region();
	int cells_in_region_size = cells_in_region.size();

	int max_number_source_per_cell = 1000000; //In case of no below corresponding region name
	double cell_labeling_percentage = 100.;

	if (region.name() == "Necrosis") {
		max_number_source_per_cell = _maxNumberSourcePerCellNecrosis;
		cell_labeling_percentage = _cellLabelingPercentageNecrosis;
	} else if (region.name() == "Intermediary") {
		max_number_source_per_cell = _maxNumberSourcePerCellIntermediary;
		cell_labeling_percentage = _cellLabelingPercentageIntermediary;
	} else if (region.name() == "External") {
		max_number_source_per_cell = _maxNumberSourcePerCellExternal;
		cell_labeling_percentage = _cellLabelingPercentageExternal;
	}

	if (cell_labeling_percentage == 0) return;

	labeledCells = chooseLabeledCells(cell_labeling_percentage, region);

	if(max_number_source_per_cell*cells_in_region.size()*cell_labeling_percentage < number_source) {
		throw std::invalid_argument("Number of particles > Max number of particles per cell * Number of cells labeled.");
	}

	int nb_source_per_cell[cells_in_region_size];
	memset(nb_source_per_cell, 0, cells_in_region_size*sizeof(int));

	std::vector<float> randomNumbers(cells_in_region_size);

	maxNbPartPerCell = applyMethodDistributionNbParticlesInCells(
		labeledCells,
		number_source, max_number_source_per_cell
	);

	distribute_in_cells_with_maximum_nb(number_source, nb_source_per_cell, labeledCells, maxNbPartPerCell);

	std::cout << " Number of labeled cells in region " << region.name() << " = " << labeledCells.size()  <<'\n';

	if (cells_in_region_size!=0) {
		std::cout << " Labeled cells percentage in region " << region.name() << " = " << (static_cast<double>(labeledCells.size())/cells_in_region_size)*100 << "%" <<'\n';
	} else {
		std::cout << " Zero cells in region " << region.name()  <<'\n';
	}
}

void DistributedSource::distribute_in_cells_with_maximum_nb(
	int total_nb_source,
	int* nb_source_per_cell,
	std::vector<const Settings::nCell::t_Cell_3*> labeled_cells,
	std::vector<int> max_nb_part_per_cell
)
{
	int indexCell = 0;
	for(int i = 0 ; i < total_nb_source; ++i) {
		// TODO bad rand
		indexCell = rand() % (labeled_cells.size());
		while (nb_source_per_cell[indexCell]>=max_nb_part_per_cell[indexCell]) {
			indexCell = rand() % (labeled_cells.size());
		}

		const Settings::nCell::t_Cell_3 * selected_cell = labeled_cells[indexCell];

		auto already_selected = cellSource.find(selected_cell);
		if (already_selected == cellSource.end()) {
			if (population()->verbose_level() > 0)
					std::cout << "  Inserting particle source in cell with id " << selected_cell->getID()  <<'\n';

			cellSource.insert({selected_cell, {selected_cell, 1, _numberParticlesPerSource, *_organelleWeight}});
			nb_source_per_cell[indexCell] +=1 ;
		} else {
			if (this->population()->verbose_level() > 0)
				std::cout << "  Adding particle source to cell with id  " << selected_cell->getID() << '\n';
			already_selected->second.addDistributedSource();
			if (onlyOnePositionForAllParticlesOnACell == 0) {
				already_selected->second.AddParticlePosition(*_organelleWeight);
			}
			nb_source_per_cell[indexCell] += 1;
		}
	}
}

std::vector<G4ThreeVector> DistributedSource::getPositionInCell() const {
	return _currentCell->second.GetPosition();
}

int DistributedSource::getID_OfCell() {
  return _currentCell->second.getID_SourceInfo();
}

int DistributedSource::number_source_external() const {
	return _numberSourceExternal;
}

void DistributedSource::setNumber_source_external(int number_source_external) {
	_numberSourceExternal = number_source_external;
}

void DistributedSource::setOrganelle_weight(double pCellMembrane, double pNucleoplasm, double pNuclearMembrane, double pCytoplasm) {
	_organelleWeight = std::make_unique<OrganellesWeight>(pCellMembrane, pNucleoplasm, pNuclearMembrane, pCytoplasm);
}

std::vector<double> DistributedSource::getOrganelle_weight() {
  organelleWeightVector = {emissionInMembrane, emissionInNucleus, emissionInNucleusMembrane, emissionCytoplasm};
  return organelleWeightVector;
}

DistributedSourceMessenger &DistributedSource::messenger() {
	return *_messenger;
}

int DistributedSource::number_source_intermediary() const {
	return _numberSourceIntermediary;
}

void DistributedSource::setNumber_source_intermediary(int number_source_intermediary) {
	_numberSourceIntermediary = number_source_intermediary;
}

int DistributedSource::number_source_necrosis() const {
	return _numberSourceNecrosis;
}

void DistributedSource::setNumber_source_necrosis(int number_source_necrosis) {
	_numberSourceNecrosis = number_source_necrosis;
}

int DistributedSource::number_particles_per_source() const {
	return _numberParticlesPerSource;
}

void DistributedSource::setNumber_particles_per_source(int number_particles) {
	_numberParticlesPerSource = number_particles;
}

int DistributedSource::number_distributed() const {
	return _numberSource;
}

void DistributedSource::setNumber_source(int number_source) {
	_numberSource = number_source;
}

void DistributedSource::setMax_number_source_per_cell_necrosis(int number_source_per_cell_arg) {
	_maxNumberSourcePerCellNecrosis = number_source_per_cell_arg;
}

void DistributedSource::setMax_number_source_per_cell_intermediary(int number_source_per_cell_arg) {
	_maxNumberSourcePerCellIntermediary = number_source_per_cell_arg;
}

void DistributedSource::setMax_number_source_per_cell_external(int number_source_per_cell_arg) {
	_maxNumberSourcePerCellExternal = number_source_per_cell_arg;
}

void DistributedSource::setCell_Labeling_Percentage_necrosis(double cell_labeling_percentage_arg) {
  _cellLabelingPercentageNecrosis = cell_labeling_percentage_arg/100;
}

void DistributedSource::setCell_Labeling_Percentage_intermediary(double cell_labeling_percentage_arg) {
  _cellLabelingPercentageIntermediary = cell_labeling_percentage_arg/100;
}

void DistributedSource::setCell_Labeling_Percentage_external(double cell_labeling_percentage_arg) {
  _cellLabelingPercentageExternal = cell_labeling_percentage_arg/100;
}

std::vector<int> DistributedSource::inverse_cdf_log_normal_distribution(
	const std::vector<float>& uniform_numbers, float shape_param, float mean_output_value
) {
	std::vector<int> results;
	float epsilon = std::numeric_limits<float>::epsilon();

	for (float i : uniform_numbers) {
		float _arg_erf_inv = 2 * i - 1;
		if (_arg_erf_inv >= 1.0)
			_arg_erf_inv = 1.0 - epsilon;

		if (_arg_erf_inv <= -1.0)
			_arg_erf_inv = -1.0 + epsilon;

		float result = exp(
			boost::math::erf_inv(_arg_erf_inv) * shape_param * sqrt(2) +
			log(mean_output_value) - 0.5 * pow(shape_param, 2)
		);
		results.push_back(round(result));
	}
	return results;
}

std::vector<const Settings::nCell::t_Cell_3 *> DistributedSource::chooseLabeledCells(
	float cell_labeling_percentage, const SpheroidRegion &region
) {
  if ((cell_labeling_percentage != 1.0) and (cell_labeling_percentage != 0.0)) {
    int cells_in_region_size = region.cells_in_region().size();
		std::vector<const Settings::nCell::t_Cell_3*> labeled_cells = {};
		std::vector<int> available_cells_id(cells_in_region_size);
    iota(available_cells_id.begin(), available_cells_id.end(), 1); //contains 1 to available_cells_id.size()
    for(int i = 0 ; i < int(cell_labeling_percentage * cells_in_region_size); ++i) {
			int indexCell = available_cells_id[rand() % available_cells_id.size()] - 1;
			labeled_cells.push_back(region.cells_in_region()[indexCell]);
			remove(available_cells_id.begin(),available_cells_id.end(),indexCell);
		}

    return labeled_cells;
  }

  return region.cells_in_region();
}

std::vector<int> DistributedSource::applyMethodDistributionNbParticlesInCells(
	std::vector<const Settings::nCell::t_Cell_3 *> labeled_cells, int number_source, int max_number_source_per_cell
) {
  std::random_device rd;  // Will be used to obtain a seed for the random number engine
  std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> uniform_distribution(0.0, 1.0);
  int labeled_cells_size = labeled_cells.size(); //Should be equal to labeling_percentage * nb_cells

  if (isLogNormDistribution) {
		std::vector<int> log_norm_distrib_particles;
		std::vector<float> randomNumbers(labeled_cells_size);
		for (int i = 0; i < labeled_cells_size; ++i)
			randomNumbers[i] = uniform_distribution(gen);

		log_norm_distrib_particles = inverse_cdf_log_normal_distribution(randomNumbers, shapeFactor, meanPPC);
		while ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) != meanPPC) {
			for (int i = 0; i < labeled_cells_size; ++i)
				randomNumbers[i] = uniform_distribution(gen);
			log_norm_distrib_particles = inverse_cdf_log_normal_distribution(randomNumbers, shapeFactor, meanPPC);
		}

		if ((sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * labeled_cells_size != number_source) {
			std::stringstream error_msg;
			error_msg << "Expected number of event = MeanNbPartPerCell * NbCells \n";
			error_msg << "Change number of events to" << ": " << (sum_array(log_norm_distrib_particles)/nb_elements_array(log_norm_distrib_particles)) * labeled_cells_size << '\n';
			throw std::runtime_error(error_msg.str());
		}

		return log_norm_distrib_particles;
	} else {
		std::vector<int> uniform_distrib_particles = std::vector<int>(labeled_cells_size, max_number_source_per_cell);
		return uniform_distrib_particles;
	}
}

}
