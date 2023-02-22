// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Region;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(G4bool DetectorMode);
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

    G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }
    //G4LogicalVolume* GetSpectrumVolume() const { return fSpectrumScore; } 
    G4LogicalVolume* GetSpectrumVolume2() const { return fFilteredSpectrum; } //dopo il filtro

    inline G4VPhysicalVolume* GetWorld() const {return physWorld;};
    inline G4double GetSize() const                  {return fBoxSize;};      
    inline const G4Material* GetMaterial() const     {return fMaterial;};

  protected:
    G4LogicalVolume* fScoringVolume = nullptr;
    //G4LogicalVolume* fSpectrumScore = nullptr;
    G4LogicalVolume* fFilteredSpectrum = nullptr;

    G4VPhysicalVolume*    physWorld;
    G4LogicalVolume*      logicWorld;
    G4Box*                solidWorld;

    G4double              fBoxSize;
    G4Material*           fMaterial;

  private:
    G4bool fDetMode;

};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
