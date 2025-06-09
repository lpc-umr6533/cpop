#include "CpopSpectrumInfo.hh"

#include <algorithm>

void CpopSpectrumInfo::addSpectrum(const std::string &spectrum) {
	_spectrumMap.push_back(spectrum);
	_spectrumProba.insert(std::make_pair(spectrum, std::vector<double>(3)));
}

std::vector<std::string> CpopSpectrumInfo::getSpectrumMap() const {
	return _spectrumMap;
}

void CpopSpectrumInfo::displaySpectrum(std::ostream &o) const {
	for(unsigned i =  0 ; i < _spectrumMap.size(); ++i) {
		o << "Id : " << i << " Name : " << _spectrumMap[i] << std::endl;
	}

	o << std::endl;
}

void CpopSpectrumInfo::addSpectrumProba(std::string spectrum, CpopSpectrumInfo::Layer layer, const std::vector<double> proba) {
	auto it = _spectrumProba.find(spectrum);
	if(it == _spectrumProba.end())
		throw std::invalid_argument(spectrum + " does not exist");

	int pos = static_cast<int>(layer);
	it->second[pos] = proba[pos];
}

void CpopSpectrumInfo::addSpectrumProba(std::string spectrum, const std::vector<double> proba) {
	addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::Necrosis, proba);
	addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::Intermediary, proba);
	addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::External, proba);
}

std::map<std::string, std::vector<double> > CpopSpectrumInfo::getSpectrumProba() const {
	return _spectrumProba;
}

std::map<std::string, double> CpopSpectrumInfo::getSpectrum(CpopSpectrumInfo::Layer layer) {
	std::map<std::string, double> spectrums; //spectrumId + proba associated with layer

	for(auto const& it : _spectrumProba) {
		std::string spectrum = it.first;
		int id = getId(spectrum);
		if (id != -1)
			spectrums.insert(std::make_pair(spectrum, _spectrumProba[spectrum][static_cast<int>(layer)]));
	}

	return spectrums;
}

int CpopSpectrumInfo::getId(std::string spectrum) {
	auto it = std::find(_spectrumMap.begin(), _spectrumMap.end(), spectrum);
	int id = -1;

	if (it != _spectrumMap.end())
		id = std::distance(_spectrumMap.begin(), it);

	return id;
}

std::ostream &operator<<(std::ostream &o, const CpopSpectrumInfo &spectrumInfo) {
	spectrumInfo.displaySpectrum(o);
	return o;
}
