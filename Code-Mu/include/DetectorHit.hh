#ifndef DetectorHit_h
#define DetectorHit_h 1

#include <vector>
#include <utility>
#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4TouchableHistory.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

//class G4Step;
//class G4HCofThisEvent;

class DetectorHit : public G4VHit
{
    public:
    DetectorHit();
    DetectorHit(const DetectorHit&);
    virtual ~DetectorHit();
    
    // Operators
    const DetectorHit& operator=(const DetectorHit&);
    G4int operator==(const DetectorHit&) const;
    
    inline void* operator new(size_t);
    inline void operator delete(void*);
    
    //virtual void Draw() const;
    virtual void Print();
    
    // Set methods
    void SetEnergyDeposit(G4double energy) {energyDeposit = energy;};
    void SetPosition(G4ThreeVector pos) {position = pos;};
    void SetParticleName(G4String name) {particleName = name;};
    void SetProcessName(G4String name) {processName = name;};
    void SetParticleID(G4int iD) {particleID = iD;};
    
    // Get methods
    G4double GetEnergyDeposit() {return energyDeposit;};
    G4ThreeVector GetPosition(){ return position;};
    G4String GetParticleName() {return particleName;};
    G4String GetProcessName() {return processName;};
    G4int GetParticleID() {return particleID;};
    
    private:
    
    G4double energyDeposit;
    G4ThreeVector position;
    G4String particleName;
    G4String processName;
    G4int particleID;
    
};

typedef G4THitsCollection<DetectorHit> DetectorHitsCollection;

extern G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator;

inline void* DetectorHit::operator new(size_t)
{
    if(!DetectorHitAllocator)
        DetectorHitAllocator = new G4Allocator<DetectorHit>;
    return (void *) DetectorHitAllocator->MallocSingle();
}

inline void DetectorHit::operator delete(void *aHit)
{
    DetectorHitAllocator->FreeSingle((DetectorHit*) aHit);
}

#endif

