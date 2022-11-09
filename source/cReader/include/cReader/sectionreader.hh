/**
  * \file sectionreader.hh
  * \brief Base class to create section readers
  * \author Axel Delsol
  * \date 2018 April 06
  * */
#ifndef SECTIONREADER_H
#define SECTIONREADER_H

#include <sstream>

#include "cReader/zupply.hpp"

/** \namespace conf
  *
  * namespace regrouping all classes to read a configuration file
  * */
namespace conf {


/**
 * \class SectionReader
 * \brief Base class to create section reader.
 *
 * A section reader extracts informations from the configuration file.
 * You should not use this class directly but instead inherits from it and redefine the parse methods using the load method.
 *
 */
template <typename T = int>
class SectionReader
{
    zz::cfg::CfgParser* parser;

protected:
    T* objToFill;

public:
    /**
     * \brief SectionReader constructor
     */
    SectionReader() {};
    virtual ~SectionReader() {}

    /**
     * \brief Parser getter
     */
    zz::cfg::CfgParser *getParser() const {
        return parser;
    }

    /**
     * \brief Parser setter
     */
    void setParser(zz::cfg::CfgParser *value) {
        parser = value;
    }

    /**
     * \brief Check if value contains data
     *
     * \return Returns TRUE if value contains something and FALSE otherwise
     */
    bool check(zz::cfg::Value value) {
        return !value.empty();
    }

    T *getObjToFill() const {
        return objToFill;
    }

    void setObjToFill(T *value) {
        objToFill = value;
    }

    /**
     * \brief Loads a value associated with keyname contained in sectionName
     *
     * \param sectionName section name in the configuration file
     * \param keyName key name in the sectionName part of the configuration file
     *
     * \return Returns the stored value if it exists. Otherwise throw a std::invalid_argument exception
     */
    template <typename U>
    U load(const char* sectionName, const char* keyName);

    virtual void fill() {}

};

template <typename T>
template <typename U>
U SectionReader<T>::load(const char *sectionName, const char *keyName)
{
    zz::cfg::Value value = (*parser)(sectionName)[keyName];

    if(!check(value)) {
        std::stringstream errorMsg("");

        errorMsg << "Section " << sectionName << "does not contain key " << keyName << std::endl;

        throw std::invalid_argument(errorMsg.str().c_str());
    }

    return value.load<U>();
}

}



#endif // SECTIONREADER_H
