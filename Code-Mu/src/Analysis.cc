//primeira versão funcional
#include "Analysis.hh"
#include "G4AnalysisManager.hh"

Analysis::Analysis()
{
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->SetVerboseLevel(1);
    analysisManager->SetNtupleMerging(true);

    
    analysisManager->CreateNtuple("DetectorData", "Hits in Sensitive Detector");
    analysisManager->CreateNtupleIColumn("hitID");
    analysisManager->CreateNtupleDColumn("energyDeposit");
    analysisManager->CreateNtupleDColumn("positionX");
    analysisManager->CreateNtupleDColumn("positionY");
    analysisManager->CreateNtupleDColumn("positionZ");
    analysisManager->FinishNtuple();
}

Analysis::~Analysis()
{
}

