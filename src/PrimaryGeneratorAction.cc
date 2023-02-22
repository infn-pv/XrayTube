// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
//#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  G4int n_particle = 1; 
  fParticleGun  = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;

//SORGENTE: ELETTRONI DA 250 keV CHE INCIDONO SU UNA LASTRA DI TUGSTENO
//PHYSICS LIST: X-RAYS DA BREMMSTRAHLUNG + X-RAYS CARATTERISTICI FILTRO IN RAME 
  
  G4ParticleDefinition* particle
   = particleTable->FindParticle(particleName="e-");
    
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0)); 
  fParticleGun->SetParticleEnergy(250*keV);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of each event
  //

  // In order to avoid dependence of PrimaryGeneratorAction
  // on DetectorConstruction class we get World Volume
  // from G4LogicalVolumeStore.


  if (!fSourceTub)
  {
    G4LogicalVolume* SourceTub
      = G4LogicalVolumeStore::GetInstance()->GetVolume("XTub_void");
    if ( SourceTub ) fSourceTub = dynamic_cast<G4Tubs*>(SourceTub->GetSolid());
  }

  if ( fSourceTub ) {
  
  }
  else  {
    G4ExceptionDescription msg;
    msg << "World volume of box shape not found.\n";
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }

  G4double size = 0.5*cm;

  G4double x0 = -40*mm ;
  G4double y0 = size*G4UniformRand();
  G4double z0 = 371.7*mm + size*G4UniformRand();

  fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0)); 

  fParticleGun->GeneratePrimaryVertex(anEvent);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

