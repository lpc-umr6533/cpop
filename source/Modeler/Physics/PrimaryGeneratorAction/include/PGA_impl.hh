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
    bool HasSource() const;
    int TotalEvent() const;
    void Initialize();

    const Settings::nCell::t_Cell_3 *findCell(const Settings::Geometry::Point_3& point);

    void checkBeamOn() const;

    void ActivateDiffusion(G4String diffusion_string);

    UniformSource& addUniformSource(const std::string& source_name);
    UniformSource* uniform_source();

    DistributedSource& addDistributedSource(const std::string& source_name);
    DistributedSource* distributed_source();

    PGA_implMessenger& messenger() {
        return *messenger_;
    }

    double PDF_RadioactiveDecay(double uniform_number, double lambda);
    double GenerateTimeBeforeDecay();
    double GenerateDistanceAfterDiffusion(double timeBeforeDecay);

    void writePositionsDirectionsTxt(G4ThreeVector position,
                                     G4ThreeVector direction);

    G4ThreeVector GenerateNewPositionAfterDiffusion(G4ThreeVector previousPosition,
                                                    double diffusion_distance);

    std::string findOrganelle(const Settings::nCell::t_Cell_3* cell, const Settings::Geometry::Point_3& point);

    /// \brief Select a source between the uniform source and the distributed one
    Source *selectSource() const;

    static std::atomic<int> nbUniform;
    static std::atomic<int> nbDistributed;

    static void resetFlags() {
        is_init_ = false;
    }

    int current_cell_id;

    bool diffusion_bool;

    G4int nb_essais_diffusion;

    G4int indice_if_diffusion = 0;

    std::vector<G4double> if_Energy_diffusion_vector;

    double half_life;

    G4ThreeVector new_G4_particle_position;


private:

    /// \brief Octree containing SAMPLED cells
    std::unique_ptr<Octree<OctreeNodeForSpheroidalCell>> octree_;

    const Population* population_;

    /// \brief Particle gun to generate particles
    std::unique_ptr<G4ParticleGun>  particle_gun_;

    /// \brief Source applied uniformly in the spheroid
    std::unique_ptr<UniformSource> uniform_source_;

    /// \brief DistributedSource source
    std::unique_ptr<DistributedSource> distributed_source_;

    /// \brief Messenger
    std::unique_ptr<PGA_implMessenger> messenger_;

    bool is_initialized_ = false;

    bool is_checked = false;

    // Thread safe mutex
    static std::mutex generate_primaries_mutex_;
    static std::mutex add_uniform_mutex_;
    static std::mutex add_distributed_mutex_;
    static std::mutex initialize_mutex_;
    static bool is_init_;
};

}

#endif // PGA_IMPL_HH
