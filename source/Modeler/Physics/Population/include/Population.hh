#ifndef POPULATION_HH
#define POPULATION_HH

#include <CLHEP/Random/MTwistEngine.h>
#include <ctime>
#include <vector>
#include <memory>

#include "UnitSystemManager.hh"
#include "Mesh3DSettings.hh"
#include "CellSettings.hh"
#include "SpheroidRegion.hh"

#include "PopulationMessenger.hh"

namespace cpop {

class Population {
public:
	Population();
	~Population();

	int verbose_level() const;
	void setVerbose_level(double verbose_level);

	double spheroid_radius() const;
	void setSpheroid_radius(double spheroid_radius);

	Settings::Geometry::Point_3 spheroid_centroid() const;
	void setSpheroid_centroid(const Settings::Geometry::Point_3 &spheroid_centroid);

	unsigned int number_max_facet_poly() const;
	void setNumber_max_facet_poly(unsigned int number_max_facet_poly);

	double delta_reffinement() const;
	void setDelta_reffinement(double delta_reffinement);

	std::string population_file() const;
	void setPopulation_file(const std::string &population_file);

	const std::vector<const Settings::nCell::t_Cell_3 *>& cells() const;

	double internal_layer_ratio() const;
	void setInternal_layer_ratio(double internal_layer_ratio);

	double intermediary_layer_ratio() const;
	void setIntermediary_layer_ratio(double intermediary_layer_ratio);

	double number_sampling_cell_per_region() const;
	void setNumber_sampling_cell_per_region(double value);

	void loadPopulation();
	void printPopulationInfo();

	void defineRegion();
	void printRegionInfo();

	G4int calculateNumberOfCells_InXML_File();

	void enableWritingInfoPrimariesTxt(G4String choice, G4String name_file);

	void set_Stepping_level_info_bool(int stepping_level_info_arg);
	void set_Event_level_info_bool(int event_level_info_arg);

	const std::vector<const Settings::nCell::t_Cell_3 *>& sampled_cells() const;

	PopulationMessenger& messenger();

	std::vector<SpheroidRegion> regions() const;

	G4int nbCellXml = 0;

	/// brief Spheroid radius of the cell population in G4 unit
	double spheroidRadius = 0;

	G4int steppingLevelInfo = 1;
	G4int eventLevelInfo = 0;
	bool writeInfoPrimariesTxt = false;
	bool usePositionsDirectionsFile = false;

	G4String nameFilePrimaries;

	// TODO mutable!
	mutable std::vector<const Settings::nCell::t_Cell_3 *> labeledCells;

	void set_labeled_cells(std::vector<const Settings::nCell::t_Cell_3 *> vector_labeled) const {
		labeledCells = vector_labeled;
	}

private:
	//Conversion helpers
	/// \brief Convert factor between CPOP default unit and G4 default unit
	double conversionFrmCPOPToG4 = UnitSystemManager::getInstance()->getConversionToG4();
	/// \brief Convert factor between G4 default unit and CPOP default unit
	double conversionFrmG4ToCPOP = UnitSystemManager::getInstance()->getConversionFromG4();

	/// \brief Default is 0 (no terminal output). Set to 1 to print informations at the beginning of the simulation
	int _verboseLevel = 0;

	// Cell population
	/// \brief Cell mesh
	Settings::Geometry::Mesh3D::t_Mesh_3* _voronoiMesh = nullptr;
	/// \brief Cells generated from the mesh
	std::vector<const Settings::nCell::t_Cell_3*> _cells;
	/// \brief Center of the cell population spheroid in G4 unit
	Settings::Geometry::Point_3 _spheroidCentroid = Settings::Geometry::Point_3(0,0,0);
	/// \brief Number of facet for each polygon (used in cell representation)
	unsigned int _numberMaxFacetPoly = 0;
	/// \brief Delta refinement for each cell
	double _deltaReffinement = -1;
	/// \brief File containing the population
	std::string _populationFile = "";

	// Regions
	/// \brief Region container : necrosis, intermediary and external regions
	std::vector<SpheroidRegion> _regions;
	/// \brief Ratio defining the internal radius (ie outer delimitation of the necrosis region)
	double _internalLayerRatio = -1;
	/// \brief Ratio defining the intermediary radius (ie outer delimitation of the intermediary region)
	double _intermediaryLayerRatio = -1;
	/// \brief Number of sampled cell for each region. -1 means we observe all cells
	double _numberSamplingCellPerRegion = -1;
	/// \brief Sampled cells
	std::vector<const Settings::nCell::t_Cell_3*> _sampledCells;

	// Random engine (only used if not already set by the user
	CLHEP::MTwistEngine _randomEngine = CLHEP::MTwistEngine(time(nullptr));

	// Messenger
	std::unique_ptr<PopulationMessenger> _messenger;
};

}

#endif
