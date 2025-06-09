#ifndef SPECTRUM_WEIGHT_HH
#define SPECTRUM_WEIGHT_HH

#include "CPOP_UserSpectrum.hh"

#include <cassert>
#include <map>
#include <numeric>
#include <utility>
#include <vector>

/// \brief links User spectrum and the sum of proba of each spectrum
/// warning : sum of all probabilities should be equal to 1
struct SpectrumsWeight {
	using UnNormalizeWeights = std::vector<std::pair<double, const CPOP_UserSpectrum*>>;

	UnNormalizeWeights unNormalizeWeights;
	std::map<double, const CPOP_UserSpectrum*> weights;

	SpectrumsWeight(UnNormalizeWeights p = {}):
		unNormalizeWeights(std::move(p))
	{
	}

	/// \brief add a possible spectrum with the given weights
	void add(double pWeight, const CPOP_UserSpectrum* pSpect) {
		assert(pSpect);
		unNormalizeWeights.emplace_back(pWeight, pSpect);
	}

	/// \brief init the spectrums weights by creating the map to shoot rand spectrums.
	void init() {
		// get sum weight
		double sumWeight = std::accumulate(std::begin(unNormalizeWeights), std::end(unNormalizeWeights), 0.);

		// normlization and creation of the "weights" map
		weights.clear();
		double lWeight = 0.;
		for(auto const& unNormalizeWeight : unNormalizeWeights) {
			// here we add weights of each spectrums because we use this weight as het top bound probablitie to be picked (by calling the map function low_bound)
			lWeight += unNormalizeWeight.first;
			weights.insert(std::make_pair(lWeight/sumWeight, unNormalizeWeight.second));
		}
	}

	/// \brief return the UserSpectrum corresponding to the given probability
	[[nodiscard]] const CPOP_UserSpectrum* getSpectrum(double pProba) const {
		assert(pProba >= 0);
		assert(pProba <= 1);
		auto itWeight = weights.lower_bound(pProba);
		assert(itWeight != weights.end());
		return itWeight->second;
	}

	/// \ brief return if the SpectrumWeight is normalized for the getSpectrum function
	[[nodiscard]] bool isNormalized() const {
		// get sum weight
		if(weights.lower_bound(0.) == weights.end())
			return false;

		if(weights.lower_bound(1.) == weights.end())
			return false;

		auto it = weights.end();
		--it;

		return (it->first == 1.);
	}
};

#endif
