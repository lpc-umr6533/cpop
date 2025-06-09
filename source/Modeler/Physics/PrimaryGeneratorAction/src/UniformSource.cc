#include "UniformSource.hh"

#include "Population.hh"
#include "Randomize.hh"

namespace cpop {

UniformSource::UniformSource(const std::string &name, const Population &population):
	Source(name, population),
	_messenger(std::make_unique<UniformSourceMessenger>(this))
{
}

int UniformSource::total_particle() const {
	return _totalParticle;
}

void UniformSource::setTotal_particle(int total_particle) {
	_totalParticle = total_particle;
}

std::vector<G4ThreeVector> UniformSource::GetPosition() {
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

void UniformSource::Update() {
	++_alreadyGenerated;
}

bool UniformSource::HasLeft() {
	return _alreadyGenerated < _totalParticle;
}

int UniformSource::already_generated() const {
	return _alreadyGenerated;
}

UniformSourceMessenger &UniformSource::messenger() {
	return *_messenger;
}

}
