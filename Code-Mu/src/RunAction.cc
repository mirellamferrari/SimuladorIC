#include "RunAction.hh"
#include "Analysis.hh"
#include "G4Run.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//
// This class is called before and after the run of the simulation.
// The ROOT data files are created here.
//

RunAction::RunAction()
{
    int nBins = 2*600;
    // Creating 1D histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->CreateH1("H0", "Number of Hits", 10, -0.5, 9.5);
    analysisManager->CreateH1("H1", "Energy Deposit", nBins, 0., 7*keV);
    analysisManager->CreateH1("H2", "Kinetic Energy for Photo", nBins, 0., 6*keV);
    analysisManager->CreateH1("H3", "Kinetic Energy for Compton", nBins, 0., 6*keV);
    analysisManager->CreateH1("H4", "Kinetic Energy", nBins, 0., 6*keV);
    
    // Creating ntuple
    analysisManager->CreateNtuple("N1", "Primary electrons");
    analysisManager->CreateNtupleDColumn("KEnergy");    // column Id = 0
    analysisManager->CreateNtupleDColumn("Xposition");  // column Id = 1
    analysisManager->CreateNtupleDColumn("Yposition");  // column Id = 2
    analysisManager->CreateNtupleDColumn("Zposition");  // column Id = 3
    analysisManager->CreateNtupleDColumn("Xmomentum");  // column Id = 4
    analysisManager->CreateNtupleDColumn("Ymomentum");  // column Id = 5
    analysisManager->CreateNtupleDColumn("Zmomentum");  // column Id = 6
    analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
   delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    //Creating data.root file
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    if (*argument == '1') {
        analysisManager->OpenFile("DataAr100.root"); 
    }
    
    else if(*argument == '2'){
        analysisManager->OpenFile("DataXe100.root");
    }
    
    else if(*argument == '3'){
        analysisManager->OpenFile("DataCO2.root");
    }
    
    else if(*argument == '4'){
    
        analysisManager->OpenFile("DataAr70.root");
    
    }
    
    else{
        analysisManager->OpenFile("DataXe70.root");
    }
    
}

void RunAction::EndOfRunAction(const G4Run*)
{ 
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

}