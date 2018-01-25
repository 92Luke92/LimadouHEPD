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
  :/*fSolidBlanket(0),*/fSolidBlanketKapton(0),fSolidBlanketCopper(0),fSolidWallExternal(0),fSolidWallHoneyComb(0),
   /*fLogicBlanket(0),*/fLogicBlanketKapton(0),fLogicBlanketCopper(0),fLogicWallExternal(0),fLogicWallHoneyComb(0),
   /*fPhysiBlanket(0),*/fPhysiBlanketKapton(0),fPhysiBlanketCopper(0),fPhysiWallExternalIn(0),fPhysiWallHoneyComb(0),fPhysiWallExternalOut(0)
{
  pMaterial     = new HEPDSWMaterial();
  fBlanket_X  = 254*mm;
  fBlanket_Y  = 238*mm;
  //fBlanket_Z  = 0.1*mm;
  fBlanketCopper_Z  = 0.035*mm;
  fBlanketKapton_Z  = 0.045*mm;

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
  blanketMaterial        = "mylar";
  blanketOuterMaterial   = "Kapton";
  blanketInnerMaterial   = "Copper";
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

  //fPhysiBlanket_Z = 370.57*mm;
  fPhysiBlanketCopper_Z = 370.5125*mm;
  fPhysiBlanketKapton_Z = fPhysiBlanketCopper_Z + fBlanketCopper_Z/2.+fBlanketKapton_Z/2.;

  fPhysiWallExternalIn_X = +14.5*mm;
  fPhysiWallExternalIn_Y = -92*mm;
  //PhysiWallExternalIn_Z = fPhysiBlanket_Z+fBlanket_Z/2.+fWallExternal_Z/2.;
  fPhysiWallExternalIn_Z = fPhysiBlanketKapton_Z+fBlanketKapton_Z/2. +0.01*mm +fWallExternal_Z/2.;  // 0.01mm is added to fill the gap difference

  fPhysiWallHoneyComb_X = +14.5*mm;
  fPhysiWallHoneyComb_Y = -92*mm;
  fPhysiWallHoneyComb_Z = fPhysiWallExternalIn_Z+fWallExternal_Z/2.+fWallHoneyComb_Z/2.;

  fPhysiWallExternalOut_X = +14.5*mm;
  fPhysiWallExternalOut_Y = -92*mm;
  fPhysiWallExternalOut_Z = fPhysiWallHoneyComb_Z+fWallHoneyComb_Z/2.+fWallExternal_Z/2.;
}


void HEPDBoxConstructionConfig4::SetBlanketMaterial(G4String aMat){
  blanketMaterial=aMat;
}
void HEPDBoxConstructionConfig4::SetBlanketOuterMaterial(G4String aMat){
  blanketOuterMaterial=aMat;
}
void HEPDBoxConstructionConfig4::SetBlanketInnerMaterial(G4String aMat){
  blanketInnerMaterial=aMat;
}

void HEPDBoxConstructionConfig4::SetWallMaterial(G4String aMat1,G4String aMat2){
  wallMaterial=aMat1;
  wallHoneyCombMaterial=aMat2;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HEPDBoxConstructionConfig4::Builder(G4VPhysicalVolume* motherVolume )
{

  G4cout << "HEPD Box ConstructionConfig4" << G4endl;

  pMaterial->DefineMaterials();
  //G4Material* blanketMat = pMaterial->GetMaterial(blanketMaterial);
  G4Material* blanketOuterMat = pMaterial->GetMaterial(blanketOuterMaterial);
  G4Material* blanketInnerMat = pMaterial->GetMaterial(blanketInnerMaterial);
  G4Material* wallMat = pMaterial->GetMaterial(wallMaterial);
  G4Material* wallHCMat = pMaterial->GetMaterial(wallHoneyCombMaterial);


  G4RotationMatrix* myRot = new G4RotationMatrix;

//  fSolidBlanket = new G4Box("fSolidThermalBlanket",fBlanket_X/2.,fBlanket_Y/2.,fBlanket_Z/2);

  fSolidBlanketKapton = new G4Box("SThermalBlanketKapton",fBlanket_X/2.,fBlanket_Y/2.,fBlanketKapton_Z/2);
  fSolidBlanketCopper = new G4Box("SThermalBlanketCopper",fBlanket_X/2.,fBlanket_Y/2.,fBlanketCopper_Z/2);

  G4ThreeVector transWallHole(transWallHole_X,transWallHole_Y,0);
  fSolidWallExternal = new G4SubtractionSolid("fSolidWallExternal",
					      new G4Box("WallExternal",fWallExternal_X/2.,fWallExternal_Y/2.,fWallExternal_Z/2.),
					      new G4Box("WallHole",fWallHole_X/2.,fWallHole_Y/2.,fWallHole_Z/2.),
					      myRot,transWallHole);

  fSolidWallHoneyComb = new G4SubtractionSolid("fSolidWallHoneyComb",
					      new G4Box("WallHoneyComb",fWallHoneyComb_X/2.,fWallHoneyComb_Y/2.,fWallHoneyComb_Z/2.),
					      new G4Box("WallHole",fWallHole_X/2.,fWallHole_Y/2.,fWallHole_Z/2.),
					      myRot,transWallHole);



  //fLogicBlanket = new G4LogicalVolume(fSolidBlanket,blanketMat,"ThermalBlanket");
  fLogicBlanketKapton = new G4LogicalVolume(fSolidBlanketKapton,blanketOuterMat,"LogicThermalBlanketKapton");

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String mcTruthSDname = "/hepd/mctruth";
  G4VSensitiveDetector* mcSD = SDman->FindSensitiveDetector(mcTruthSDname);
  fLogicBlanketKapton->SetSensitiveDetector(mcSD);

  fLogicBlanketCopper = new G4LogicalVolume(fSolidBlanketCopper,blanketInnerMat,"LogicThermalBlanketCopper");
  fLogicWallExternal = new G4LogicalVolume(fSolidWallExternal,wallMat,"fLogicWallExternal");
  fLogicWallHoneyComb = new G4LogicalVolume(fSolidWallHoneyComb,wallHCMat,"fLogicWallHoneyComb");


  //G4cout << " zpos blanket cm " << fPhysiBlanket_Z/cm << " fBlanket_Z/2 cm " <<  (fBlanket_Z/2)/cm << G4endl;
  G4cout << " zpos blanket kapton cm " << fPhysiBlanketKapton_Z/cm << " fBlanketKapton_Z/2 cm " <<  (fBlanketKapton_Z/2)/cm << G4endl;
  G4cout << " zpos blanket copper cm " << fPhysiBlanketCopper_Z/cm << " fBlanketCopper_Z/2 cm " <<  (fBlanketCopper_Z/2)/cm << G4endl;

  //fPhysiBlanket = new G4PVPlacement(0,
	//			    G4ThreeVector(0,0,fPhysiBlanket_Z),
	//			    "HEPDBoxThermalBlanket",
	//			    fLogicBlanket,
	//			    motherVolume,
	//			    false,0,true);

  fPhysiBlanketKapton = new G4PVPlacement(0,
            G4ThreeVector(0,0,fPhysiBlanketKapton_Z),
            "ThermalBlanketKapton",
            fLogicBlanketKapton,
            motherVolume,
            false,0,true);

  fPhysiBlanketCopper = new G4PVPlacement(0,
            G4ThreeVector(0,0,fPhysiBlanketCopper_Z),
            "ThermalBlanketCopper",
            fLogicBlanketCopper,
            motherVolume,
            false,0,true);

  G4cout << " Wall Extern In Z " << fPhysiWallExternalIn_Z << G4endl;

  fPhysiWallExternalIn = new G4PVPlacement(0,
				    G4ThreeVector(fPhysiWallExternalIn_X,fPhysiWallExternalIn_Y,fPhysiWallExternalIn_Z),
				    "HEPDBoxWallExternalIn ",
				    fLogicWallExternal,
				    motherVolume,
				    false,0,true);

  G4cout << " Wall HoneyComb Z " << fPhysiWallHoneyComb_Z << G4endl;

  fPhysiWallHoneyComb = new G4PVPlacement(0,
					  G4ThreeVector(fPhysiWallHoneyComb_X,fPhysiWallHoneyComb_Y,fPhysiWallHoneyComb_Z),
				    "HEPDBoxHoneyComb",
				    fLogicWallHoneyComb,
				    motherVolume,
				    false,0,true);

  fPhysiWallExternalOut = new G4PVPlacement(0,
				    G4ThreeVector(fPhysiWallExternalOut_X,fPhysiWallExternalOut_Y,fPhysiWallExternalOut_Z),
				    "HEPDBoxWallExternalOut ",
				    fLogicWallExternal,
				    motherVolume,
				    false,0,true);

  G4cout << " Wall Extern Out Z " << fPhysiWallExternalOut_Z << " + " << fWallExternal_Z/2. << G4endl;

  //Visualization Attribute

  G4VisAttributes* attCyan = new G4VisAttributes(G4Colour::Cyan());
  attCyan->SetVisibility(false);
  attCyan->SetForceAuxEdgeVisible(true);
  fLogicWallHoneyComb->SetVisAttributes(attCyan);

  G4VisAttributes* attRed = new G4VisAttributes(G4Colour::Red());
  attRed->SetVisibility(false);
  attRed->SetForceAuxEdgeVisible(true);
  fLogicWallExternal->SetVisAttributes(attRed);

  G4VisAttributes* attGray = new G4VisAttributes(G4Colour::Gray());
  attGray->SetVisibility(false);
  attGray->SetForceAuxEdgeVisible(true);
  //fLogicBlanket->SetVisAttributes(attGray);
  fLogicBlanketKapton->SetVisAttributes(attGray);
  fLogicBlanketCopper->SetVisAttributes(attGray);


}


