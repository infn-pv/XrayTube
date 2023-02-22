// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "Analysis.hh"
#include <string>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction() //+std::to_string(run->GetRunID())
{

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->SetVerboseLevel(1);  
  man-> SetFirstNtupleId(1);
  // Creating ntuples
  man->CreateNtuple("Ek_", "Kinetic Energy in sensitive volume");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->FinishNtuple();
  man->CreateNtuple("Ed_","Energy Deposition in sensitive volume");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->CreateNtupleDColumn("P");
  man->FinishNtuple();
 /* man->CreateNtuple("ESpec","Kinetic Energy in WindowXTub");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->FinishNtuple();
  man->CreateNtuple("EdInESpec","Energy Deposition");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->FinishNtuple();*/
  man->CreateNtuple("ESpecFiltered_","Kinetic Energy in WindowCube_plastic");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->FinishNtuple();
  man->CreateNtuple("EdInESpecFiltered_","Energy Deposition In WindowCube_plastic ");
  man->CreateNtupleDColumn("E");
  man->CreateNtupleDColumn("W");
  man->CreateNtupleDColumn("P");
  man->FinishNtuple();
  man->CreateNtuple("TotalEdepFilt_","Total energy dep in filter volume");
  man->CreateNtupleDColumn("E");
  man->FinishNtuple();
  man->CreateNtuple("TotalEdepScoring_","Total energy dep in sensitive volume");
  man->CreateNtupleDColumn("E");
  man->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* run)
{ 

  G4AnalysisManager* man = G4AnalysisManager::Instance();
  std::string runID = std::to_string(run->GetRunID());
  G4String fileName = "Results_";
  man->OpenFile(fileName+runID);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* run)
{
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();

}

