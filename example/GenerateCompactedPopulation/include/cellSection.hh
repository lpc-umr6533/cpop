#ifndef CELL__SECTION__H
#define CELL__SECTION__H

#include <vector>
#include <string>

// CPOP header
#include <cReader/sectionreader.hh>

// How to create your own configuration reader to build a T object
/* 1) Declare a ConfigReader object
 * conf::ConfigReader<T>* reader = new conf::ConfigReader<T>();
 *
 * 2) Add sections to the reader
 * reader->addSection<Section1>();
 * reader->addSection<Section2>();
 *
 * 3) Parse your file and retrieve your object
 * T* myObject = reader->parse(configFile);
 *
 * 4) Do not forget to free memory when you do not need it anymore
 * delete reader;
 * delete myObject;
 */

// How to create custom sections
/* 1) Include #include <cReader/sectionreader.hh>
 *
 * 2) Create a template class which inherits from conf::SectionReader<T>
 *
 * 3) Overload the fill method. One section class should only read one section in the configuration file
 * We provide a load method to read parts of the configuration file.
 * If you want to read :
 * _int    : int myInt = this->template load<int>(sectionName, keyName);
 * _double : double myDouble = this->template load<double>(sectionName, keyName);
 * _string : string myString = this->template load<string>(sectionName, keyName);
 * _vector : std::vector<double> myVector = this->template load<std::vector<double>>(sectionName, keyName);
 *
 * Note : vector can contain any type that supports << to a stringstream (according to zupply documentation http://zhreshold.github.io/zupply/classzz_1_1cfg_1_1_value.html)
 *
 * 4) You can get the object you are building this->objToFill and then call whatever methods/function you want
 *
 */

template <typename T>
class CellSection: public conf::SectionReader<T> {
public:
	void fill() {
		std::string sectionName("CellProperties");

		auto const& nucleusRadius = this->template load<std::vector<double>>(sectionName, "nucleusRadius");
		double minRadiusN = nucleusRadius[0], maxRadiusN = nucleusRadius[1];

		auto const& membraneRadius = this->template load<std::vector<double>>(sectionName, "membraneRadius");
		double minRadiusM = membraneRadius[0], maxRadiusM = membraneRadius[1];

		auto const& cytoplasmMaterials = this->template load<std::string>(sectionName, "cytoplasmMaterials");
		auto const& nucleusMaterials = this->template load<std::string>(sectionName, "nucleusMaterials");

		this->objToFill->setCellProperties(
			minRadiusN, maxRadiusN,
			minRadiusM, maxRadiusM,
			cytoplasmMaterials, nucleusMaterials
		);
	}
};

#endif
