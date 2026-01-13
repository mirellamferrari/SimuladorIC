#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class UserDetectorConstruction;
class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        PrimaryGeneratorAction();
        ~PrimaryGeneratorAction();

    public:
        void GeneratePrimaries(G4Event*);

    private:
        G4ParticleGun* gun;
        UserDetectorConstruction* myDetector;
};

#endif
