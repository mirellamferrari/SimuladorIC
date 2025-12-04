//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "UserDetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "globals.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4SDManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"

#include "G4NistManager.hh"


UserDetectorConstruction::UserDetectorConstruction() : G4VUserDetectorConstruction()
{}

UserDetectorConstruction::~UserDetectorConstruction()
{}

G4VPhysicalVolume* UserDetectorConstruction::Construct()
{
    // Get nist material manager
    G4NistManager* nist = G4NistManager::Instance();
    
    //Materials
    G4Material* air  = nist->FindOrBuildMaterial("G4_AIR");
    
    //Creating gas mixture
    
    G4double z, a, fractionOfNobleGas, density,temperature, pressure;
    G4String name, symbol;
    G4int ncomponents, natoms;
    
    a = 39.95*g/mole;
    G4Element* elAr  = new G4Element(name="Argon", symbol="Ar", z= 18., a);
    
    a = 131.29*g/mole;
    G4Element* elXe  = new G4Element(name="Xenon", symbol="Xe", z= 54., a);
    
    a = 16.00*g/mole;
    G4Element* elO  = new G4Element(name="Oxygen", symbol="O", z= 8., a);
    
    a = 12.01*g/mole;
    G4Element* elC  = new G4Element(name="Carbon", symbol="C", z= 6., a);
    
    //The constructor checks the density and automatically sets the state to gas in STP conditions
    //below a given threshold (10 mg/cm3)
    
    density = 1.960*mg/cm3;
    G4Material* CO2 = new G4Material(name="Carbonic Gas", density, ncomponents=2);
    CO2->AddElement(elC, natoms=1);
    CO2->AddElement(elO, natoms=2);
    
    G4Material* gas;
    
    if (*argument == '1') {
        density = 1.780*mg/cm3; //Density for Argon alone
        
        gas = new G4Material(name="Gas", density, ncomponents=1, kStateGas, temperature=298*kelvin, pressure=1*atmosphere);
        
        gas->AddElement(elAr, 1);
    }
    
    else if (*argument == '2') {
        density = 5.894*mg/cm3; //Density for Xenon alone
        
        gas = new G4Material(name="Gas", density, ncomponents=1, kStateGas, temperature=298*kelvin, pressure=1*atmosphere);
        
        gas->AddElement(elXe, 1);
    }
    
    else if (*argument == '3') {
        //CO2 alone
        
        gas = new G4Material(name="Gas", density, ncomponents=1, kStateGas, temperature=298*kelvin, pressure=1*atmosphere);
        
        gas->AddMaterial(CO2, 1);
    }
    
    else if (*argument == '4') {
        
        density = (0.7*1.780+0.3*1.960)*mg/cm3; //Density for mix of Argon and CO2
        
        gas = new G4Material(name="Gas", density, ncomponents=2, kStateGas, temperature=298*kelvin, pressure=1*atmosphere);
        
        fractionOfNobleGas = 70.0*perCent;
        gas->AddElement(elAr, fractionOfNobleGas);
        gas->AddMaterial(CO2, 1-fractionOfNobleGas);
    }
    
    else{
        
        density = (0.7*5.894+0.3*1.960)*mg/cm3; //Density for mix of Xenon and CO2
        
        gas = new G4Material(name="Gas", density, ncomponents=2, kStateGas, temperature=298*kelvin, pressure=1*atmosphere);
        
        fractionOfNobleGas = 70.0*perCent;
        gas->AddElement(elXe, fractionOfNobleGas);
        gas->AddMaterial(CO2, 1-fractionOfNobleGas);
    }
    
    //
    // World
    //
    
    G4Box* world =
        new G4Box("World",                          //its name
            0.5*m, 0.5*m, 0.5*m);                   //its size
    
    G4LogicalVolume* logicalWorld =
        new G4LogicalVolume(world,                  //its solid
                            air,                    //its material
                            "World");               //its name
    
    G4VPhysicalVolume* physicalWorld =
        new G4PVPlacement(0,                        //no rotation
                          G4ThreeVector(),          //at (0,0,0)
                          logicalWorld,             //its logical volume
                          "World",                  //its name
                          0,                        //its mother volume (none)
                          false,                    //no boolean operation
                          0,                        //copy number
                          true);                    //check overlaps
    
    G4Box* scintillator =
        new G4Box("Scintillator",                   //its name
                  25*mm, 25*mm, 25*mm);                //its size
    
    G4LogicalVolume* logicalScintillator =
        new G4LogicalVolume(scintillator,           //its solid
                            gas,                    //its material
                            "Scintillator");        //its name
    
    new G4PVPlacement(0,                            //no rotation
                      G4ThreeVector(0,0,26*mm),      //at (0,0,51*mm)
                      logicalScintillator,          //its logical volume
                      "Scintillator",               //its name
                      logicalWorld,                 //its mother volume (World)
                      false,                        //no boolean operation
                      0,                            //copy number
                      true);                        //check overlaps
    
    
    
    //
    //Sensitive Detector
    //
    G4String nameSD = "TrackerSD";
    
    SensitiveDetector* aTrackerSD = new SensitiveDetector(nameSD, "DetectorHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
    SetSensitiveDetector("Scintillator", aTrackerSD, true);
    
    
    return physicalWorld;
    
}



