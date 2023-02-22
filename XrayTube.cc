// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//
#include "Analysis.hh"
#include <G4Types.hh>

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "G4GenericBiasingPhysics.hh"
#include "ActionInitialization.hh"

#include <G4RunManagerFactory.hh>
#include <G4SteppingVerbose.hh>
#include <G4UImanager.hh>
#include <G4ScoringManager.hh>

#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include "Randomize.hh"
#include "G4ImportanceBiasing.hh"
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"

#include <iostream>

#include <stdlib.h>

#include "G4Types.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  G4cout << "Application starting..." << G4endl;

  // Initialize visualization 
  G4bool interactive = false;
  G4bool serial = false;
  G4bool DetectorMode = false;

  G4String macro = "";
  G4String command = "/control/execute ";

  // Parse command line arguments
  if  (argc == 1)
    {
      interactive = true;
    }
  else
    {
      for (int i = 1; i < argc; i++)
        {
          G4String arg = argv[i];
          if (arg == "-i" || arg == "--interactive"){
              interactive = true;
              continue;
          }
          else if(arg == "-s" || arg == "--serial"){
            serial = true;
          }
          else if (arg == "-c" || arg == "--activate_cells_geometry"){
              DetectorMode = true;
              continue;
          }
          else{
            macro = arg;
          }
        }
    }
  
  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  G4RunManager* runManager = nullptr;
  if(serial){
    runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
  } else {
    runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
  }

  // Set mandatory initialization classes  
  //
  // Detector construction
  DetectorConstruction* det;
  runManager->SetUserInitialization(det = new DetectorConstruction(DetectorMode));
  
  //Physics List 
  PhysicsList* phys = new PhysicsList(det);
  runManager->SetUserInitialization(phys);
  
  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  //G4ScoringManager::GetScoringManager();

  //initialize visualization
  G4VisManager* visManager = nullptr;

  //get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  
  G4UIExecutive* ui = nullptr;
  if (interactive) ui = new G4UIExecutive(argc,argv);
  
  if (ui)  {
   //interactive mode
   visManager = new G4VisExecutive;
   visManager->Initialize();
   UImanager->ApplyCommand(command+macro);
   ui->SessionStart();
   delete ui;
  }
  else  {
   //batch mode
   UImanager->ApplyCommand(command+macro);
  }

  delete visManager;
  delete runManager;

  G4cout << "Application successfully ended.\nBye :-)" << G4endl;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
