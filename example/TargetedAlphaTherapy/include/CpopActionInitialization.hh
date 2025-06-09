#ifndef TARGETEDALPHATHERAPY_ACTIONINITIALIZATION_HH
#define TARGETEDALPHATHERAPY_ACTIONINITIALIZATION_HH

#include <memory>

#include "G4VUserActionInitialization.hh"
#include "PGA_impl.hh"

namespace cpop {

class Population;

class CpopActionInitialization : public G4VUserActionInitialization {
public:
	CpopActionInitialization(const Population& population);

	void BuildForMaster() const override;
	void Build() const override;

private:
	// PGA_impl shared by all primary generators.
	// Only owned by the ActionInitialization class
	std::unique_ptr<PGA_impl> _pgaImpl;

	const Population* _population;
};

}

#endif
