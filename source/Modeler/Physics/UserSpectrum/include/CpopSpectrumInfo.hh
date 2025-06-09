#ifndef CPOPSPECTRUMINFO_H
#define CPOPSPECTRUMINFO_H

#include <map>
#include <vector>
#include <iostream>

class CpopSpectrumInfo {
public:
	enum class Layer {Necrosis = 0, Intermediary = 1, External = 2};

	void addSpectrum(const std::string& spectrum);
	[[nodiscard]] std::vector<std::string> getSpectrumMap() const;
	void displaySpectrum(std::ostream& o = std::cout)  const;

	void addSpectrumProba(std::string spectrum, Layer layer, const std::vector<double> proba);
	void addSpectrumProba(std::string spectrum, const std::vector<double> proba);
	[[nodiscard]] std::map<std::string, std::vector<double> > getSpectrumProba() const;
	std::map<std::string, double> getSpectrum(Layer layer);

	int getId(std::string spectrum);

private:
	std::vector<std::string> _spectrumMap;
	std::map<std::string, std::vector<double>> _spectrumProba;
};

std::ostream& operator<<(std::ostream& o, const CpopSpectrumInfo& spectrumInfo);

#endif
