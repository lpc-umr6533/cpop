#include "UniformSource.hh"

#include "Population.hh"
#include "Randomize.hh"

namespace cpop {

UniformSource::UniformSource(const string &name, const Population &population)
    :Source(name, population),
      messenger_(std::make_unique<UniformSourceMessenger>(this))
{

}

int UniformSource::total_particle() const
{
    return total_particle_;
}

void UniformSource::setTotal_particle(int total_particle)
{
    total_particle_ = total_particle;
}

std::vector<G4ThreeVector> UniformSource::GetPosition()
{
    G4double spheroid_radius = this->population()->spheroid_radius();

    Settings::Geometry::Point_3 center = this->population()->spheroid_centroid();
    G4ThreeVector spheroid_centroid(center.x(), center.y(), center.z());

    //Get a random point on the unit sphere surface
    G4ThreeVector position_0 = this->randSphere();
    // Generate a random radius in [0, spheroid_radius]
    G4double radius = std::pow(G4UniformRand(), 1.0/3.0) * spheroid_radius;
    // Scale and translate position
    position_0 *= radius;
    position_0 += spheroid_centroid;

    std::vector<G4ThreeVector> position = {position_0};

    return position;
}

void UniformSource::Update()
{
    ++already_generated_;
}

bool UniformSource::HasLeft()
{
    return already_generated_ < total_particle_;
}

int UniformSource::already_generated() const
{
    return already_generated_;
}

UniformSourceMessenger &UniformSource::messenger()
{
    return *messenger_;
}

}
