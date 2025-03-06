#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"

namespace cpop {

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    std::string file_name() const;
    void setFile_name(const std::string &file_name);

private:

    std::string file_name_ = "";
};

}

#endif // RUNACTION_HH
