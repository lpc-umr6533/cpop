#ifndef CONFIGREADER_HH
#define CONFIGREADER_HH

#include <vector>
#include "cReader/zupply.hpp"
#include "cReader/sectionreader.hh"
#include "cReader/sectionreaderfactory.hh"

namespace conf {

template <typename T>
class ConfigReader {
	T* objToFill;
	std::vector<SectionReader<T>*> sections;

public:
	ConfigReader(T* objFill = nullptr):
		objToFill(objFill)
	{}

	~ConfigReader() {
		for(size_t i = 0 ; i < sections.size(); ++i)
			delete sections[i];
	}

	void addSection(SectionReader<T>* section);

	template <template <class V> class U>
	void addSection();

	T* parse(std::string const& filename);

	T *getObjToFill() const {
		return objToFill;
	}

	void setObjToFill(T *value) {
		objToFill = value;
	}
};

template <typename T>
void ConfigReader<T>::addSection(SectionReader<T>* section) {
	sections.push_back(section);
}

template <typename T>
template <template <class V> class U>
void ConfigReader<T>::addSection() {
	sections.push_back(SectionReaderFactory::getInstance()->produce<U<T>>());
}

template <typename T>
T* ConfigReader<T>::parse(std::string const& filename) {
	if(sections.empty())
		throw std::logic_error("Error : ConfigReader does not contain any sections.");

	zz::cfg::CfgParser parser(filename);

	if(!objToFill) objToFill = new T();

	try {
		for(size_t i = 0 ; i < sections.size(); ++i) {
			sections[i]->setParser(&parser);
			sections[i]->setObjToFill(objToFill);
			sections[i]->fill();
		}
	} catch (const std::invalid_argument& e) {
		std::cerr << "Warning : some parts of the configuration file may not be read properly" << std::endl;
		std::cerr << e.what() << std::endl;
	} catch (const std::exception& e) {
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Error : Unexpected and unknown error happened" << std::endl;
	}

	return objToFill;
}

}

#endif

