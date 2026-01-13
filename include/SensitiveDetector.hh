#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"
#include "G4TouchableHistory.hh"
#include "DetectorHit.hh"
#include <vector>


class G4Step;
class G4HCofThisEvent;

class SensitiveDetector : public G4VSensitiveDetector
{
    public:
    SensitiveDetector(const G4String& name,
                      const G4String& hitsCollectionName);
        virtual ~SensitiveDetector();

        virtual void Initialize(G4HCofThisEvent*);
        virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
        virtual void EndOfEvent(G4HCofThisEvent*);
    
    private:
        DetectorHitsCollection* fHitsCollection;


};
#endif

