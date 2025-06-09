#ifndef SECTIONREADERFACTORY_HH
#define SECTIONREADERFACTORY_HH

#include "cReader/sectionreader.hh"

namespace conf {

class SectionReaderFactory {

	SectionReaderFactory() {}
	~SectionReaderFactory() = default;

public:
	static SectionReaderFactory* getInstance() {
		static SectionReaderFactory instance;
		return &instance;
	}

	template <typename T>
	T* produce();
};

template <typename T>
T *SectionReaderFactory::produce() {
	return new T();
}

}

#endif
