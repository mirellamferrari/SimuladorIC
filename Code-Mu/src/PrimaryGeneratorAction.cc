#include "PrimaryGeneratorAction.hh"
#include "UserDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Gamma.hh"
#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

//
// Primary Generator creates the first particle to be tracked.
// Hence, our beam is defined here.
//
//

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction() {
    
    G4int n_particle = 1;
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle("gamma");
    gun = new G4ParticleGun(n_particle);
    gun->SetParticleDefinition(particle);
    gun->SetParticlePosition(G4ThreeVector(0,0,0));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete gun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {

    
    G4double phi = (G4RandGauss::shoot(0., 1./12))*pi;       //Mean and standard deviation
    G4double theta = G4UniformRand()*twopi;                  //Uniform range from 0 to 2pi
    
    //Spherical coordinates for r=1
    gun->SetParticleMomentumDirection(G4ThreeVector(sin(phi)*cos(theta),sin(phi)*sin(theta),cos(phi)));
    
    //Gamma energy
    //double energy = (G4RandGauss::shoot(1000000000, 50))*eV;
    double energy = std::max(0.0, G4RandGauss::shoot(1*GeV, 50*eV));
    


    
    gun->SetParticleEnergy(energy);
    gun->GeneratePrimaryVertex(anEvent);
}

// void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
// {
//     // Posição do feixe 
//     gun->SetParticlePosition(G4ThreeVector(0., 0., -5*cm));

//     // Direção fixa para o detector no centro
//     gun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

//     // Energia gaussiana
//     double energy = std::max(0.0, G4RandGauss::shoot(1*GeV, 50*eV));
//     gun->SetParticleEnergy(energy);

//     // Criação do vértice primário
//     gun->GeneratePrimaryVertex(anEvent);
// }

// void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
// {
//     // posição inicial do feixe (atrás do detector)
//     gun->SetParticlePosition(G4ThreeVector(0., 0., 0.*mm));

//     // direção apontando para o centro do detector
//     gun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

//     // Energia Gaussiana
//     double energy = std::max(0.0, G4RandGauss::shoot(1*GeV, 50*eV));
//     gun->SetParticleEnergy(energy);

//     // Criar o evento
//     gun->GeneratePrimaryVertex(anEvent);
// }

