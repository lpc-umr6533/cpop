#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"

namespace cpop {

class RunAction : public G4UserRunAction {
public:
	RunAction();
	~RunAction() override;
	void BeginOfRunAction(const G4Run*) override;
	void EndOfRunAction(const G4Run*) override;

	[[nodiscard]] std::string file_name() const;
	void setFile_name(const std::string &file_name);

private:
	std::string _filename = "";
};

}

#endif
