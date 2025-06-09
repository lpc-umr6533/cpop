#ifndef DISTRIBUTEDSOURCE_HH
#define DISTRIBUTEDSOURCE_HH

#include <memory>
#include <numeric>
#include <unordered_map>

#include "CGAL_Utils.hh"
#include "Source.hh"
#include "CellSettings.hh"
#include "OrganellesWeight.hh"
#include "DistributedSourceMessenger.hh"

#include <boost/math/special_functions/erf.hpp>
#include <cmath>
#include <vector>

#include "RandomEngineManager.hh"
#include "Randomize.hh"

namespace cpop {

class SpheroidRegion;

/// \brief This class is used to store particle source information for each cell
class SourceInfo {
/// Victor Levrague : functions to allow random positions for each different particle generated on a cell ///

public:
	SourceInfo(const Settings::nCell::t_Cell_3* cell, int nb_source, int part_per_source, const OrganellesWeight& organelle_weight):
		_cell(cell),
		_numberSource(nb_source),
		_numberParticlesPerSource(part_per_source)
	{
		// Pick a random organelle
		auto organelle = organelle_weight.getRandomOrganelle();

		Point_3 pos = Utils::myCGAL::to_G4(_cell->getSpotOnOrganelle(organelle));
		_positionInCell.emplace_back(pos.x(), pos.y(), pos.z());
	}

	[[nodiscard]] bool HasLeft() const { return _alreadyGenerated < totalSecondary(); }
	void addDistributedSource(int inc = 1) { _numberSource += inc; }
	void AddParticlePosition(OrganellesWeight& organelle_weight) {
		auto organelle = organelle_weight.getRandomOrganelle();

		Point_3 pos = Utils::myCGAL::to_G4(_cell->getSpotOnOrganelle(organelle));

		_positionInCell.emplace_back(pos.x(), pos.y(), pos.z());
	}

	void Update() { ++_alreadyGenerated; }
	int getID_SourceInfo() { return _cell->getID(); }

	[[nodiscard]] std::vector<G4ThreeVector> GetPosition() const {
		// G4cout<< "GetPosition called in NanoSource.hh" << G4endl;
		// G4cout<< "position in cell x : " << position_in_cell_[0].getX() << G4endl;
		// G4cout<< "position in cell y : " << position_in_cell_[0].getY() << G4endl;
		// G4cout<< "position in cell z: " << position_in_cell_[0].getZ() << G4endl;
		return _positionInCell;
	}

	/// \brief Position in cell
	std::vector<G4ThreeVector> _positionInCell;

private:
    [[nodiscard]] int totalSecondary() const { return _numberSource*_numberParticlesPerSource; }

    /// \brief Cell containing particle sources
    const Settings::nCell::t_Cell_3* _cell;
    /// \brief Number of source in the cell
    int _numberSource = 0;
    /// \brief Number of secondary particle to generate for one source
    int _numberParticlesPerSource = 0;
    /// \brief Number of secondary particle already generated
    int _alreadyGenerated = 0;
};

class DistributedSource : public Source {
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

	void setOrganelle_weight(double pCellMembrane, double pNucleoplasm, double pNuclearMembrane, double pCytoplasm);

	std::vector<double> getOrganelle_weight();

	DistributedSourceMessenger& messenger();

	int total_particle() const { return _numberSource*_numberParticlesPerSource; }

	void eraseFront_position_cell() {
		((_currentCell->second)._positionInCell).erase(((_currentCell->second)._positionInCell).begin());
	}

	std::vector<const Settings::nCell::t_Cell_3*> chooseLabeledCells(
		float cell_labeling_percentage, const SpheroidRegion &region
	);

	std::vector<int> applyMethodDistributionNbParticlesInCells(
		std::vector<const Settings::nCell::t_Cell_3*> labeled_cells,
		int number_source,
		int max_number_source_per_cell
	);

	std::vector<const Settings::nCell::t_Cell_3 *> labeledCells;

	/// \brief particle source repartition in each cell
	std::unordered_map<const Settings::nCell::t_Cell_3*, SourceInfo> cellSource;

	double emissionInMembrane;
	double emissionInNucleus;
	double emissionInNucleusMembrane;
	double emissionCytoplasm;

	int onlyOnePositionForAllParticlesOnACell = 0;

	bool isLogNormDistribution = false;
	int meanPPC;
	float shapeFactor;

	std::vector<double> organelleWeightVector;

	std::vector<int> maxNbPartPerCell;

	std::vector<int> inverse_cdf_log_normal_distribution(
		const std::vector<float>& uniform_numbers,
		float shape_param, float mean_output_value
	);

	int nb_elements_array(const std::vector<int>& u) {
		// TODO return type
		return static_cast<int>(u.size());
	}

	double sum_array(const std::vector<int>& u) {
		return std::accumulate(std::begin(u), std::end(u), 0);
	}

private:
	bool _isInitialized = false;

	/// \brief Return the number of particle source to distribute in region
	int source_in_region(const SpheroidRegion& region) const;

	/// \brief Distribute number_source particle sources inside region and update the unordered map
	void distribute(int number_source, const SpheroidRegion& region);

	/// \brief Distribute number_source particle sources inside cells, with a
	/// maximum number per source and update the unordered map
	void distribute_in_cells_with_maximum_nb(
		int total_nb_source,
		int* nb_source_per_cell,
		std::vector<const Settings::nCell::t_Cell_3*> labeled_cells,
		std::vector<int> max_nb_part_per_cell
	);

	/// \brief Return a position in the cell in G4 unit according to the organelle_weight probablity
	std::vector<G4ThreeVector> getPositionInCell() const;

	/// \brief Number of particle sources to be simulated
	int _numberSource = 0;
	/// \brief Maximum number of source per cell to be generated, in necrosis region
	int _maxNumberSourcePerCellNecrosis = 0;
	/// \brief Maximum number of source per cell to be generated, in intermediary region
	int _maxNumberSourcePerCellIntermediary = 0;
	/// \brief Maximum number of source per cell to be generated, in external region
	int _maxNumberSourcePerCellExternal = 0;
	/// \brief Number of particles to generate from one source
	int _numberParticlesPerSource = 0;
	/// \brief Number of particle source in the necrosis region
	int _numberSourceNecrosis = 0;
	/// \brief Number of particle source in the intermediary region
	int _numberSourceIntermediary = 0;
	/// \brief Number of particle source in the external region
	int _numberSourceExternal = 0;
	/// \brief Current cell generating particle source
	std::unordered_map<const Settings::nCell::t_Cell_3*, SourceInfo>::iterator _currentCell;
	/// \brief Secondary distribution in a cell
	std::unique_ptr<OrganellesWeight> _organelleWeight;
	/// \brief Cell labeling percentage, in necrosis region
	double _cellLabelingPercentageNecrosis;
	/// \brief Cell labeling percentage, in intermediary region
	double _cellLabelingPercentageIntermediary;
	/// \brief Cell labeling percentage, in external region
	double _cellLabelingPercentageExternal;

	/// \brief Messenger
	std::unique_ptr<DistributedSourceMessenger> _messenger;
};

}

#endif
