
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
* PrimaryGeneratorAction

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
    // Posição do feixe 
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
* EventAction 

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

------------------------------------------------------------------------------------------------------------------------------

* Sensitive Detector está ligado?
* Geometria como está? (muito improvavél)
Resolver o problema das energia gerada ser 0 

------------------------------------------------------------------------------------------------------------------------------
*UserDetector

.hh
Adicionei 

virtual void ConstructSDandField();

No Geant4 moderno precisa disso! pois o método ConstructSDandField() é o local apropriado para a criação e associação de Sensitive Detectors. Dessa forma, o Geant4 garante que os detectores sensíveis sejam corretamente inicializados após a construção da geometria.

----------------------------------
.cc
Aqui fiz diversas mudanças que incrementou no funcionamento do código, adicionei para evitar comportamento indefinido causado pelo uso de um ponteiro não inicializado.

UserDetectorConstruction::UserDetectorConstruction() : G4VUserDetectorConstruction()
, argument(nullptr) 
{}

----------------------------------

além disso, fiz a seguinte modificação abaixo para que nenhum argumento seja fornecido na linha de comando, o código assume um valor padrão, evitando falhas de execução.

if (!argument) {
    argument = (char*)"1"; 
}

----------------------------------

também adicionei o script abaixo, o material do volume sensível foi alterado para um material denso presente no banco de dados NIST do Geant4, para validar  a deposição de energia no detector:

G4NistManager::Instance()->FindOrBuildMaterial("G4_SODIUM_IODIDE"); //ADICIONADO
    G4LogicalVolume* logicalScintillator = //ADICIONADO
    new G4LogicalVolume(scintillator, testMat, "Scintillator");

----------------------------------

Exclui evitando inicializações fora do fluxo recomendado pelo Geant4:

//Sensitive Detector
    //
    // G4String nameSD = "TrackerSD";
    
    // SensitiveDetector* aTrackerSD = new SensitiveDetector(nameSD, "DetectorHitsCollection");
    // G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    // SetSensitiveDetector("Scintillator", aTrackerSD, true);

----------------------------------

e fiz adição responsável pela definição do detector sensível:

void UserDetectorConstruction::ConstructSDandField()
{
    G4String nameSD = "TrackerSD";

    SensitiveDetector* aTrackerSD =
        new SensitiveDetector(nameSD, "DetectorHitsCollection"); //ADICIONADO

    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

    SetSensitiveDetector("Scintillator", aTrackerSD, true);
}

------------------------------------------------------------------------------------------------------------------------------

* EventAction.cc
foi corrigido o acesso aos hits do evento. O código anterior assumia um índice fixo (GetHC(0)), o que não é garantido pelo Geant4 Atual. A nova implementação utiliza o identificador correto da coleção obtido via G4SDManager, além de incluir verificações de ponteiros nulos (hce e hits).

#include "G4SDManager.hh"  // ADICIONADO

G4HCofThisEvent* hce = anEvent->GetHCofThisEvent();
if (!hce) return;  // ADICIONADO

static G4int hcID = -1;
if (hcID < 0) {
    hcID = G4SDManager::GetSDMpointer()
               ->GetCollectionID("DetectorHitsCollection");
}

DetectorHitsCollection* hits =
    static_cast<DetectorHitsCollection*>(hce->GetHC(hcID));

if (!hits) {  // ADICIONADO
    G4cerr << "HitsCollection nao encontrada!" << G4endl;
    return;
}

------------------------------------------------------------------------------------------------------------------------------

* PrimaryGenerator.cc

Para conseguir ter algum tipo de diagnośtico, no Primary Generator foi temporariamente configurado para emitir elétrons com energia fixa de 1 MeV, posicionados no centro do volume sensível e com direção de momento definida. Essa modificação permitiu verificar se a ausência de energia depositada estava relacionada à física da interação do fóton ou a problemas na implementação do detector sensível. A ausência de energia depositada nos eventos foi atribuída à configuração geométrica entre o gerador primário e o volume sensível. Inicialmente, as partículas primárias eram geradas fora do detector e com direções aleatórias, resultando em baixa probabilidade de interação. Após posicionar a fonte e alinhar a direção do feixe em relação ao scintillator, observou-se a criação de hits e a contabilização correta da energia depositada por evento.

------------------------------------------------------------------------------------------------------------------------------

Nesta etapa, como mencionado anteriormente, foi utilizada uma configuração temporária da simulação com o objetivo de validar o funcionamento do sensitive detector, da geração de partículas e do fluxo de análise e a produção de histogramas no ROOT. A partícula primária simulada foi um elétron, com energia fixa de 1 MeV, gerado dentro do volume do scintillator e com direção fixa apontando para o detector. Com isso, essa configuração garantiu que a partícula interagisse diretamente com o material.

O histograma analisado no ROOT corresponde à energia cinética dos elétrons secundários (que tem baixa energia), preenchido durante as interações registradas pelo SensitiveDetector. No gráfico, o eixo x representa a energia cinética (KEnergy) em MeV, enquanto o eixo y representa o número de ocorrências (contagens) para cada intervalo de energia. É importante destacar que cada entrada do histograma não representa a energia total depositada em um evento, mas sim a energia cinética de elétrons secundários produzidos em cada interação individual. 

------------------------------------------------------------------------------------------------------------------------------
