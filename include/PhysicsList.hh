// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

class DetectorConstruction;
class PhysicsListMessenger;
class G4VPhysicsConstructor;
class G4VProcess;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhysicsList: public G4VModularPhysicsList
{
  public:
    PhysicsList(DetectorConstruction*);
   ~PhysicsList();

    virtual void ConstructParticle();
    virtual void ConstructProcess();
    void AddPhysicsList(const G4String& name);
    
    void AddDecay();
    void AddRadioactiveDecay();
    void AddStepMax();

    void GetRange(G4double);
    G4VProcess*  GetProcess(const G4String&) const;

  private:    
    G4VPhysicsConstructor* fEmPhysicsList;
    G4String               fEmName;
    
    DetectorConstruction* fDet;
    PhysicsListMessenger* fMessenger;         
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

