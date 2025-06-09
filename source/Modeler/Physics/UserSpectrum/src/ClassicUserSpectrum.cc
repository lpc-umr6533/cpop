#include "ClassicUserSpectrum.hh"
#include "Randomize.hh"
#include <stdexcept>

ClassicUserSpectrum::ClassicUserSpectrum(G4String file_to_read) {
	G4int nline;
	G4float proba_read;
	G4float energy_read;
	FILE *file = fopen(file_to_read, "r");

	// Read energy and proba
	if(fscanf(file,"%i %i %f",&_dimSpectrum,&_mode,&_eminSpectrum) != 3)
		throw std::runtime_error{"ClassicUserSpectrum: bad file format"};

	_tabEnergy = new G4float[_dimSpectrum];
	_tabProba = new G4float[_dimSpectrum];

	nline = 0;

	while(nline<_dimSpectrum) {
		if(fscanf(file,"%f %f",&energy_read,&proba_read) != 2)
			throw std::runtime_error{"ClassicUserSpectrum: bad file format"};

		_tabEnergy[nline] = energy_read;
		_tabProba[nline] = proba_read;
		nline++;
	}

	fclose(file);

	// Construct proba table
	_sumProba = 0;
	nline = 0;

	switch(_mode) {
	case 1:
		G4cout << "Discrete spectrum" << G4endl;
		_tabSumproba = new G4float[_dimSpectrum];

		while(nline < _dimSpectrum){
			_sumProba += _tabProba[nline];
			_tabSumproba[nline] = _sumProba;
			nline++;
		}
		break;
	case 2:
		G4cout << "Histogram spectrum" << G4endl;
		_tabSumproba = new G4float[_dimSpectrum];
		_sumProba = _tabProba[0]*(_tabEnergy[0]-_eminSpectrum);
		_tabSumproba[0] = _sumProba;
		for(nline=1; nline < _dimSpectrum; nline++){
			_sumProba += (_tabEnergy[nline]-_tabEnergy[nline-1])*_tabProba[nline];
			_tabSumproba[nline] = _sumProba;
		}
		break;
	case 3:
		G4cout << "Interpolated spectrum" << G4endl;
		_tabSumproba = new G4float[_dimSpectrum-1];
		for(nline=1; nline < _dimSpectrum; nline++){
			_sumProba += (_tabEnergy[nline]-_tabEnergy[nline-1])*_tabProba[nline-1]-0.5*(_tabEnergy[nline]-_tabEnergy[nline-1])*(_tabProba[nline-1]-_tabProba[nline]);
			_tabSumproba[nline-1] = _sumProba;
		}
		break;
	default:
		G4cout << "Mode not recognized, check spectrum file!" << G4endl;
		break;
	}
}

ClassicUserSpectrum::~ClassicUserSpectrum() {
	delete[] _tabProba;
	delete[] _tabSumproba;
	delete[] _tabEnergy;
}

G4float ClassicUserSpectrum::GetEnergy() const {
	return GetEnergy(G4UniformRand());
}

G4float ClassicUserSpectrum::GetEnergy(G4double my_rndm) const {
	G4double energy = 0.;
	G4int i = 0;

	while(my_rndm >= (_tabSumproba[i])/_sumProba) i++;

	G4double a, b, alpha, beta, gamma, X;

	switch(_mode) {
	case 1:
		//Discrete Spectrum
		energy = _tabEnergy[i];
		break;
	case 2:
		//Histogram spectrum
		if(i == 0) {
			energy = (_tabEnergy[i]-_eminSpectrum)*G4UniformRand()+_eminSpectrum;
		} else {
			energy = (_tabEnergy[i]-_tabEnergy[i-1])*G4UniformRand()+_tabEnergy[i-1];
		}
		break;
	case 3:
		//Interpolated spectrum
		a = _tabEnergy[i];
		b = _tabEnergy[i+1];

		if(_tabProba[i]==_tabProba[i+1]) {
			if(i==0) {
				energy = (_tabEnergy[0]-_eminSpectrum)*G4UniformRand()+_eminSpectrum;
			} else {
				energy = (_tabEnergy[i]-_tabEnergy[i-1])*G4UniformRand()+_tabEnergy[i-1];
			}
		} else {
			alpha = (_tabProba[i+1]-_tabProba[i])/(_tabEnergy[i+1]-_tabEnergy[i]);
			beta = _tabProba[i+1]-_tabEnergy[i+1]*alpha;
			gamma = (alpha/2)*(b*b-a*a)+beta*(b-a);
			my_rndm = G4UniformRand();

			auto delta = (alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm;
			auto sqrtDelta = sqrt(delta);
			X = (-beta+sqrtDelta)/(alpha);
			if((X-a)*(X-b)<=0) {
				energy = X;
			} else {
				energy = (-beta-sqrtDelta)/(alpha);
			}
		}
		break;
	default:
		energy = 0;
		break;
	}

	// std::cout << " energy returned = " << energy << std::endl;
	return energy;
}

G4float ClassicUserSpectrum::GetEnergy(G4double rnd1, G4double rnd2) const {
	G4double energy = 0.;
	G4int i = 0;
	G4double my_rndm = rnd1;
	// std::cout << " fist shotted = " << my_rndm << std::endl;
	while(my_rndm >= (_tabSumproba[i])/_sumProba) i++;

	G4double a, b, alpha, beta, gamma, X;

	switch(_mode) {
	case 1:
		//Discrete Spectrum
		energy=_tabEnergy[i];
		break;
	case 2:
		//Histogram spectrum
		if(i == 0) energy = (_tabEnergy[0]-_eminSpectrum)*rnd2+_eminSpectrum;
		else energy = (_tabEnergy[i]-_tabEnergy[i-1])*rnd2+_tabEnergy[i-1];
		break;
	case 3:
		//Interpolated spectrum
		a = _tabEnergy[i];
		b = _tabEnergy[i+1];
		if(_tabProba[i] == _tabProba[i+1]) {
			if(i==0) {
				energy=(_tabEnergy[0]-_eminSpectrum)*rnd2+_eminSpectrum;
			} else {
				energy=(_tabEnergy[i]-_tabEnergy[i-1])*rnd2+_tabEnergy[i-1];
			}
		} else {
			alpha = (_tabProba[i+1]-_tabProba[i])/(_tabEnergy[i+1]-_tabEnergy[i]);
			beta=_tabProba[i+1]-_tabEnergy[i+1]*alpha;
			gamma=(alpha/2)*(b*b-a*a)+beta*(b-a);
			my_rndm=rnd2;

			auto delta = (alpha*a+beta)*(alpha*a+beta)+2*alpha*gamma*my_rndm;
			auto sqrtDelta = sqrt(delta);
			X = (-beta+sqrtDelta)/(alpha);
			if((X-a)*(X-b)<=0) {
				energy = X;
			} else {
				energy = (-beta-sqrtDelta)/(alpha);
			}
		}
		break;
	default:
		energy = 0;
		break;
	}

	// std::cout << " energy returned = " << energy << std::endl;
	return energy;
}
