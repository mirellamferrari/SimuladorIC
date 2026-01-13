#include "globals.hh"
#include "PhysicsList.hh"
#include "G4PhysicsListHelper.hh"

#include "G4EmPenelopePhysics.hh"
//#include "G4EmStandardPhysics.hh"

#include "G4PenelopeComptonModel.hh"
#include "G4ComptonScattering.hh"

#include "G4PenelopePhotoElectricModel.hh"
#include "G4PhotoElectricEffect.hh"
#include "G4LivermorePhotoElectricModel.hh"

#include "G4RayleighScattering.hh"
#include "G4PenelopeRayleighModel.hh"

#include "G4PenelopeIonisationModel.hh"
#include "G4MuIonisation.hh"
#include "G4eIonisation.hh"

#include "G4PenelopeBremsstrahlungModel.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4eBremsstrahlung.hh"

#include "G4UniversalFluctuation.hh"

#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"

#include "G4Decay.hh"
#include "G4DecayWithSpin.hh"
#include "G4ProcessManager.hh"

#include "G4MuPairProduction.hh"

#include "G4VAtomDeexcitation.hh"
#include "G4UAtomicDeexcitation.hh"
#include "G4LossTableManager.hh"
//#include "G4EmProcessOptions.hh"

#include "G4Gamma.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Proton.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4NeutrinoMu.hh"
#include "G4AntiNeutrinoMu.hh"
#include "G4TauMinus.hh"
#include "G4TauPlus.hh"



#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
// #include "G4EmProcessOptions.hh"

//
// This class defines all the particles and physical processes they can undergo.
//

PhysicsList::PhysicsList():  G4VModularPhysicsList()
{
    
    SetVerboseLevel(0);
    defaultCutValue = 1.0*mm;
    
}

PhysicsList::~PhysicsList()
{}

void PhysicsList::ConstructParticle()
{

    //Electron
    G4Electron::ElectronDefinition();

    //Electronic AntiNeutrino
    G4AntiNeutrinoE::AntiNeutrinoEDefinition();

    //Electronic Neutrino
    G4NeutrinoE::NeutrinoEDefinition();

    //Gamma
    G4Gamma::GammaDefinition();

    //Muon 
    G4MuonMinus::MuonMinusDefinition();
    G4MuonPlus::MuonPlusDefinition();

    //Muonic AntiNeutrino
    G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();

    //Muonic Neutrino
    G4NeutrinoMu::NeutrinoMuDefinition();

    //Positron
    G4Positron::PositronDefinition();

    //Proton
    G4Proton::ProtonDefinition();

    //Tau
    G4TauMinus::TauMinusDefinition();
    G4TauPlus::TauPlusDefinition();
    
}

void PhysicsList::ConstructProcess()
{
    AddTransportation();
    
    //RegisterPhysics( new G4EmPenelopePhysics() );
    //RegisterPhysics( new G4EmStandardPhysics() );
    
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    
    auto particleIterator=GetParticleIterator();
    particleIterator->reset();

    while( (*particleIterator)() ){
        G4ParticleDefinition* particle = particleIterator->value();
        G4String particleName = particle->GetParticleName();
    
    
        //
        //Muon processes
        //
        if (particleName == "mu+" ||
            particleName == "mu-") {
   
            //Muon Ionisation 
              G4MuIonisation* MuIoni = new G4MuIonisation();
              ph->RegisterProcess(MuIoni, particle);
   
            //Bremsstrahlung
              ph->RegisterProcess(new G4MuBremsstrahlung(), particle);
            
 	    //Multiple Scattering 
              ph->RegisterProcess(new G4MuMultipleScattering(), particle);

	    //Pair Production
	      G4MuPairProduction *Mu = new G4MuPairProduction();
	      ph->RegisterProcess(Mu, particle);

            //Decay 
              G4Decay* theDecayProcess = new G4DecayWithSpin();

	      if (particleName == "mu-") {
		  G4ParticleDefinition* muPlus = G4MuonPlus::MuonPlusDefinition();
		  G4ProcessManager* muPlusManager = muPlus->GetProcessManager();
		  muPlusManager->AddProcess(theDecayProcess);
		  muPlusManager->SetProcessOrdering(theDecayProcess, idxPostStep);
		  muPlusManager->SetProcessOrdering(theDecayProcess, idxAtRest);
	      }
	    
	       else if (particleName == "mu-") {
		  G4ParticleDefinition* muMinus = G4MuonMinus::MuonMinusDefinition();
		  G4ProcessManager* muMinusManager = muMinus->GetProcessManager();
		  muMinusManager->AddProcess(theDecayProcess);
		  muMinusManager->SetProcessOrdering(theDecayProcess, idxPostStep);
		  muMinusManager->SetProcessOrdering(theDecayProcess, idxAtRest);
	      }

	  
       }

        //
        //Gamma processes
	//
	 if (particleName == "gamma") {
    
            //Compton Scattering
            G4ComptonScattering* theComptonScattering = new G4ComptonScattering();
            ph->RegisterProcess(theComptonScattering, particle);
    
            //Photo Electric Effect
            G4PhotoElectricEffect* thePhotoEletric = new G4PhotoElectricEffect();
            //G4LivermorePhotoElectricModel* photoEletric = new G4LivermorePhotoElectricModel();
            ph->RegisterProcess(thePhotoEletric, particle);
            
            //Rayleigh Scattering
            G4RayleighScattering* theRayleigh = new G4RayleighScattering();
            ph->RegisterProcess(theRayleigh, particle);
        }


        //
        //Electron processes
        //
        else if (particleName == "e-") {
           
            //e- Ionisation
            G4eIonisation* eIoni = new G4eIonisation();
            ph->RegisterProcess(eIoni, particle);
    
            //Bremsstrahlung
            G4eBremsstrahlung* eBrem = new G4eBremsstrahlung();
            ph->RegisterProcess(eBrem, particle);
            
            //Multiple Scattering
            ph->RegisterProcess(new G4eMultipleScattering(), particle);
        }
    
    }
    
    // Em options
    //G4EmProcessOptions opt;
    
    // Ionization
    //opt.SetSubCutoff(true);
    
    //
    // Deexcitation
    //
    G4VAtomDeexcitation* deexcitation = new G4UAtomicDeexcitation();
    G4LossTableManager::Instance()->SetAtomDeexcitation(deexcitation);
    deexcitation->SetFluo(true);
    
}


void PhysicsList::SetCuts()
{
    SetCutsWithDefault();
}
