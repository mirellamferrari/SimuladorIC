#include "DetectorHit.hh"

#include "SensitiveDetector.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include <G4VProcess.hh>
#include "G4RunManager.hh"
#include "RunAction.hh"
#include "Analysis.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

SensitiveDetector::SensitiveDetector(const G4String& name,
                                     const G4String& hitsCollectionName) :
G4VSensitiveDetector(name)
,fHitsCollection(NULL)
{
  collectionName.insert(hitsCollectionName);
}

SensitiveDetector::~SensitiveDetector()
{}

void SensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
    fHitsCollection
    = new DetectorHitsCollection(SensitiveDetectorName, collectionName[0]);

    static G4int hcID = -1;
    if(hcID<0){
        hcID= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    }
    hce->AddHitsCollection( hcID, fHitsCollection );
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
    //Printing particle name in current step
    G4Track* track         = aStep->GetTrack();
    const G4ParticleDefinition* particle = (track->GetParticleDefinition());
    const G4String particleName = particle->GetParticleName();
    
    G4cout << "Particle: " << std::setw(10) << particleName << G4endl;
    
    //Geting process name that the particle underwent
    G4String processName = ((aStep->GetPostStepPoint())->GetProcessDefinedStep())->GetProcessName();
    
    //Getting particle ID
    G4int particleID = track->GetTrackID();
    
    //Energy deposit
    G4double energy = aStep->GetTotalEnergyDeposit();
    
    
    if (energy == 0.) return false; //Particle inside sensitive detector, but no interaction with the medium
    
    //
    //Getting the secondary electron information
    //
    const std::vector<const G4Track*>* secondary = aStep->GetSecondaryInCurrentStep();
    
    int number_secondaries = (*secondary).size();
    
    if (particleName=="gamma" && number_secondaries!=0) {
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        G4double kinEnergy = 0;
        G4ThreeVector secondaryPosition;
        G4ThreeVector momentumDirection;
        for (int i=0; i<number_secondaries; i++) {
            const G4ParticleDefinition* secondaryDefinition = (*secondary)[i]->GetParticleDefinition();
            G4String secondaryName = secondaryDefinition->GetParticleName();
            
            if (secondaryName == "e-") {
                //Kinectic energy of the secondary
                kinEnergy = (*secondary)[i]->GetKineticEnergy();
                analysisManager->FillNtupleDColumn(0, kinEnergy);
                analysisManager->FillH1(4, kinEnergy);
                G4cout << std::setw(10) << G4BestUnit(kinEnergy,"Energy") << G4endl;
                
                if (processName=="phot") {
                    analysisManager->FillH1(2, kinEnergy);
                }
                
                if (processName=="compt") {
                    analysisManager->FillH1(3, kinEnergy);
                }
                
                
                //Secondary position
                secondaryPosition = (*secondary)[i]->GetPosition();
                analysisManager->FillNtupleDColumn(1, secondaryPosition.getX());
                analysisManager->FillNtupleDColumn(2, secondaryPosition.getY());
                analysisManager->FillNtupleDColumn(3, secondaryPosition.getZ());
                
                //Momentum direction
                momentumDirection = (*secondary)[i]->GetMomentumDirection();
                analysisManager->FillNtupleDColumn(4, momentumDirection.getX());
                analysisManager->FillNtupleDColumn(5, momentumDirection.getY());
                analysisManager->FillNtupleDColumn(6, momentumDirection.getZ());
                
                analysisManager->AddNtupleRow();
            }
        }
    }

    //Storing the hit (interation) information of the current step
    DetectorHit* newHit = new DetectorHit();
    
    newHit->SetEnergyDeposit(energy);
    newHit->SetPosition (aStep->GetPostStepPoint()->GetPosition());
    newHit->SetProcessName(particleName);
    newHit->SetProcessName(processName);
    newHit->SetParticleID(particleID);
    
    fHitsCollection->insert(newHit);

    newHit->Print();

    return true;
    
}



void SensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
    G4cout << "Event end!" << G4endl;


}
