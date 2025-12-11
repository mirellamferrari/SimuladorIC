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

