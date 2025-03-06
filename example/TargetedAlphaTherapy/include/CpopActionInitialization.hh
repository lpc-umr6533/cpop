// #ifndef ACTIONINITIALIZATION_HH
// #define ACTIONINITIALIZATION_HH

#include <memory>

#include "G4VUserActionInitialization.hh"
#include "PGA_impl.hh"

namespace cpop {

class Population;

class CpopActionInitialization : public G4VUserActionInitialization
{
public:
    CpopActionInitialization(const Population& population);

    virtual void BuildForMaster() const;
    virtual void Build() const;

private:
    // PGA_impl shared by all primary generators.
    // Only owned by the ActionInitialization class
    std::unique_ptr<PGA_impl> pga_impl_;

    const Population* population_;
};

}

// #endif // ACTIONINITIALIZATION_HH
