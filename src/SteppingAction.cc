// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//
//
#include "G4Types.hh"

#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

#include <G4Gamma.hh>
#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* eventAction)
: fEventAction(eventAction)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  if (!fScoringVolume) {
    const DetectorConstruction* detConstruction
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detConstruction->GetScoringVolume();
  }
/*
//associa al puntatore l'indirizzo del volume logico puntato da fSpectrumScore in DetectorConstruction.cc
  if (!fSpectrumScore) {
    const DetectorConstruction* det
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fSpectrumScore = det->GetSpectrumVolume();
  }*/
//associa al puntatore l'indirizzo del volume logico della WindowCube_air (dopo il filtro)
  if (!fFilteredSpectrum) {
    const DetectorConstruction* detC
      = static_cast<const DetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fFilteredSpectrum = detC->GetSpectrumVolume2();
  }


  // get volume of the current step
  G4LogicalVolume* volume
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
  /*G4LogicalVolume* volume_post
    = step->GetPostStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();*/
      
  //fEventAction->GetVolumeFromStep(volume);

  // check if we are in scoring volume
  //if (volume !=fSpectrumScore && volume!=fScoringVolume && volume!=fFilteredSpectrum) return;
 if (volume!=fScoringVolume && volume!=fFilteredSpectrum) return;
 
  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  G4double pWeight = step->GetTrack()->GetWeight();
  G4double KinEnergy = step->GetTrack()->GetKineticEnergy();

  //collect number of particle and kinetick energy
  //if (step->GetTrack()->GetParticleDefinition()!=G4Gamma::Gamma()) return;

  if (volume==fScoringVolume ){
    fEventAction->AddE(0,edepStep*pWeight);
  }
  else if(volume==fFilteredSpectrum){
    fEventAction->AddE(edepStep*pWeight,0);
  }

  //
  G4AnalysisManager* man = G4AnalysisManager::Instance();
  
  if (step->GetTrack()->GetParticleDefinition()==G4Gamma::Gamma()){
	  if (volume == fScoringVolume && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
	      man->FillNtupleDColumn(1, 0, KinEnergy);
	      man->FillNtupleDColumn(1, 1, pWeight);
	      man->AddNtupleRow(1);
	  }
	  else if(volume == fFilteredSpectrum && step->GetPreStepPoint()->GetStepStatus() == fGeomBoundary){
	      man->FillNtupleDColumn(3, 0, KinEnergy);
	      man->FillNtupleDColumn(3, 1, pWeight);
	      man->AddNtupleRow(3);
	  }
  }

  if (volume == fScoringVolume) {
    // then store the kinetic energy of the particle
    if(edepStep>0){
      man->FillNtupleDColumn(2, 0, edepStep);
      man->FillNtupleDColumn(2, 1, pWeight);
      if (step->GetTrack()->GetParticleDefinition()==G4Gamma::Gamma()){
        man->FillNtupleDColumn(2, 2, 1);
      } 
      else{
        man->FillNtupleDColumn(2, 2, 0);
      }
      man->AddNtupleRow(2);
    }
  } /*
  else if (volume == fSpectrumScore) {
    // then store the kinetic energy of the particle
    man->FillNtupleDColumn(3, 0, KinEnergy);
    man->FillNtupleDColumn(3, 1, pWeight);
    man->AddNtupleRow(3);
    man->FillNtupleDColumn(4, 0, edepStep);
    man->FillNtupleDColumn(4, 1, pWeight);
    man->AddNtupleRow(4);
  }  */       
   else if (volume == fFilteredSpectrum) {
    // then store the kinetic energy of the particle
    if(edepStep>0){
      man->FillNtupleDColumn(4, 0, edepStep);
      man->FillNtupleDColumn(4, 1, pWeight);
      if (step->GetTrack()->GetParticleDefinition()==G4Gamma::Gamma()){
        man->FillNtupleDColumn(4, 2, 1);
      } 
      else{
        man->FillNtupleDColumn(4, 2, 0);
      }
      man->AddNtupleRow(4);
    }
   } 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
