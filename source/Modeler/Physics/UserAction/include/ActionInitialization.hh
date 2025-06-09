#ifndef ACTIONINITIALIZATION_HH
#define ACTIONINITIALIZATION_HH

#include <memory>

#include "G4VUserActionInitialization.hh"
#include "PGA_impl.hh"

namespace cpop {

class Population;

class ActionInitialization : public G4VUserActionInitialization
{
public:
	ActionInitialization(const Population& population);

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
