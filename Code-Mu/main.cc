#include "UserDetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SensitiveDetector.hh"
#include "DetectorHit.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
//#include "G4UIExecutive.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"


#include "G4Timer.hh"
#include "ctime"
#include "cstdio"

int main(int argc,char** argv)
{
    
    // Run manager
	//
	G4cout<<"main: init G4RunManager"<<G4endl;
	G4RunManager * runManager = new G4RunManager;

	// User Initialization classes (mandatory)
	//
	UserDetectorConstruction* detector = new UserDetectorConstruction;
    detector->SetArgument(argv[1]);
	runManager->SetUserInitialization(detector);
    
    
	G4VUserPhysicsList* physics = new PhysicsList;
	runManager->SetUserInitialization(physics);

	// User Action classes
	//
	G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction();
	runManager->SetUserAction(gen_action);
	//
    //
	RunAction* runAction = new RunAction;
    runAction->SetArgument(argv[1]);
	runManager->SetUserAction(runAction);
    //
	//
	G4UserEventAction* event_action = new EventAction;
	runManager->SetUserAction(event_action);
	//

	// Initialize G4 kernel
	//
	runManager->Initialize();
    
    // Initialize vis manager
    //
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    
    
    // Get the pointer to the User Interface manager
    if (*(argv[2])=='1') {
         G4UImanager * UI = G4UImanager::GetUIpointer();
        G4String command = "/control/execute ";
        G4String fileName = "vis.mac";
        UI->ApplyCommand(command+fileName);
        
/*         G4UIExecutive* UI = new G4UIExecutive(argc, argv);
        if (UI->IsGUI()) {
            visManager->Initialize();
        }
        UI->SessionStart();
        delete UI; */
    }
    
    else{
        G4int numberOfEvent = 1000; 
        runManager->BeamOn(numberOfEvent);
    }
    
    
    //Interactive mode
    //
    //G4UIsession* session = new G4UIterminal(new G4UItcsh);
    //session->SessionStart();
    
    //delete session;

        delete visManager;
        
        delete runManager;
	
    std::cout << '\a'; //Beeps once when it's done
    
	return 0;
}

