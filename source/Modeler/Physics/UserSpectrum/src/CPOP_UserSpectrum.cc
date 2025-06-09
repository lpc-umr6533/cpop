#include "CPOP_UserSpectrum.hh"
#include "Randomize.hh"

#include "CPOP_DiscreteSpectrumRange.hh"
#include "CPOP_HistogramSpectrumRange.hh"
#include "CPOP_InterpolatedSpectrumRange.hh"

#include <set>

CPOP_UserSpectrum::CPOP_UserSpectrum(G4String file_to_read) {
	G4int nline;
	G4double proba_read;
	G4double previous_proba;
	G4double energy_read;

	// Read energy and proba
	FILE* file = fopen(file_to_read, "r");
	if(!file) {
		std::cout << "failed to open file : " << file_to_read << std::endl;
		exit(0);
	}

	int nbByteRead = fscanf(file, "%i %i %f", &_dimSpectrum, &_mode, &_eminSpectrum);

	if(nbByteRead < 0) {
		std::cout << "fail to read the given spectrum\n";
		exit(0);
	}

	switch(_mode) {
		case 1:
		{
			//G4cout << "Discrete spectrum" << G4endl;
			break;
		}
		case 2:
		{
			//G4cout << "Histogram spectrum" << G4endl;
			break;
		}
		case 3:
		{
			//G4cout << "Interpolated spectrum" << G4endl;
			break;
		}
		default:
		{
			//G4cout << "Mode not recognized, check spectrum file!" << G4endl;
			exit(1);
		}
	}

	_sumProba = 0.;
	previous_proba = -1.;
	G4double previous_sum_proba = 0.;
	G4double previousEnergy = 0.;
	std::set<CPOP_SpectrumRange*> spectrumRanges;

	G4double probaLowBound = 0.; 	// used for case 2 only ( histogram )
	G4double probaHighBound = 0.;	// used for case 2 only ( histogram )

	G4double previousEnergy2 = -1.; 	// used for casz 3 only ( Interpolaed ). Is the previous previous energy

	for(nline=0;nline<_dimSpectrum;nline++) {
		nbByteRead = fscanf(file,"%lf %lf",&energy_read,&proba_read);
		// if probability is null
		if(proba_read <= 0.)
			continue;

		if(nbByteRead < 0 ) {
			std::cout << "fail to rad the given spectrum\n";
			exit(0);
		}

		switch(_mode) {
			// case Discrete spectrum
			case 1:
			{
				// discrete case : lowEnergyBound and high energy bound are the same
				spectrumRanges.insert(new CPOP_DiscreteSpectrumRange(std::make_pair(_sumProba, energy_read), std::make_pair(_sumProba+proba_read, energy_read)));
				_sumProba += proba_read;
				break;
			}
			// case histogram spectrum
			case 2:
			{
				G4double lowEnergyBound;

				// store the first element differently
				if(nline == 0) {
					lowEnergyBound = _eminSpectrum;

					probaLowBound = 0.;
					probaHighBound = proba_read*(energy_read - _eminSpectrum);
				} else {
					lowEnergyBound = previousEnergy;

					probaLowBound = probaHighBound;
					probaHighBound += (energy_read - previousEnergy)*proba_read;
				}
				// cout << "@creation : probas : " << probaLowBound << "-" << probaHighBound << ", energies : " << lowEnergyBound << "-" << energy_read << endl;
				spectrumRanges.insert( new CPOP_HistogramSpectrumRange(std::make_pair(probaLowBound, lowEnergyBound), std::make_pair(probaHighBound, energy_read)));
				_sumProba = probaHighBound;
				break;
			}
			// case interpolated spectrum
			case 3:
			{
				// the first range comporte himself as an histogramSpectrumRange
				if(nline > 0) {
					previous_sum_proba = _sumProba;
					_sumProba += (energy_read - previousEnergy)*previous_proba-0.5*(energy_read - previousEnergy)*(previous_proba - proba_read);

					// if same probabilities
					if(proba_read == previous_proba) {
						// if no previousEnergy2 setted yet :
						if(nline == 1) {
							spectrumRanges.insert( new CPOP_HistogramSpectrumRange(std::make_pair(0., _eminSpectrum), std::make_pair(_sumProba, previousEnergy)));
						} else {
							spectrumRanges.insert(new CPOP_HistogramSpectrumRange(std::make_pair(previous_sum_proba, previousEnergy2), std::make_pair(_sumProba, previousEnergy)));
						}
					} else {
						G4double lAlpha = (proba_read - previous_proba) / (energy_read - previousEnergy) ;
						G4double lBeta = proba_read - energy_read*lAlpha;
						G4double lA = previousEnergy;
						G4double lB = energy_read;
						G4double lGamma = (lAlpha/2.)*(lB*lB-lA*lA)+lBeta*(lB-lA) ;
						spectrumRanges.insert( new CPOP_InterpolatedSpectrumRange(std::make_pair(previous_sum_proba, _sumProba), lAlpha, lBeta, lGamma, lA, lB));
					}
				}

				previousEnergy2 = previousEnergy;
				break;
			}
		}

		previousEnergy = energy_read;
		previous_proba = proba_read;
	}

	fclose(file);

	// uniformize proba
	{
		for(auto const& spectrumRange : spectrumRanges) {
			spectrumRange->setProbaLowBound( spectrumRange->getProbaLowBound() / _sumProba);
			spectrumRange->setProbaHighBound(spectrumRange->getProbaHighBound() / _sumProba);
		}
	}

	// create the map stored by the proba sum ( high bound )
	{

		for(auto const& spectrumRange : spectrumRanges) {
			_mapToSpectrumRanges.insert(std::make_pair(
				spectrumRange->getProbaHighBound(),
				spectrumRange
			));
		}
	}
}

CPOP_UserSpectrum::~CPOP_UserSpectrum() {
	for(auto const& _mapToSpectrumRange : _mapToSpectrumRanges)
		delete _mapToSpectrumRange.second;
	_mapToSpectrumRanges.clear();
}

G4double CPOP_UserSpectrum::GetEnergy() const {
	G4double my_rndm=G4UniformRand();

	auto itRange = _mapToSpectrumRanges.lower_bound(my_rndm);
	while (itRange == _mapToSpectrumRanges.end()) {
		my_rndm = G4UniformRand();
		itRange = _mapToSpectrumRanges.lower_bound(my_rndm);
	}

	return itRange->second->computeEnergy();
}

G4double CPOP_UserSpectrum::GetEnergy(G4double my_rndm, G4double secondRnd) const {
	auto itRange = _mapToSpectrumRanges.lower_bound(my_rndm);
	if(itRange == _mapToSpectrumRanges.end()) {
		//G4cout << "CPOP_UserSpectrum::GetEnergy, havn't found energy for the shooted probability : ! " << my_rndm << G4endl;
		return 0.;
	}

	return itRange->second->GetEnergy(secondRnd);
}
