#ifndef UNIFORMSOURCE_HH
#define UNIFORMSOURCE_HH

#include "Source.hh"
#include "UniformSourceMessenger.hh"

namespace cpop {

class Population;

class UniformSource : public Source
{
public:
    UniformSource(const std::string& name, const Population& population);

    int total_particle() const;
    void setTotal_particle(int total_particle);

    std::vector<G4ThreeVector> GetPosition() override;
    void Update() override;
    bool HasLeft() override;

    int already_generated() const;

    UniformSourceMessenger& messenger();

private:
    /// \brief Number of particles to be generated
    int total_particle_ = 0;
    /// \brief Number of particles already generated
    int already_generated_ = 0;
    /// \brief Messenger
    std::unique_ptr<UniformSourceMessenger> messenger_;
};

}

#endif // UNIFORMSOURCE_HH
