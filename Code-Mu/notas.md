<<<<<<< HEAD
Anotações sobre o código: sugestões, dúvidas e observações gerais
------------------------------------------------------------------------------------------------------------------------------

* Analysis

Na pasta src, não tem "Analysis.cc" - Criar! ele faz o root ficar funcional
lembrete! no Analysis.hh está //#include "g4root.hh" //#include "g4xml.hh" - isso talvez possa ser um problema no Analysis.hh

na versão atual do Geant4 não existe mais: 
#include "G4RootAnalysisManager.hh"
#include "G4RootFileManager.hh"
#include "G4RootNtupleFileManager.hh"
#include "G4AnalysisManagerState.hh"

apenas #include "G4AnalysisManager.hh" é viável! ele supre tudo o que o Root precisa.

Sobre o Analisys.hh = está vázio, então preciso "encher" ele, então eu fiz o mínimo para que ele funcione e não problematize o .cc (funcionou!)

------------------------------------------------------------------------------------------------------------------------------
*PrimaryGeneratorAction

Quando rodo o código atual, o tanto de energia gerada é 0 eV.
O código gera energias negativas com frequência:

média = 1.000.000.000 eV = 1 GeV

desvio padrão = 50 eV

distribuição gaussiana → pode gerar valores menores que zero, por isso no log aparece 0 eV.
1. Troquei G4RandGauss::shoot(1000000000, 50) * eV;
por 
double energy = std::max(0.0, G4RandGauss::shoot(1*GeV, 50*eV));
Assim a energia não ficará mais negativa

Nessa parte: gun->SetParticlePosition(G4ThreeVector(0.,0.,0.));
O feixe está sendo gerado longe do detector? O Detector é pequeno, a chance de acerto é muito baixa! 
Energia grande + material pequeno → pouca interação

Quero garantir que os gamas acertem no detector!

Troquei 
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

por

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Posição do feixe (exemplo: 5 cm antes do detector)
    gun->SetParticlePosition(G4ThreeVector(0., 0., -5*cm));

    // Direção fixa para o detector no centro
    gun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));

    // Energia gaussiana
    double energy = std::max(0.0, G4RandGauss::shoot(1*GeV, 50*eV));
    gun->SetParticleEnergy(energy);

    // Criação do vértice primário
    gun->GeneratePrimaryVertex(anEvent);
}


------------------------------------------------------------------------------------------------------------------------------
*EventAction 

no código atual 
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

Isso está problematizando a geração de energia.
➜ Só soma energia depositada
➜ Gamas só depositam energia se interagirem 
➜ Se não acertarem o detector → nenhum hit → 0 energia depositada
➜ Então fica Total Energy = 0 eV

Nessa parte: DetectorHitsCollection* hits = static_cast<DetectorHitsCollection*>(hce->GetHC(0));
Isso pega SOMENTE hits do seu sensitive detector, então se o gama não interagir = não tem energia!

!! 
=======
Anotações sobre o código: sugestões, dúvidas e observações gerais


* Analysis.cc
Na pasta src, não tem "Analysis.cc" - Criar! ele faz o root ficar funcional
lembrete! no Analysis.hh está //#include "g4root.hh" //#include "g4xml.hh" - isso talvez possa ser um problema no Analysis.hh

na versão atual do Geant4 não existe mais: 
#include "G4RootAnalysisManager.hh"
#include "G4RootFileManager.hh"
#include "G4RootNtupleFileManager.hh"
#include "G4AnalysisManagerState.hh"

apenas #include "G4AnalysisManager.hh" é viável! ele supre tudo o que o Root precisa.

Sobre o Analisys.hh = está vázio, então preciso "encher" ele, então eu fiz o mínimo para que ele funcione e não problematize o .cc (funcionou!)
