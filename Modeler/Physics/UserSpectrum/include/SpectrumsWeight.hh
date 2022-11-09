#ifndef SPECTRUM_WEIGHT_HH
#define SPECTRUM_WEIGHT_HH

#include "CPOP_UserSpectrum.hh"

#include <assert.h>
#include <map>
#include <vector>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
/// \brief links User spectrum and the sum of proba of each spectrum
/// warning : sum of all probabilities should be equal to 1
struct SpectrumsWeight
{
	vector<pair<double, const CPOP_UserSpectrum*> > unNormalizeWeights;
	map<double, const CPOP_UserSpectrum*> weights;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SpectrumsWeight(vector<pair<double, const CPOP_UserSpectrum*> > p = vector<pair<double, const CPOP_UserSpectrum*> > ())
	{
		unNormalizeWeights = p;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~SpectrumsWeight()
	{
		unNormalizeWeights.begin();
		weights.begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief add a possible spectrum with the given weights
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void add(double pWeight, const CPOP_UserSpectrum* pSpect)
	{
		assert(pSpect);
		unNormalizeWeights.push_back(make_pair(pWeight, pSpect));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief init the spectrums weights by creating the map to shoot rand spectrums.
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void init()
	{
		// get sum weight
		double sumWeight = 0.;
		vector<pair<double, const CPOP_UserSpectrum*> >::const_iterator it;
		for(it = unNormalizeWeights.begin(); it != unNormalizeWeights.end(); ++it)
		{
			sumWeight += it->first;
		}

		// normlization and creation of the "weights" map
		weights.clear();
		double lWeight = 0.;
		for(it = unNormalizeWeights.begin(); it != unNormalizeWeights.end(); ++it)
		{
			// here we add weights of each spectrums because we use this weight as het top bound probablitie to be picked (by calling the map function low_bound)
			lWeight += it->first;
			weights.insert(make_pair(lWeight/sumWeight, it->second));
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \brief return the UserSpectrum corresponding to the given probability
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const CPOP_UserSpectrum* getSpectrum(double pProba) const
	{
		assert(pProba >= 0);
		assert(pProba <= 1);
		map<double, const CPOP_UserSpectrum*>::const_iterator itWeight = weights.lower_bound(pProba);
		assert(itWeight != weights.end());
		return itWeight->second;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// \ brief return if the SpectrumWeight is normalized for the getSpectrum function
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool isNormalized() const
	{
		// get sum weight
		if(weights.lower_bound(0.) == weights.end()) 
		{
			return false;
		}

		if(weights.lower_bound(1.) == weights.end())
		{
			return false;
		}

		map<double, const CPOP_UserSpectrum*>::const_iterator it = weights.end();
		it --;
		return(it->first == 1.);
	}

};

#endif // SPECTRUM_WEIGHT_HH