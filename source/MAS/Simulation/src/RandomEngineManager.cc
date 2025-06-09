#include "RandomEngineManager.hh"

static RandomEngineManager* randomEngine = nullptr;

#include <limits>
#include <QString>

static QString fileOutName = "randomOutput";

RandomEngineManager::RandomEngineManager():
	_rndEngine(nullptr),
	_outputRandom(nullptr)
{
}

RandomEngineManager::~RandomEngineManager() {
	delete _rndEngine;
}

RandomEngineManager* RandomEngineManager::getInstance() {
	if(!randomEngine)
		randomEngine = new RandomEngineManager();
	return randomEngine;
}

void RandomEngineManager::setEngine(CLHEP::HepRandomEngine* pEngine) {
	assert(pEngine);
	_rndEngine = pEngine;
}

double RandomEngineManager::randd() {
	assert(_rndEngine);
	return _rndEngine->flat();
}

double RandomEngineManager::randd(double min, double max) {
	if(min == max)	return min;
	return randd() * ( max - min ) + min;
}

int RandomEngineManager::randi() {
	assert(_rndEngine);
	return (int) (randd() * std::numeric_limits<int>::max());
}

int RandomEngineManager::randi(int min, int max) {
	if(min == max)  return min;
	if(min > max)   return randi(max, min);
	return (min + (randi()%(max - min +1)));
}

double RandomEngineManager::normaled(double min, double max, double medium, double standardDeviation) {
	return -1.;
}
