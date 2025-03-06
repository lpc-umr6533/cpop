#include "RunAction.hh"

#include <sstream>

#include "G4RunManager.hh"
#include "G4Run.hh"

#include "PrimaryGeneratorAction.hh"

#include "analysis.hh"
#include <G4AnalysisManager.hh>

namespace cpop {

RunAction::RunAction()
    :G4UserRunAction()
{
    // The choice of analysis technology is done via selection of a namespace
      // in analysis.hh
     auto analysisManager = G4AnalysisManager::Instance();
     analysisManager->SetNtupleMerging(false); //switch to true when merging works ...

     // Creating ntuple
     //
     analysisManager->CreateNtuple("Edep", "Energy deposition by cell");
     analysisManager->CreateNtupleDColumn("posX");
     analysisManager->CreateNtupleDColumn("posY");
     analysisManager->CreateNtupleDColumn("posZ");
     analysisManager->CreateNtupleDColumn("momDirX");
     analysisManager->CreateNtupleDColumn("momDirY");
     analysisManager->CreateNtupleDColumn("momDirZ");
     analysisManager->CreateNtupleDColumn("edep");
     analysisManager->CreateNtupleIColumn("cellID");
     analysisManager->CreateNtupleSColumn("organelle");
     analysisManager->CreateNtupleSColumn("region");
     analysisManager->FinishNtuple();

}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run * /*run*/)
{

    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();
    if(!file_name_.empty()) { // If you are setting the output file name using the setter instead of G4 macro
        analysisManager->OpenFile(file_name_);
    } else {
        analysisManager->OpenFile();
    }
}

void RunAction::EndOfRunAction(const G4Run * /*run*/)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}

std::string RunAction::file_name() const
{
    return file_name_;
}

void RunAction::setFile_name(const std::string &file_name)
{
    file_name_ = file_name;
}

}
