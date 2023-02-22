// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

/// Event action class
///

class RunAction;
class G4LogicalVolume;

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction* runAction);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

    void AddE( G4double edepf, G4double edeps);
  

  private:
    RunAction* fRunAction = nullptr;
    G4double fEDepfilt   = 0.;
    G4double fEDepsens   = 0.;
    
    G4LogicalVolume* fScoringVolumeforEdep = nullptr;
    //G4LogicalVolume* fSpectrumScoreforEdep = nullptr;
    G4LogicalVolume* fFilteredSpectrumforEdep = nullptr;
  };

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif



