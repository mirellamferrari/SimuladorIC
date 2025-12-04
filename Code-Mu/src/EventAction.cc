#include "EventAction.hh"
#include "DetectorHit.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "Analysis.hh"
#include "RunAction.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

//
// This class controls action before and after an event.
// The total energy collected from the hits is stored here as the total deposit energy of that event.
//

EventAction::EventAction()
: G4UserEventAction()
{}

EventAction::~EventAction()
{}

void EventAction::BeginOfEventAction(const G4Event* )
{}

void EventAction::EndOfEventAction(const G4Event* anEvent)
{
    //Getting collection of hits of this event
    
    G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();
    DetectorHitsCollection* hits = static_cast<DetectorHitsCollection*>(hce->GetHC(0));
    
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    
    //Filling Number of Hits
    G4int n_hit = hits->GetSize();
    analysisManager->FillH1(0, n_hit);
    
    //Filling the Total Energy Deposited in this event
    G4double totalEnergy = 0.;
    
    for (G4int i=0;i<n_hit;i++)
    {
        DetectorHit* hit = (*hits)[i];
        G4double eDep = hit->GetEnergyDeposit();
        if (eDep>0.)
        {
            totalEnergy += eDep;
        }
    }
    
    analysisManager->FillH1(1, totalEnergy);
    
    G4cout
    << "Total Energy: "
    << std::setw(7) << G4BestUnit(totalEnergy,"Energy")
    << "\n"
    << G4endl;
    
}
