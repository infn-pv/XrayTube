// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#include "PhysicsListMessenger.hh"

#include "PhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::PhysicsListMessenger(PhysicsList* pPhys)
:G4UImessenger(),fPhysicsList(pPhys),
 fPhysDir(0),
 fRCmd(0),    
 fListCmd(0)
{ 
  fPhysDir = new G4UIdirectory("/XrayTube/phys/");
  fPhysDir->SetGuidance("physics list commands");
  
  fRCmd = new G4UIcmdWithADoubleAndUnit("/XrayTube/phys/getRange",this);
  fRCmd->SetGuidance("get the electron cut for the current material.");
  fRCmd->SetParameterName("energy",false);
  fRCmd->SetRange("energy>0.");
  fRCmd->SetUnitCategory("Energy");  
  fRCmd->AvailableForStates(G4State_Idle);  

  fListCmd = new G4UIcmdWithAString("/XrayTube/phys/addPhysics",this);  
  fListCmd->SetGuidance("Add modula physics list.");
  fListCmd->SetParameterName("PList",false);
  fListCmd->AvailableForStates(G4State_PreInit);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsListMessenger::~PhysicsListMessenger()
{
  delete fRCmd;
  delete fListCmd;
  delete fPhysDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsListMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{            
  if( command == fRCmd )
   { fPhysicsList->GetRange(fRCmd->GetNewDoubleValue(newValue));}
    
  if( command == fListCmd )
   { fPhysicsList->AddPhysicsList(newValue);}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

