#ifndef PGA_IMPL_HH
#define PGA_IMPL_HH

#include <atomic>
#include <memory>
#include <mutex>
#include <vector>

#include "analysis.hh"
#include "G4Step.hh"

#include "G4ParticleGun.hh"

#include "CGAL_Utils.hh"
#include "BoundingBox.hh"
#include "AgentSettings.hh"
#include "CellSettings.hh"
#include "Population.hh"

#include "PGA_implMessenger.hh"
#include "UniformSource.hh"
#include "DistributedSource.hh"

#include "Octree.hh"
#include "OctreeNodeForSpheroidalCell.hh"

namespace cpop {

class Population;
class Source;
class UniformSource;
class DistributedSource;

class PGA_impl
{
  /// Victor Levrague : modification of the GeneratePrimaries function in order to take into account :
  /// - diffusion of alpha-emitting radionuclide's daughter(s) after fixation
  /// - get ID of emission cell, to store it later in root file
  /// - generate random positions for each different particle generated on the sam cell
public:
	PGA_impl(const Population& population);

	void GeneratePrimaries(G4Event* event);
	[[nodiscard]] bool HasSource() const;
	[[nodiscard]] int TotalEvent() const;
	void Initialize();

	const Settings::nCell::t_Cell_3 *findCell(const Settings::Geometry::Point_3& point);

	void checkBeamOn() const;

	void ActivateDiffusion(G4String diffusion_string, G4double half_life);

	UniformSource& addUniformSource(const std::string& source_name);
	UniformSource* uniform_source();

	DistributedSource& addDistributedSource(const std::string& source_name);
	DistributedSource* distributed_source();

	PGA_implMessenger& messenger() {
		return *_messenger;
	}

	double PDF_RadioactiveDecay(double uniform_number, double lambda);
	double GenerateTimeBeforeDecay();
	double GenerateDistanceAfterDiffusion(double timeBeforeDecay);

	void writingInfoPrimariesTxt(
		G4ThreeVector position,
		G4ThreeVector direction,
		G4double energy,
		G4String name_file
	);

	void readInfoPrimariesTxt(int i, G4String name_file);

	void readInfoPrimariesTxt_Hack(int i, G4String name_file);

	void setPositionsDirections(G4String name_file, G4String name_method);

	void setPositions(G4String name_file);

	void SetTxtInfoPrimariesName_and_MethodName(G4String name_file, G4String name_method) {
		nameInfoPrimariesFile = name_file;
		nameMethodForInfoPrimaries = name_method;
		readInputPositionFile = true;
	}

	void energySpectraLithium7BNCT();

	void SetLi7BNCTSpectrumBool(G4String string_bool) {
		if (string_bool == "yes") {
			li7BNCTSpectra = true;
		} else if (string_bool == "no") {
			li7BNCTSpectra = false;
		}
	}

	G4ThreeVector GenerateNewPositionAfterDiffusion(
		G4ThreeVector previousPosition,
		double diffusion_distance
	);

	std::string findOrganelle(const Settings::nCell::t_Cell_3* cell, const Settings::Geometry::Point_3& point);

	/// \brief Select a source between the uniform source and the distributed one
	[[nodiscard]] Source *selectSource() const;

	static std::atomic<int> nbUniform;
	static std::atomic<int> nbDistributed;

	static void resetFlags() {
		_isInit = false;
	}

	int currentCellId;

	bool diffusionBool;

	bool readInputPositionFile = false;

	G4int nbEssaisDiffusion;

	G4int indiceIfDiffusion = 0;

	std::vector<G4double> ifEnergyDiffusionVector;

	double halfLife;

	G4ThreeVector newG4ParticlePosition;

	G4ThreeVector vecPosition;
	G4ThreeVector vecDirection;

	G4ThreeVector direction;
	G4ThreeVector g4ParticlePosition;
	G4double particleEnergy;
	G4double energyFromTxt;

	G4String nameInfoPrimariesFile;
	G4String nameMethodForInfoPrimaries;

	bool li7BNCTSpectra;

	std::vector<const Settings::nCell::t_Cell_3 *> labeledCells;

private:
	/// \brief Octree containing SAMPLED cells
	std::unique_ptr<Octree<OctreeNodeForSpheroidalCell>> _octree;

	const Population* _population;

	/// \brief Particle gun to generate particles
	std::unique_ptr<G4ParticleGun> _particleGun;

	/// \brief Source applied uniformly in the spheroid
	std::unique_ptr<UniformSource> _uniformSource;

	/// \brief DistributedSource source
	std::unique_ptr<DistributedSource> _distributedSource;

	/// \brief Messenger
	std::unique_ptr<PGA_implMessenger> _messenger;

	bool _isInitialized = false;

	bool _isChecked = false;

	// Thread safe mutex
	static std::mutex _generatePrimariesMutex;
	static std::mutex _addUniformMutex;
	static std::mutex _addDistributedMutex;
	static std::mutex _initializeMutex;
	static bool _isInit;
};

}

#endif
