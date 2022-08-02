#include "CpopSpectrumInfo.hh"

CpopSpectrumInfo::CpopSpectrumInfo()
{

}

CpopSpectrumInfo::~CpopSpectrumInfo()
{

}

void CpopSpectrumInfo::addSpectrum(const std::string &spectrum)
{
    spectrumMap.push_back(spectrum);
    spectrumProba.insert(std::make_pair(spectrum, std::vector<double>(3)));
//    std::cout << "Necro proba :" << spectrumProba[spectrum][static_cast<int>(Layer::Necrosis)];
}

std::vector<std::string> CpopSpectrumInfo::getSpectrumMap() const
{
    return spectrumMap;
}

void CpopSpectrumInfo::displaySpectrum(std::ostream &o) const
{
    for(unsigned i =  0 ; i < spectrumMap.size(); ++i) {
        o << "Id : " << i << " Name : " << spectrumMap[i] << std::endl;
    }
    o << std::endl;
}

void CpopSpectrumInfo::addSpectrumProba(std::string spectrum, CpopSpectrumInfo::Layer layer, const std::vector<double> proba)
{
    auto it = spectrumProba.find(spectrum);


    if(it == spectrumProba.end())
        throw std::invalid_argument(spectrum + " does not exist");

    int pos = static_cast<int>(layer);
    it->second[pos] = proba[pos];

}

void CpopSpectrumInfo::addSpectrumProba(std::string spectrum, const std::vector<double> proba)
{
    addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::Necrosis, proba);
    addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::Intermediary, proba);
    addSpectrumProba(spectrum, CpopSpectrumInfo::Layer::External, proba);
}

std::map<std::string, std::vector<double> > CpopSpectrumInfo::getSpectrumProba() const
{
    return spectrumProba;
}

std::map<std::string, double> CpopSpectrumInfo::getSpectrum(CpopSpectrumInfo::Layer layer)
{
    std::map<std::string, double> spectrums; //spectrumId + proba associated with layer

    for(auto it = spectrumProba.begin(); it != spectrumProba.end(); ++it) {
        std::string spectrum = it->first;
        int id = getId(spectrum);
        if (id != -1)
            spectrums.insert(std::make_pair(spectrum, spectrumProba[spectrum][static_cast<int>(layer)]));
    }

    return spectrums;
}

int CpopSpectrumInfo::getId(std::string spectrum)
{
    auto it = std::find(spectrumMap.begin(), spectrumMap.end(), spectrum);
    int id = -1;

    if (it != spectrumMap.end())
        id = std::distance(spectrumMap.begin(), it);

    return id;
}

std::ostream &operator<<(std::ostream &o, const CpopSpectrumInfo &spectrumInfo)
{
    spectrumInfo.displaySpectrum(o);
    return o;
}
