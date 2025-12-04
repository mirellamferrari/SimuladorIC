#ifndef UserDetectorConstruction_h
#define UserDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4VPVParameterisation;
class G4UserLimits;

class UserDetectorConstruction : public G4VUserDetectorConstruction
{
    public:

        UserDetectorConstruction();
        virtual ~UserDetectorConstruction();
    
        void SetArgument(char* arg) {argument = arg;};

        virtual G4VPhysicalVolume* Construct();
            
    private:
            
        char* argument;

};

#endif
