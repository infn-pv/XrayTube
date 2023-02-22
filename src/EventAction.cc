// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#include "EventAction.hh"
#include "RunAction.hh"
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* runAction)
: fRunAction(runAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event*)
{
  fEDepfilt = 0;
  fEDepsens = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::AddE(G4double edepf, G4double edeps)
{
  fEDepfilt   += edepf;
  fEDepsens   += edeps;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event*)
{
   G4AnalysisManager* man = G4AnalysisManager::Instance();
  // if a particles has crossed the volume
  if (fEDepfilt>0) {
    man->FillNtupleDColumn(5, 0, fEDepfilt);
    man->AddNtupleRow(5);
  }
  // if a particles has crossed the volume
  if (fEDepsens>0) {
    man->FillNtupleDColumn(6, 0, fEDepsens);
    man->AddNtupleRow(6);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
