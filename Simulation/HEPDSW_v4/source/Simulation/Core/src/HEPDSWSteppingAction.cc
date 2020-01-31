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
//
// $Id$
//
//
//

#include "HEPDSWSteppingAction.hh"
#include "HEPDSWPrimaryGeneratorAction.hh"
#include "Interaction.hh"
#include "MCTruthSD.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4ParticleDefinition.hh"
#include "G4VPhysicalVolume.hh"
#include "G4TrackStatus.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

HEPDSWSteppingAction::HEPDSWSteppingAction(HEPDSWPrimaryGeneratorAction*)
{

  Xpos = -999.999;
  Ypos = -999.999;
  Zpos = -999.999;
  TrackID = -1;
  stepNumber=0;
  saveInteraction = true;

}

HEPDSWSteppingAction::~HEPDSWSteppingAction()
{
}

void HEPDSWSteppingAction::UserSteppingAction(const G4Step* step)
{
   
   const std::vector<const G4Track*>* tVec = step->GetSecondaryInCurrentStep();
   if(tVec)
   {
      size_t n_tVec = (*tVec).size();
      if(n_tVec>0 && step->GetPostStepPoint()->GetProcessDefinedStep())
      {
	 //G4cout << "Event " << G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()  << G4endl;
	 G4String theIntName = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	 G4int theIntID;
	 for(size_t i=0; i<n_tVec; i++)
	 {
	    G4ThreeVector theIntPoint = step->GetPostStepPoint()->GetPosition();
	    G4String theDetName = step->GetPostStepPoint()->GetPhysicalVolume()->GetName();
	    G4int theDetID;
	    
	    G4int theParticleType = (*tVec)[i]->GetParticleDefinition()->GetPDGEncoding();
	    G4double theKinEnergy = (*tVec)[i]->GetKineticEnergy();
	    
	    //---------------------------------------------------------------------------------------------------------------------------
	    //GET DET ID
	    if(step->GetPostStepPoint()->GetTouchable()->GetHistoryDepth()>1)
	    {
	       G4int layerVol = step->GetPostStepPoint()->GetTouchable()->GetCopyNumber();
	       G4int layer1Up = step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(1);
	       G4int layer2Up = step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(2);
	       
	       if(theDetName == "SiliconSensorM" || theDetName == "SiliconSensorP")
	       {
		  if(step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(4) == 0) theDetID = 2200;
		  else theDetID = 2100;
		  if(step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(2) == 0) theDetID += 30;
		  else if(step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(2) == 1) theDetID += 20;
		  else if(step->GetPostStepPoint()->GetTouchable()->GetCopyNumber(2) == 2) theDetID += 10;
		  if(theDetName == "SiliconSensorM") theDetID += 1;
		  else theDetID += 2;
	       }
	       else if(theDetName == "S1ScintillatorM") theDetID = 1310 + (layerVol+1);
	       else if(theDetName == "S1ScintillatorP") theDetID = 1320 + (layerVol+1);
	       //in case Config6 is activated
	       else if(theDetName == "ActiveLayerScint") theDetID = 1200 + (layer1Up+2);
	       //in case ConfigOptical is activated
	       else if(theDetName == "ActiveLayerScintOdd")
	       {
		  if(layer1Up==1) theDetID = 1216; //P1
		  if(layer1Up==2) theDetID = 1214; //P3
		  if(layer1Up==3) theDetID = 1212; //P5
		  if(layer1Up==4) theDetID = 1210; //P7
		  if(layer1Up==5) theDetID = 1208; //P9
		  if(layer1Up==6) theDetID = 1206; //P11
		  if(layer1Up==7) theDetID = 1204; //P13
		  if(layer1Up==8) theDetID = 1202; //P15
	       }
	       else if(theDetName == "ActiveLayerScintEven")
	       {
		  if(layer1Up==1) theDetID = 1215; //P2
		  if(layer1Up==2) theDetID = 1213; //P4
		  if(layer1Up==3) theDetID = 1211; //P6
		  if(layer1Up==4) theDetID = 1209; //P8
		  if(layer1Up==5) theDetID = 1207; //P10
		  if(layer1Up==6) theDetID = 1205; //P12
		  if(layer1Up==7) theDetID = 1203; //P14
	       }
	       else if(theDetName == "ActiveLastLayerScint") theDetID = 1201; //P16
	       else if(theDetName == "ActiveBlockCrystal") theDetID = 1100 + 10*(layer2Up+1) + (layer1Up+1);
	       else if(theDetName == "SuppLYSO") theDetID = 1140;
	       else if(theDetName == "HeatSink" || theDetName.contains("Kapton") || theDetName.contains("Hybrid") || theDetName.contains("Ring") || theDetName.contains("SiliconPlateM") || theDetName.contains("SiliconPlateP")) theDetID = 2300; //tracker supports
	       else if(theDetName.contains("S1Support")) theDetID = 1400; //trigger supports
	       
	       else if(theDetName.contains("Poron") || theDetName.contains("POR")) theDetID = 3000; //Poron structures
	       else if(theDetName.contains("CF")) theDetID = 4000; //CF structures
	       else if(theDetName == "Teflon") theDetID = 5000; //Teflon
	    }
	    else if(theDetName == "VetoYDown") theDetID = 1411; //VE
	    else if(theDetName == "VetoYUp") theDetID = 1412; //VW
	    else if(theDetName == "VetoXLeft") theDetID = 1421; //VN
	    else if(theDetName == "VetoXRight") theDetID = 1422; //VS
	    else if(theDetName == "VETOBottScintLayer") theDetID = 1430; //VB
	    
	    else if(theDetName.contains("Poron") || theDetName.contains("POR")) theDetID = 3000; //Poron structures
	    else if(theDetName.contains("CF")) theDetID = 4000; //CF structures
	    else if(theDetName.contains("Satellite")) theDetID = 100;
	    else if(theDetName.contains("HEPDBox")) theDetID = 200;
	    else if(theDetName == "world") theDetID = 1;
	    //---------------------------------------------------------------------------------------------------------------
	    
	    if(theIntName == "protonInelastic") theIntID = 1;
	    else if(theIntName == "neutronInelastic") theIntID = 2;
	    else if(theIntName == "pi+Inelastic") theIntID = 3;
	    else if(theIntName == "pi-Inelastic") theIntID = 4;
	    else if(theIntName == "tInelastic") theIntID = 5;
	    else if(theIntName == "dInelastic") theIntID = 6;
	    else if(theIntName == "alphaInelastic") theIntID = 7;
	    
	    
	    else if(theIntName == "hadElastic") theIntID = 10;
	    else if(theIntName == "hIoni") theIntID = 11;
	    else if(theIntName == "nCapture") theIntID = 12;
	    
	    else if(theIntName == "eBrem") theIntID = 100;
	    else if(theIntName == "eIoni") theIntID = 101;
	    else if(theIntName == "annihil") theIntID = 102;
	    
	    else if(theIntName == "muIoni") theIntID = 103;
	    
	    else if(theIntName == "phot") theIntID = 201;
	    else if(theIntName == "compt") theIntID = 202;
	    else if(theIntName == "conv") theIntID = 203;
	    
	    else if(theIntName == "Scintillation") theIntID = 300;
	    else if(theIntName == "Decay") theIntID = 400;
	    
	    Interaction* interaction = new Interaction(theIntPoint, theParticleType, theKinEnergy, theDetName, theDetID, theIntName, theIntID);
	    MCTruthSD* sd = dynamic_cast<MCTruthSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("/hepd/mctruth"));
	    sd->GetInteractionCollection()->insert(interaction);
	    
	    //G4cout << "\t" << i << " " <<
	    //   step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << " generated a " <<
	    //   (*tVec)[i]->GetParticleDefinition()->GetParticleName() << " of " <<
	    //   (*tVec)[i]->GetKineticEnergy() << " MeV in volume " <<
	    //   step->GetPostStepPoint()->GetPhysicalVolume()->GetName() << " (" << theDetID << ")" << G4endl;
	 }
      }
   }
}
   
  /*
void HEPDSWSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  if(fabs((aStep->GetPreStepPoint()->GetPosition()).getX()-Xpos)<0.01*mm && 
     fabs((aStep->GetPreStepPoint()->GetPosition()).getY()-Ypos)<0.01*mm && 
     fabs((aStep->GetPreStepPoint()->GetPosition()).getZ()-Zpos)<0.01*mm && 
     aStep->GetTrack()->GetTrackID()==TrackID){
      stepNumber++;
      if(stepNumber>50){
	Xpos = -999.999;
	Ypos = -999.999;
	Zpos = -999.999;
	TrackID = -1;
	stepNumber=0;
	aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
	G4Cout<<"WARNING EVENT WITH TRACK ABORTED BY OUR XCHECK"<<G4Endl;
      }
    }else{
      Xpos=(aStep->GetPreStepPoint()->GetPosition()).getX();
      Ypos=(aStep->GetPreStepPoint()->GetPosition()).getY();
      Zpos=(aStep->GetPreStepPoint()->GetPosition()).getZ();
      TrackID=aStep->GetTrack()->GetTrackID();
      stepNumber=0;
      }
  */
