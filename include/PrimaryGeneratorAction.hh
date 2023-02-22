// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
//class G4Box;
class G4Tubs;

/// The primary generator action class with particle gun.
///
/// The default kinematic is 250 keV electrons, incidenti su una lastra di tugsteno
/// raggi x prodotti da bremmstrahlung + x caratteristici dovuti al filtro in rame

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    // method from the base class
    void GeneratePrimaries(G4Event*) override;

    // method to access particle gun
    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  private:
    G4ParticleGun* fParticleGun = nullptr; // pointer a to G4 gun class
    //G4Box* fWorldBox = nullptr;
    G4Tubs* fSourceTub = nullptr;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
