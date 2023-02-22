// SPDX-License-Identifier: EUPL-1.2
// Copyright 2023 Istituto Nazionale di Fisica Nucleare
//

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4Tubs.hh"
#include "G4Sphere.hh"

#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4ImportanceBiasing.hh"
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"
#include <iostream>

#include <stdlib.h>

#include "G4Types.hh"
#include "globals.hh"

#include <G4SDManager.hh>
#include <G4MultiFunctionalDetector.hh>
#include <G4VSensitiveDetector.hh>


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


DetectorConstruction::DetectorConstruction(G4bool DetectorMode)
{
  fBoxSize = 100*cm;
  //SetVerboseLevel(1);
  fDetMode = DetectorMode;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

//MATERIALS
 
  G4Material* air_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* C8H8 = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  G4Material* pyrex = nist->FindOrBuildMaterial("G4_Pyrex_Glass");
  G4Material* Polycarbonate = nist->FindOrBuildMaterial("G4_POLYCARBONATE");
  //G4Material* Graphite = nist->FindOrBuildMaterial("G4_GRAPHITE");
  G4Material* Rame = nist->FindOrBuildMaterial("G4_Cu");
  G4Material* Tugsteno = nist->FindOrBuildMaterial("G4_W");
  G4Material* Piombo = nist->FindOrBuildMaterial("G4_Pb");  
  G4Material* vuoto = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* plate_mat = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
  
    //cells
  G4Material* soft_tissue = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRP"); 
  //G4Material* soft_tissue = nist->FindOrBuildMaterial("G4_TISSUE_SOFT_ICRU-4"); 

   //DMEM
  G4Element* elNa = new G4Element("Sodium","Na",11.,22.9898*g/mole);
  G4Element* elCl = new G4Element("Chlorine","Cl",17.,35.4527*g/mole);
  G4Element* elC = new G4Element("Carbon","C",6.,12.0107*g/mole);
  G4Element* elH = new G4Element("Hydrogen","H",1.,1.01*g/mole);
  G4Element* elO = new G4Element("Oxygen","O",8.,15.999*g/mole);
         
  G4double densityDMEM = 1.0*g/cm3; 
  G4int ncomp = 5; //number of elements in the material

  G4Material* DMEM = new G4Material("DMEM",densityDMEM, ncomp);
  DMEM->AddElement(elH, 10.996*perCent);
  DMEM->AddElement(elC, 0.21*perCent);
  DMEM->AddElement(elCl, 0.349*perCent);
  DMEM->AddElement(elNa, 0.318*perCent);
  DMEM->AddElement(elO, 88.127*perCent);
 
      


//GEOMETRY
  //
  // WORLD
  //
  fMaterial = air_mat;
  G4double world_sizeXY = fBoxSize;
  G4double world_sizeZ  = fBoxSize;

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        fMaterial,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking                   



// 
//CONFIGURAZIONE SPERIMENTALE ---------------------------------------------------------------------------

//CILINDRO PORTANTE
//parte esterna in piombo, spessore 3 mm

  G4double Column_radius  = 5.3*cm;
  G4double Column_lenght  = 10*cm;
  
  G4Tubs* solidColumn =
    new G4Tubs("Column",                    //its name
                0,                         // raggio interno 
                Column_radius,          //raggio esterno  
                0.5*Column_lenght,     //metà altezza, h_tot=120 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicColumn =
    new G4LogicalVolume(solidColumn,            //its solid
                        Piombo,             //its material
                        "Column");         //its name   --> piombo

  G4VPhysicalVolume* physColumn =
  new G4PVPlacement(0,                       // rotation -> asse su x
                    G4ThreeVector(0,0,281.7*mm),         //centre
                    logicColumn,                //its logical volume
                    "Column",              //its name
                    logicWorld,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking 
                    
                    
//interno di aria

  G4double column_int_radius  = 5.*cm;

  G4Tubs* solidColumnINT =
    new G4Tubs("ColumnINT",                    //its name
                0,                         // raggio interno 
                column_int_radius,          //raggio esterno  
                0.5*Column_lenght,     //metà altezza, h_tot=120 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicColumnINT =
    new G4LogicalVolume(solidColumnINT,            //its solid
                        air_mat,             //its material
                        "ColumnINT");         //its name   --> piombo

  G4VPhysicalVolume* physColumnINT =
  new G4PVPlacement(0,                       // rotation -> asse su x
                    G4ThreeVector(0,0,0*mm),         //centre
                    logicColumnINT,                //its logical volume
                    "ColumnINT",              //its name
                    logicColumn,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking 
 
                    
// 
//FILTRO IN RAME 
//

  G4double Filter_height = 1*mm; 
  G4double Filter_diametre = 10*cm; 
 
  G4Tubs* solidFilter =
    new G4Tubs ("Filter",                       //its name
                       0,                         // raggio interno 
                0.5*Filter_diametre,          //raggio esterno  
                0.5*Filter_height,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicFilter =
    new G4LogicalVolume(solidFilter,          //its solid
                        Rame,           //its material -------------> rame
                        "Filter");            //its name

  G4VPhysicalVolume* physFilter =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,9.5*mm),  //centre 
                      logicFilter,            //its logical volume
                      "Filter",               //its name
                      logicColumnINT,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                    
                      

//------------------------------------------------------------------------------------------------------------------------------                                     
// 
// COSTRUZIONE TUBO RADIOGENO/--------------------------------------------------------------------------

  //rivestimento esterno in piombo 

  G4double XTub_radius  = 7*cm;
  G4double XTub_lenght  = 48*cm; //misure prese da articolo su signal
  
  G4Tubs* solidXTub =
    new G4Tubs("XTub",                    //its name
                0,                         // raggio interno 
                XTub_radius,          //raggio esterno  
                0.5*XTub_lenght,     //metà altezza, h_tot=120 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie
  
  solidXTub->SurfaceNormal(G4ThreeVector(1,0,0));

  G4RotationMatrix* XTubRot = new G4RotationMatrix();
  XTubRot->rotateY(90.*deg);

  //sottraiamo  column
  G4SubtractionSolid* solidXTub_sub = new G4SubtractionSolid("solidXTub_sub", solidXTub, solidColumn, XTubRot,G4ThreeVector(-90,0,0));

  G4LogicalVolume* logicXTub =
    new G4LogicalVolume(solidXTub_sub,            //its solid
                        Piombo,             //its material
                        "XTub");         //its name   --> piombo

  G4VPhysicalVolume* physXTub =
  new G4PVPlacement(XTubRot,                       // rotation -> asse su x
                    G4ThreeVector(0,0,371.7*mm),         //centre
                    logicXTub,                //its logical volume
                    "XTub",              //its name
                    logicWorld,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking  
  
//finestra nello spessore di piombo fatta in pyrex
//spessore finestra 3 mm come il riv. in piombo, altezza hip 7 cm

  /*G4double WindowXTub_lenght  = 7.*cm;
  G4double WindowXTub_radius  = 5.7*cm;
  
  G4Tubs* solidWindowXTub =
    new G4Tubs("WindowXTub",                    //its name
                WindowXTub_radius,                         // raggio interno 
                XTub_radius,          //raggio esterno  
                0.5*WindowXTub_lenght,     //metà altezza
                -1.15*M_PI,                        //starting phi 
                +0.3333334*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicWindowXTub =
    new G4LogicalVolume(solidWindowXTub,            //its solid
                        air_mat,             //its material -> devo mettere aria?
                        "WindowXTub");         //its name 

  G4VPhysicalVolume* physWindowXTub =
  new G4PVPlacement(0,                       // no rotation
                    G4ThreeVector(0,0,0),         //centre
                    logicWindowXTub,                //its logical volume
                    "WindowXTub",              //its name
                    logicXTub,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking*/

  //interno riempito di aria

  G4double XTub_air_radius  = XTub_radius - 0.3*cm; //rivestimento in vetro 1 mm
  G4double XTub_air_lenght  = 47.4*cm;

  G4Tubs* solidXTub_air =
    new G4Tubs("XTub_air",                    //its name
                0,                         // raggio interno 
                XTub_air_radius,          //raggio esterno  
                0.5*XTub_air_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  //sottraiamo  column
  G4SubtractionSolid* solidXTub_air_sub = new G4SubtractionSolid("solidXTub_air_sub", solidXTub_air, solidColumn, XTubRot,G4ThreeVector(-90,0,0));

  G4LogicalVolume* logicXTub_air =
    new G4LogicalVolume(solidXTub_air_sub,            //its solid
                        air_mat,             //its material 
                        "XTub_air");         //its name 

  G4VPhysicalVolume* physXTub_air =
  new G4PVPlacement(0,                       // no rotation 
                    G4ThreeVector(0,0,0*mm),         //centre
                    logicXTub_air,                //its logical volume
                    "XTube_air",              //its name
                    logicXTub,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking     

  //rivestimento interno in vetro-pyrex, involucro contenente anodo e sorgente
  
  G4double XTub_glass_radius  = 3.1*cm; //rivestimento in piombo 3 mm
  G4double XTub_glass_lenght  = 12.2*cm; //rivestimento in piombo 3 mm sopra e sotto

  G4Tubs* solidXTub_glass =
    new G4Tubs("XTub_glass",                    //its name
                0,                         // raggio interno 
                XTub_glass_radius,          //raggio esterno  
                0.5*XTub_glass_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicXTub_glass =
    new G4LogicalVolume(solidXTub_glass,            //its solid
                        pyrex,             //its material 
                        "XTub_glass");         //its name  

  G4VPhysicalVolume* physXTub_glass =
  new G4PVPlacement(0,                       // no rotation 
                    G4ThreeVector(0,0,0*mm),         //centre
                    logicXTub_glass,                //its logical volume
                    "XTube_glass",              //its name
                    logicXTub_air,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                

  //interno riempito di vuoto

  G4double XTub_void_radius  = 3*cm; //rivestimento in vetro 1 mm
  G4double XTub_void_lenght  = 12*cm;

  G4Tubs* solidXTub_void =
    new G4Tubs("XTub_void",                    //its name
                0,                         // raggio interno 
                XTub_void_radius,          //raggio esterno  
                0.5*XTub_void_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicXTub_void =
    new G4LogicalVolume(solidXTub_void,            //its solid
                        vuoto,             //its material 
                        "XTub_void");         //its name 

  G4VPhysicalVolume* physXTub_void =
  new G4PVPlacement(0,                       // no rotation 
                    G4ThreeVector(0,0,0*mm),         //centre
                    logicXTub_void,                //its logical volume
                    "XTube_void",              //its name
                    logicXTub_glass,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                     


// ANODO IN TUGSTENO INCLINATO di 30* rispetto all'asse y della geometria del tubo radiogeno.

  G4double Anode_sizeXY = 4*cm; //sistemare le misure-->non le conosciamo, ipotizzo
  G4double Anode_sizeZ  = 1*cm; 
 
  G4Box* solidAnode =
    new G4Box("Anode",                       //its name
       0.5*Anode_sizeXY, 0.5*Anode_sizeXY, 0.5*Anode_sizeZ);     //its size

  G4LogicalVolume* logicAnode =
    new G4LogicalVolume(solidAnode,          //its solid
                        Tugsteno,           //its material  
                        "Anode");            //its name

  G4RotationMatrix* AnodeRot = new G4RotationMatrix();
  AnodeRot->rotateY(30.*deg);

  G4VPhysicalVolume* physAnode =
    new G4PVPlacement(AnodeRot,                     // rotation   
                      G4ThreeVector(0,0,-0.65470053*cm),  //centre 1.15470053-0.5cm 
                      logicAnode,            //its logical volume
                      "Anode",               //its name
                      logicXTub_void,                     //its mother  volume 
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  
  G4String regName[] = {"AnodeRegion"};                    
  G4Region* aRegion = new G4Region("AnodeRegion");
  logicAnode->SetRegion(aRegion);
  aRegion->AddRootLogicalVolume(logicAnode);

 

//BOX OTTURATORI
//box otturatori, rivestimento esterno in piombo

  G4double Cube_sizeX = 20.3*cm;
  G4double Cube_sizeY = 16*cm; //(misura casuale)
  G4double Cube_sizeZ  = 12*cm;   
 
  G4Box* solidCube =
    new G4Box("Cube",                       //its name
       0.5*Cube_sizeX, 0.5*Cube_sizeY, 0.5*Cube_sizeZ);     //its size

  G4LogicalVolume* logicCube =
    new G4LogicalVolume(solidCube,          //its solid
                        Piombo,           //its material 
                        "Cube");            //its name

 G4VPhysicalVolume* physCube =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,171.7*mm),  //centre (distanza tra cover up +6.7mm e cubo 105.0 mm+ metà cubo 60 mm) //171.7
                      logicCube,            //its logical volume
                      "WindowCube",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

//aria dentro la box otturatori

G4double Cube_air_sizeY = 15.4*cm;
G4double Cube_air_sizeX = 19.7*cm;
G4double Cube_air_sizeZ  = 11.4*cm;

  G4Box* solidCube_air =
    new G4Box("Cube_air",                       //its name
       0.5*Cube_air_sizeX, 0.5*Cube_air_sizeY, 0.5*Cube_air_sizeZ);     //its size

  G4LogicalVolume* logicCube_air =
    new G4LogicalVolume(solidCube_air,          //its solid
                        air_mat,           //its material  
                        "Cube_air");            //its name

 G4VPhysicalVolume* physCube_air =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,0*mm),  //centre 
                      logicCube_air,            //its logical volume
                      "Cube_air",               //its name
                      logicCube,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking 
                      
//finestra DOWN plastica

G4double WindowInTheBox_sizeY = 12*cm;
G4double WindowInTheBox_sizeX = 14.3*cm;
G4double WindowInTheBox_sizeZ  = 0.3*cm;

  G4Box* solidWindowInTheBox =
    new G4Box("WindowInTheBox",                       //its name
       0.5*WindowInTheBox_sizeX, 0.5*WindowInTheBox_sizeY, 0.5*WindowInTheBox_sizeZ);     //its size

  G4LogicalVolume* logicWindowInTheBox=
    new G4LogicalVolume(solidWindowInTheBox,          //its solid
                        C8H8,           //its material  
                        "WindowInTheBox");            //its name

 G4VPhysicalVolume* physWindowInTheBox =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-58.5*mm),  //centre 
                      logicWindowInTheBox,            //its logical volume
                      "WindowInTheBox",               //its name
                      logicCube,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking 


//finestra UP aria                
                     
  G4double WindowInTheBoxUP_radius  = column_int_radius;
  G4double WindowInTheBoxUP_lenght  = 0.3*cm;
  
  G4Tubs* solidWindowInTheBoxUP =
    new G4Tubs("WindowInTheBoxUP",                    //its name
                0,                         // raggio interno 
                WindowInTheBoxUP_radius,          //raggio esterno  
                0.5*WindowInTheBoxUP_lenght,     //metà altezza, h_tot=120 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicWindowInTheBoxUP =
    new G4LogicalVolume(solidWindowInTheBoxUP,            //its solid
                        air_mat,             //its material
                        "WindowInTheBoxUP");         //its name   --> piombo

  G4VPhysicalVolume* physWindowInTheBoxUP =
  new G4PVPlacement(0,                       // rotation -> asse su x
                    G4ThreeVector(0,0,58.5*mm),         //centre
                    logicWindowInTheBoxUP,                //its logical volume
                    "WindowInTheBoxUP",              //its name
                    logicCube,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   

  if(fDetMode){
//
//  PRIMA CONFIGURAZIONE: IRRAGGIAMENTO PETRI --------------------------------------------------------------------------------
//
//
// 
//COSTRUZIONE PETRI -------------------------------               
  //
  // PETRI DA 35 mm SPESSORE ESTERNO (SOLO SUP LATERALE)
  //
  G4double petri_radius_int = 17.35*mm; //dimatero interno 34.7 mm
  G4double petri_radius_ext = 17.5*mm;  
  G4double petri_height  = 11.4*mm;   //altezza tot 12.4 mm di cui 1mm è nel tappo in alto
  
  G4Tubs* solidPetri =
    new G4Tubs("Petri",                    //its name
                0, // raggio interno 
                petri_radius_ext, //raggio esterno
                0.5*petri_height,  //metà altezza 
                0., //starting phi
                2*M_PI); //segment angle

  G4LogicalVolume* logicPetri =
    new G4LogicalVolume(solidPetri,            //its solid
                        C8H8,             //its material
                        "Petri");         //its name
  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0) 
                    logicPetri,                //its logical volume
                    "Petri",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   
                    
   G4String regName9[] = {"PetriRegion"};                    
   G4Region* aRegion9 = new G4Region("PetriRegion");
   logicPetri->SetRegion(aRegion9);
   aRegion9->AddRootLogicalVolume(logicPetri); 
                     
  //
  // AIR GAP 8.2 MM
  //  
  G4double petri_air_height  = 8.2*mm;
  
  G4Tubs* solidPetri_int =
    new G4Tubs("Petri_air",                    //its name
                0,                         // raggio interno 
                petri_radius_int,          //raggio esterno
                0.5*petri_air_height,     //metà altezza h_tot=4 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicPetri_int =
    new G4LogicalVolume(solidPetri_int,            //its solid
                        air_mat,             //its material
                        "Petri_air");         //its name
  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,1.6*mm),         //centre 
                    logicPetri_int,                //its logical volume
                    "Petri_air",              //its name
                    logicPetri,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking  
                    
   G4String regName8[] = {"Petri_airRegion"};                    
   G4Region* aRegion8 = new G4Region("Petri_airRegion");
   logicPetri_int->SetRegion(aRegion8);
   aRegion8->AddRootLogicalVolume(logicPetri_int); 
                    
  //
  // COVER UP -> PETRI CUP
  //  
  G4double Cover_up_height  = 1*mm;
  
  G4Tubs* solidCover_up =
    new G4Tubs("Cover_up",                    //its name
                0,                         // raggio interno 
                petri_radius_ext,          //raggio esterno  
                0.5*Cover_up_height,     //metà altezza h_tot=1 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicCover_up =
    new G4LogicalVolume(solidCover_up,            //its solid
                        C8H8,             //its material
                        "Cover_up");         //its name
  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,6.2*mm),         //centre
                    logicCover_up,                //its logical volume
                    "Cover_up",              //its name
                    logicWorld,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                                   


   G4String regName7[] = {"Cover_upRegion"};                    
   G4Region* aRegion7 = new G4Region("Cover_upRegion");
   logicCover_up->SetRegion(aRegion7);
   aRegion7->AddRootLogicalVolume(logicCover_up);                       
  
  //
  // BASE PETRI -> PETRI BOTTOM
  //  
  
  G4Tubs* solidBase =
    new G4Tubs("Base",                    //its name
                0,                         // raggio interno 
                petri_radius_ext,          //raggio esterno
                0.5*Cover_up_height,     //metà altezza h_tot=1 mm
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicBase =
    new G4LogicalVolume(solidBase,            //its solid
                        C8H8,             //its material
                        "Base");         //its name
  
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,-6.2*mm),         //centre at (0,0,3) (base inferiore a 1mm)
                    logicBase,                //its logical volume
                    "Base",              //its name
                    logicWorld,              //its mother  volume 
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking 

   G4String regName6[] = {"BaseRegion"};                    
   G4Region* aRegion6 = new G4Region("BaseRegion");
   logicBase->SetRegion(aRegion6);
   aRegion6->AddRootLogicalVolume(logicBase); 
                                           
  //
  //TERRENO DI COLTURA
  //  
  G4double colture_medium_height  = 3.2*mm;  
  
  G4Tubs* solidDMEM =
    new G4Tubs("DMEM_medium",                    //its name
                0, // raggio interno 
                petri_radius_int, //raggio esterno
                0.5*colture_medium_height,  //metà altezza
                0., //starting phi
                2*M_PI); //segment angle

  G4LogicalVolume* logicDMEM =
    new G4LogicalVolume(solidDMEM,            //its solid
                        DMEM,             //its material
                        "DMEM_medium");         //its name
  
  G4VPhysicalVolume* physDMEM =  
    new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,-4.1*mm),         //
                    logicDMEM,                //its logical volume
                    "DMEM_medium",              //its name
                    logicPetri,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking 
    
   G4String regName3[] = {"DMEMRegion"};                    
   G4Region* aRegion3 = new G4Region("DMEMRegion");
   logicDMEM->SetRegion(aRegion3);
   aRegion3->AddRootLogicalVolume(logicDMEM);                


  //
    // COVER SLIP VETRO BOROSILICATO (pyrex) 
  //
  G4double CoverSlip_sizeXY = 24*mm;
  G4double CoverSlip_sizeZ  = 0.15*mm;

  G4Box* solidCoverSlip =
    new G4Box("CoverSlip",                       //its name
       0.5*CoverSlip_sizeXY, 0.5*CoverSlip_sizeXY, 0.5*CoverSlip_sizeZ);     //its size

  G4LogicalVolume* logicCoverSlip =
    new G4LogicalVolume(solidCoverSlip,          //its solid
                        pyrex,           //its material 
                        "CoverSlip");            //its name

  G4VPhysicalVolume* physCoverSlip =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-1.525*mm),       // its centre //considerare il suo volume madre e porsi al centro dello stesso.
                      logicCoverSlip,            //its logical volume
                      "CoverSlip",               //its name
                      logicDMEM,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

   G4String regName4[] = {"CoverSlipRegion"};                    
   G4Region* aRegion4 = new G4Region("CoverSlipRegion");
   logicCoverSlip->SetRegion(aRegion4);
   aRegion4->AddRootLogicalVolume(logicCoverSlip);

                    
   //
  //CELLULE
  //  
  G4double cells_height  = 0.01*mm;   
  
    G4Box* solidCells =
    new G4Box("Cells",                       //its name
       0.5*CoverSlip_sizeXY, 0.5*CoverSlip_sizeXY, 0.5*cells_height);     //its size

  G4LogicalVolume* logicCells =
    new G4LogicalVolume(solidCells,          //its solid
                        soft_tissue,           //its material 
                        "Cells");            //its name
  
  G4VPhysicalVolume* physCells =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-1.445*mm),       // its centre
                      logicCells,            //its logical volume
                      "Cells",               //its name
                      logicDMEM,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

   G4String regName2[] = {"CellsRegion"};                    
   G4Region* aRegion2 = new G4Region("CellsRegion");
   logicCells->SetRegion(aRegion2);
   aRegion2->AddRootLogicalVolume(logicCells);
//--------------------------------------------------------------------------


    //
  // PIATTO SU CUI APPOGGIA LA PETRI
  //
  G4double plate_sizeX = 14.3*cm;
  G4double plate_sizeY = 12.*cm;
  G4double plate_sizeZ  = 6.*mm;
 
  G4Box* solidPlate =
    new G4Box("Plate",                       //its name
       0.5*plate_sizeX, 0.5*plate_sizeY, 0.5*plate_sizeZ);     //its size

  G4LogicalVolume* logicPlate =
    new G4LogicalVolume(solidPlate,          //its solid
                        plate_mat,           //its material
                        "Plate");            //its name

 G4VPhysicalVolume* physPlate =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-9.7*mm),  //centre
                      logicPlate,            //its logical volume
                      "Plate",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
  
//set scoring volume
  fScoringVolume = logicCells;
  
   G4String regName5[] = {"PlateRegion"};                    
   G4Region* aRegion5 = new G4Region("PlateRegion");
   logicPlate->SetRegion(aRegion5);
   aRegion5->AddRootLogicalVolume(logicPlate);
  
} else { 
//
//
//----------------------------------------------------------------------------------- 
                
// 
// SECONDA CONFIGURAZIONE: irraggiamento della RADCAL
//

//RADCAL -CHAMBER
//Concentric cylinder. Polycarbonate walls and electrode. Conductive graphite interior coating. 6cm3 active volume. 0.05kg

  G4double Radcal_diameter  = 25*mm ;
  G4double RadcalINT_diameter  = 24.8*mm ;
  G4double Radcal_lenght  = 38*mm; 
  G4double RadcalINT_lenght  = 37.8*mm;

  //NB la camera è tutta fatta d'aria ad eccezione del sito sensibile.

//PARTE SUPERIORE RADCAL, RIVESTIMENTO POLYCARBONATE  
  
  G4Tubs* solidRadcalEXT =
    new G4Tubs("RadcalEXT",                    //its name
                0.,                         // raggio interno 5*RadcalINT_diameter
                0.5*Radcal_diameter,          //raggio esterno  
                0.5*Radcal_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicRadcalEXT=
    new G4LogicalVolume(solidRadcalEXT,            //its solid
                        air_mat,             //its material Polycarbonate
                        "RadcalEXT");         //its name
  
  G4RotationMatrix* RadcalEXTRot = new G4RotationMatrix();
  RadcalEXTRot->rotateY(90.*deg);

  G4VPhysicalVolume* physRadcalEXT =
  new G4PVPlacement(RadcalEXTRot,                       // rotation -> asse su x
                    G4ThreeVector(6*mm,0,-6.545*mm),         //centre
                    logicRadcalEXT,                //its logical volume
                    "RadcalEXT",              //its name
                    logicWorld,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   

   G4String regName3[] = {"RadcalEXTRegion"};                    
   G4Region* aRegion3 = new G4Region("RadcalEXTRegion");
   logicRadcalEXT->SetRegion(aRegion3);
   aRegion3->AddRootLogicalVolume(logicRadcalEXT);   


//PARTE SUPERIORE RADCAL, INTERNO IN GRAFITE CONDUTTRICE
  
  G4Tubs* solidRadcalINT=
    new G4Tubs("RadcalINT",                    //its name
                0,                         // raggio interno 
                0.5*RadcalINT_diameter,          //raggio esterno -> rivestimento di 0.2 mm spessore 
                0.5*RadcalINT_lenght,     //metà altezza 
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicRadcalINT =
    new G4LogicalVolume(solidRadcalINT,            //its solid
                        air_mat,             //its material Graphite
                        "RadcalINT");         //its name 

  G4VPhysicalVolume* physRadcalINT =
  new G4PVPlacement(0,                       // no rotation 
                    G4ThreeVector(0,0,0),         //centre
                    logicRadcalINT,                //its logical volume
                    "RadcalINT",              //its name
                    logicRadcalEXT,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   

   G4String regName4[] = {"RadcalINTRegion"};                    
   G4Region* aRegion4 = new G4Region("RadcalINTRegion");
   logicRadcalINT->SetRegion(aRegion4);
   aRegion4->AddRootLogicalVolume(logicRadcalINT); 
 
 //VOLUME SENSIBILE RADCAL
 
 G4double RadcalSensitiveVolume_radius = 1.12725165*cm; //volume sensibile 6cm^3
 
 G4Sphere* solidRadcalSensitiveVolume =
    new G4Sphere("RadcalSensitiveVolume",                    //its name
                0,                         // raggio interno 
                RadcalSensitiveVolume_radius,          //raggio esterno  
                0,                      //starting phi
                2*M_PI,                   //questo vuol dire che l'angolo phi è 180 
                0,                      //starting theta
                2*M_PI);                //theta

  G4LogicalVolume* logicRadcalSensitiveVolume =
    new G4LogicalVolume(solidRadcalSensitiveVolume,            //its solid
                        Polycarbonate,             //its material 
                        "RadcalSensitiveVolume");         //its name

  G4VPhysicalVolume* physRadcalSensitiveVolume =
    new G4PVPlacement(0,                       // no rotation 
                      G4ThreeVector(0,0,6*mm),         //centre  ----> sistemare
                      logicRadcalSensitiveVolume,                //its logical volume
                      "RadcalSensitiveVolume",              //its name
                       logicRadcalINT,              //its mother  volume -> vedi logictub in caso
                      false,                   //no boolean operation
                      0,                       //copy number
                      checkOverlaps);          //overlaps checking 

   G4String regName2[] = {"SensitiveVolumeRegion"};                    
   G4Region* aRegion2 = new G4Region("SensitiveVolumeRegion");
   logicRadcalSensitiveVolume->SetRegion(aRegion2);
   aRegion2->AddRootLogicalVolume(logicRadcalSensitiveVolume);                    
                    
//PARTE INFERIORE ESTERNA RADCAL
  G4double RadcalBottom_lenght  = 80*mm; 
  G4double RadcalBottom_diameter  = 19*mm;
  G4double RadcalINTBottom_diameter  = 18.8*mm; //spessore rivestimento 0.2mm
  
  G4Tubs* solidRadcalEXTBottom =
    new G4Tubs("RadcalEXTBottom",                    //its name
                0.,                         // raggio interno 5*RadcalINTBottom_diameter
                0.5*RadcalBottom_diameter,          //raggio esterno  
                0.5*RadcalBottom_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicRadcalEXTBottom =
    new G4LogicalVolume(solidRadcalEXTBottom,            //its solid
                        air_mat,             //its material Polycarbonate
                        "RadcalEXTBottom");         //its name   --> piombo
  
  G4RotationMatrix* RadcalEXTBottomRot = new G4RotationMatrix();
  RadcalEXTBottomRot->rotateY(90.*deg);

  G4VPhysicalVolume* physRadcalEXTBottom =
  new G4PVPlacement(RadcalEXTBottomRot,                       // rotation -> asse su x
                    G4ThreeVector(65*mm,0,-6.545*mm),         //centre
                    logicRadcalEXTBottom,                //its logical volume
                    "RadcalEXTBottom",              //its name
                    logicWorld,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking   
                    
                    
//PARTE INFERIORE INTERNA RADCAL
  
  G4Tubs* solidRadcalINTBottom =
    new G4Tubs("RadcalINTBottom",                    //its name
                0,                         // raggio interno 
                0.5*RadcalINTBottom_diameter,          //raggio esterno  
                0.5*RadcalBottom_lenght,     //metà altezza
                0.,                        //starting phi
                2*M_PI);                //segment angle: rappresenta la granularità della superficie

  G4LogicalVolume* logicRadcalINTBottom =
    new G4LogicalVolume(solidRadcalINTBottom,            //its solid
                        air_mat,             //its material Graphite
                        "RadcalINTBottom");         //its name   

  G4VPhysicalVolume* physRadcalINTBottom =
  new G4PVPlacement(0,                       // rotation -> asse su x
                    G4ThreeVector(0,0,0),         //centre
                    logicRadcalINTBottom,                //its logical volume
                    "RadcalINTBottom",              //its name
                    logicRadcalEXTBottom,              //its mother  volume -> vedi logictub in caso
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking                                                                      
                      

    //
  // PIATTO SU CUI APPOGGIA LA RADCAL
  //
  G4double plate_sizeX = 14.3*cm;
  G4double plate_sizeY = 12*cm;
  G4double plate_sizeZ  = 6*mm;
 
  G4Box* solidPlate =
    new G4Box("Plate",                       //its name
       0.5*plate_sizeX, 0.5*plate_sizeY, 0.5*plate_sizeZ);     //its size

  G4LogicalVolume* logicPlate = 
    new G4LogicalVolume(solidPlate,          //its solid
                        plate_mat,           //its material
                        "Plate");            //its name

 G4VPhysicalVolume* physPlate =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,-22.045*mm),  //centre -6.545 -12.5 raggio radcal -3 altezza piatto 
                      logicPlate,            //its logical volume
                      "Plate",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                      
    G4String regName1[] = {"PlateRegion"};                    
   G4Region* aRegion1 = new G4Region("PlateRegion");
   logicPlate->SetRegion(aRegion1);
   aRegion1->AddRootLogicalVolume(logicPlate);                     

//set scoring volume
fScoringVolume = logicRadcalSensitiveVolume; 
}

//VOLUME DI SCORING

 // Set cells as scoring volume
  //fSpectrumScore = logicFilter; 
  fFilteredSpectrum = logicWindowInTheBox; 
  //
  //always return the physical World
  //
  return physWorld;
}


