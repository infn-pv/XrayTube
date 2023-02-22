// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class.

class ActionInitialization : public G4VUserActionInitialization
{
  public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
