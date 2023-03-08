#ifndef PRIMARYGENERATORACTION_HH
#define PRIMARYGENERATORACTION_HH

#include <memory>

#include "G4VUserPrimaryGeneratorAction.hh"

#include "PGA_impl.hh"

namespace cpop {

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction(PGA_impl& pga_impl);

    void GeneratePrimaries(G4Event* event) override;
    int TotalEvent() const;

    HomogeneousSource* homogeneous_source() const;
    DistributedSource* distributed_source() const;

    PGA_implMessenger& messenger();

private:
    PGA_impl* pga_impl_;
};

}

#endif // PRIMARYGENERATORACTION_HH
