// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//
//

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;
class HistoManager;

/// Run action class
///
/// In EndOfRunAction(), it calculates the dose in the selected volume
/// from the energy deposit accumulated via stepping and event actions.
/// The computed dose is then printed on the screen.

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void   EndOfRunAction(const G4Run*) override;
};

#endif


