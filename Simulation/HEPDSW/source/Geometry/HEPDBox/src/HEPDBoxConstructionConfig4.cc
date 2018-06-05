//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//          Filippo Ambroglini : filippo.ambroglini@pg.infn.it
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HEPDBoxConstructionConfig4.hh"


#include "G4NistManager.hh"
#include "HEPDSWMaterial.hh"
#include "G4Box.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HEPDBoxConstructionConfig4::HEPDBoxConstructionConfig4()
  :fSolidWallExternal(0),fSolidWallHoneyComb(0),
   fLogicBlanket1(0),fLogicBlanket2(0),fLogicWallExternal(0),fLogicWallHoneyComb(0),
   fPhysiBlanket1(0),fPhysiBlanket2(0),fPhysiWallExternalIn(0),fPhysiWallHoneyComb(0),fPhysiWallExternalOut(0)
{
  pMaterial     = new HEPDSWMaterial();
  fBlanket_X  = 254*mm;
  fBlanket_Y  = 238*mm;
  fBlanket1_Z  = 0.045*mm;
  fBlanket2_Z  = 0.035*mm;

  fWallExternal_X = 345*mm;
  fWallExternal_Y = 490*mm;
  fWallExternal_Z = 0.5*mm;

  fWallHoneyComb_X = 345*mm;
  fWallHoneyComb_Y = 490*mm;
  fWallHoneyComb_Z = 5*mm;

  fWallHole_X = 237*mm;
  fWallHole_Y = 218*mm;
  fWallHole_Z = 6*mm;


  ComputeObjectsPositioning();
  
  // materials
  blanket1Material       = "Kapton";
  blanket2Material       = "Copper";
  wallMaterial           = "Aluminium";
  wallHoneyCombMaterial  = "HCAluminium";
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
HEPDBoxConstructionConfig4::~HEPDBoxConstructionConfig4()
{
  if (pMaterial) delete pMaterial;
}

void HEPDBoxConstructionConfig4::ComputeObjectsPositioning(){

  transWallHole_X = fWallHoneyComb_X/2.-fWallHole_X/2.-68.5*mm;
  transWallHole_Y = fWallHoneyComb_Y/2.-fWallHole_Y/2.-44*mm;

  fPhysiBlanket_Z = 370.57*mm;

  fPhysiWallExternalIn_X = +14.5*mm;
  fPhysiWallExternalIn_Y = -92*mm;
  fPhysiWallExternalIn_Z = fPhysiBlanket_Z+fBlanket1_Z/2.+fBlanket2_Z/2.+fWallExternal_Z/2.;

  fPhysiWallHoneyComb_X = +14.5*mm;
  fPhysiWallHoneyComb_Y = -92*mm;
  fPhysiWallHoneyComb_Z = fPhysiWallExternalIn_Z+fWallExternal_Z/2.+fWallHoneyComb_Z/2.;

  fPhysiWallExternalOut_X = +14.5*mm;
  fPhysiWallExternalOut_Y = -92*mm;
  fPhysiWallExternalOut_Z = fPhysiWallHoneyComb_Z+fWallHoneyComb_Z/2.+fWallExternal_Z/2.;
}


void HEPDBoxConstructionConfig4::SetBlanketMaterial(G4String aMat){
  blanket1Material=aMat; 
}    
void HEPDBoxConstructionConfig4::SetWallMaterial(G4String aMat1,G4String aMat2){
  wallMaterial=aMat1; 
  wallHoneyCombMaterial=aMat2;
}    
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
void HEPDBoxConstructionConfig4::Builder(G4VPhysicalVolume* motherVolume)
{

  G4cout << "HEPD Box ConstructionConfig4" << G4endl;

  pMaterial->DefineMaterials();
  G4Material* blanket1Mat = pMaterial->GetMaterial(blanket1Material);
  G4Material* blanket2Mat = pMaterial->GetMaterial(blanket2Material);
  G4Material* wallMat = pMaterial->GetMaterial(wallMaterial);
  //G4Material* wallHCMat = pMaterial->GetMaterial(wallHoneyCombMaterial);
  

  G4RotationMatrix* myRot = new G4RotationMatrix;

  fSolidBlanket1 = new G4Box("fSolidThermalBlanket1",fWallHole_X/2.,fWallHole_Y/2.,fBlanket1_Z/2);
  fSolidBlanket2 = new G4Box("fSolidThermalBlanket2",fWallHole_X/2.,fWallHole_Y/2.,fBlanket2_Z/2);

  G4ThreeVector transWallHole(transWallHole_X,transWallHole_Y,0);
  fSolidWallExternal = new G4SubtractionSolid("fSolidWallExternal",
					      new G4Box("WallExternal",fWallExternal_X/2.,fWallExternal_Y/2.,fWallExternal_Z/2.),
					      new G4Box("WallHole",fWallHole_X/2.,fWallHole_Y/2.,fWallHole_Z/2.),
					      myRot,transWallHole);

  fSolidWallHoneyComb = new G4SubtractionSolid("fSolidWallHoneyComb",
					      new G4Box("WallHoneyComb",fWallHoneyComb_X/2.,fWallHoneyComb_Y/2.,fWallHoneyComb_Z/2.),
					      new G4Box("WallHole",fWallHole_X/2.,fWallHole_Y/2.,fWallHole_Z/2.),
					      myRot,transWallHole);


  

  fLogicBlanket1 = new G4LogicalVolume(fSolidBlanket1,blanket1Mat,"fLogicThermalBlanket1");
  fLogicBlanket2 = new G4LogicalVolume(fSolidBlanket2,blanket2Mat,"fLogicThermalBlanket2");
  fLogicWallExternal = new G4LogicalVolume(fSolidWallExternal,wallMat,"fLogicWallExternal");
  fLogicWallHoneyComb = new G4LogicalVolume(fSolidWallHoneyComb,wallMat,"fLogicWallHoneyComb");

  G4double suppLYSO_offset = 5.*mm;

  fPhysiBlanket1 = new G4PVPlacement(0,
				    G4ThreeVector(0,0,fPhysiBlanket_Z + fWallHoneyComb_Z/2. + fWallExternal_Z/2. + fBlanket2_Z + 0.005*mm + suppLYSO_offset),
				    "HEPDBoxThermalBlanket1",
				    fLogicBlanket1,
				    motherVolume,
				    false,0,true);

  fPhysiBlanket2 = new G4PVPlacement(0,
				    G4ThreeVector(0,0,fPhysiBlanket_Z + fWallHoneyComb_Z/2. + fWallExternal_Z/2.+suppLYSO_offset),
				    "HEPDBoxThermalBlanket2",
				    fLogicBlanket2,
				    motherVolume,
				    false,0,true);

  fPhysiWallExternalIn = new G4PVPlacement(0,
				    G4ThreeVector(fPhysiWallExternalIn_X,fPhysiWallExternalIn_Y,fPhysiWallExternalIn_Z+suppLYSO_offset),
				    "HEPDBoxWallExternalIn ",
				    fLogicWallExternal,
				    motherVolume,
				    false,0,true);

  fPhysiWallHoneyComb = new G4PVPlacement(0,
					  G4ThreeVector(fPhysiWallHoneyComb_X,fPhysiWallHoneyComb_Y,fPhysiWallHoneyComb_Z+suppLYSO_offset),
				    "HEPDBoxHoneyComb",
				    fLogicWallHoneyComb,
				    motherVolume,
				    false,0,true);

  fPhysiWallExternalOut = new G4PVPlacement(0,
				    G4ThreeVector(fPhysiWallExternalOut_X,fPhysiWallExternalOut_Y,fPhysiWallExternalOut_Z+suppLYSO_offset),
				    "HEPDBoxWallExternalOut ",
				    fLogicWallExternal,
				    motherVolume,
				    false,0,true);

  //Visualization Attribute

  G4VisAttributes* attCyan = new G4VisAttributes(G4Colour::Cyan());
  attCyan->SetVisibility(true);
  attCyan->SetForceAuxEdgeVisible(true);
  //fLogicWallHoneyComb->SetVisAttributes(attCyan);

  G4VisAttributes* attRed = new G4VisAttributes(G4Colour::Red());
  attRed->SetVisibility(true);
  attRed->SetForceAuxEdgeVisible(true);
  fLogicWallExternal->SetVisAttributes(attRed);
  fLogicWallHoneyComb->SetVisAttributes(attRed);

  G4VisAttributes* attGray = new G4VisAttributes(G4Colour::Gray());
  attGray->SetVisibility(true);
  attGray->SetForceAuxEdgeVisible(true);
  fLogicBlanket1->SetVisAttributes(attGray);
  fLogicBlanket2->SetVisAttributes(attGray);


}


