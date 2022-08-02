#ifndef CUSTOMSECTION_HH
#define CUSTOMSECTION_HH

#include "cReader/sectionreader.hh"

namespace conf {

template <typename T>
class CustomSection : public conf::SectionReader<T> {

public:
    CustomSection() {}
    ~CustomSection() {}

    void fill() {
        this->objToFill->setMyInt( this->template load<int>("Load", "int") );
        this->objToFill->setMyDouble( this->template load<double>("Load", "double") );
        this->objToFill->setMyBool( this->template load<bool>("Load", "bool") );
        this->objToFill->setMyVector( this->template load<std::vector<double>>("Load", "vector") );
    }
};

}

#endif // CUSTOMSECTION_HH

