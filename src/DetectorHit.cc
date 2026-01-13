#include "DetectorHit.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsCollection.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4TouchableHistory.hh"
#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

#include "G4RunManager.hh"
//#include "RunAction.hh"

G4ThreadLocal G4Allocator<DetectorHit>* DetectorHitAllocator=0;

DetectorHit::DetectorHit()
: G4VHit(),
energyDeposit(0.),
position(G4ThreeVector()),
particleName(""),
processName(""),
particleID(-1)
{}

DetectorHit::~DetectorHit()
{}


DetectorHit::DetectorHit(const DetectorHit& right) :
G4VHit()
{
    energyDeposit = right.energyDeposit;
    position = right.position;
    particleName = right.particleName;
    processName = right.processName;
    particleID = right.particleID;
}

const DetectorHit& DetectorHit::operator=(const DetectorHit& right)
{
    energyDeposit = right.energyDeposit;
    position = right.position;
    particleName = right.particleName;
    processName = right.processName;
    particleID = right.particleID;
    return *this;
}

G4int DetectorHit::operator==(const DetectorHit& right) const
{
    return (this==&right) ? 1 : 0;
}

//void B2TrackerHit::Draw()
//{}

void DetectorHit::Print()
{
    G4cout
    << "Process: " << std::setw(7) << processName
    << "\n ID: "  << std::setw(7) << particleID
    << "\n Edep: "
    << std::setw(7) << G4BestUnit(energyDeposit,"Energy")
    << "    Position in x: "
    << std::setw(7) <<  G4BestUnit(position.getX(), "Length")
    << G4endl;

}
