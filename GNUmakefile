# --------------------------------------------------------------
# GNUmakefile for XrayTube module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := XrayTube
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk
include $(G4INSTALL)/config/binmake.gmk

