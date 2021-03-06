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

#include "CalorimeterSD.hh"
#include "CaloHit.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh"

CalorimeterSD::CalorimeterSD(G4String name):G4VSensitiveDetector(name){
  collectionName.insert("caloCollection");
  useBirks=false;

  birk1scint=0.0052*(g/(MeV*cm2));
  birk2scint=0.142;
  birk3scint=1.75;

  birk1crystal=0.03333*(g/(MeV*cm2));
  birk2crystal=0.;
  birk3crystal=1.;

  fMessenger = new CalorimeterSDMessenger(this);
  
}

CalorimeterSD::~CalorimeterSD()
{;}

void CalorimeterSD::Initialize(G4HCofThisEvent*){
  CaloCollection = new CaloHitsCollection(SensitiveDetectorName,collectionName[0]); 
  
  LayerID.clear();
  LayerTrkID.clear();
  verboseLevel = 0;
}


G4int CalorimeterSD::GetDetID(G4Step*aStep){

  G4int layer2Up = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber(2);
  G4int layerUp = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber(1);
  G4int layerVol = aStep->GetPreStepPoint()->GetTouchable()->GetCopyNumber();
  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4String volumeID = physVol->GetName();
  G4int detID = -1000;
  if(!volumeID.compare("S1ScintillatorM"))
    detID= 1E3 + 3*1E2 + 1*1E1 + (layerVol+1)*1E0;  
  if(!volumeID.compare("S1ScintillatorP"))
    detID= 1E3 + 3*1E2 + 2*1E1 + (layerVol+1)*1E0;
  //in case Config6 is activated
  if(!volumeID.compare("ActiveLayerScint"))
    detID= 1E3 + 2*1E2 + (layerUp+2); 
  //in case ConfigOptical is activated
  if(!volumeID.compare("ActiveLayerScintOdd")){
    if(layerUp==1) detID=1216; //P1
    if(layerUp==2) detID=1214; //P3
    if(layerUp==3) detID=1212; //P5
    if(layerUp==4) detID=1210; //P7
    if(layerUp==5) detID=1208; //P9
    if(layerUp==6) detID=1206; //P11
    if(layerUp==7) detID=1204; //P13
    if(layerUp==8) detID=1202; //P15
    //detID= 1E3 + 2*1E2 + (layerUp+2);
  }
  if(!volumeID.compare("ActiveLayerScintEven")){
    if(layerUp==1) detID=1215; //P2
    if(layerUp==2) detID=1213; //P4
    if(layerUp==3) detID=1211; //P6
    if(layerUp==4) detID=1209; //P8
    if(layerUp==5) detID=1207; //P10
    if(layerUp==6) detID=1205; //P12
    if(layerUp==7) detID=1203; //P14
  }
  
  if(!volumeID.compare("ActiveLastLayerScint"))
    detID= 1E3 + 2*1E2 + 1; //P16
  if(!volumeID.compare("ActiveBlockCrystal"))
    detID= 1E3 + 1*1E2 + (layer2Up+1)*1E1 + (layerUp+1)*1E0;
  return detID;
}

G4bool CalorimeterSD::ProcessHits(G4Step*aStep,G4TouchableHistory*){
  G4double edep = aStep->GetTotalEnergyDeposit();
  G4int tkID = aStep->GetTrack()->GetTrackID();
  G4int partID = aStep->GetTrack()->GetDefinition()->GetPDGEncoding();
  G4ThreeVector theExitPoint = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector theEntryPoint = aStep->GetPreStepPoint()->GetPosition();
  G4double theKE      = aStep->GetPreStepPoint()->GetKineticEnergy()/MeV;

  G4int genphot;
  const std::vector<const G4Track*> *tVec = aStep->GetSecondaryInCurrentStep();
  if(tVec)
  {
     size_t n_tVec = (*tVec).size();
     for(size_t i=0; i<n_tVec; i++)
     {
	if((*tVec)[i]->GetParticleDefinition()->GetParticleName()=="opticalphoton") genphot += 1;
     }
  }
  
  //  G4cout << "Calo step edep(MeV) = " << edep/MeV <<" ; given by Track = "<<tkID<< G4endl;
  // if(verboseLevel>1) G4cout << "Calo step edep(MeV) = " << edep/MeV <<" ; given by Track = "<<tkID<< G4endl;
  if(edep==0.) return false;
  //if(useBirks)
    //edep*=BirksAttenuation(aStep);  //it's not Birks' law!
  //  G4VPhysicalVolume* physVol = aStep->GetPreStepPoint()->GetPhysicalVolume();
  
  //  std::stringstream ss;
//   if(layerUp==layerVol||layerUp>layerVol){
//     ss << layerUp;
//   }else{
//     ss << layerVol;
//   }
  //G4String volumeID = physVol->GetName()+ss.str();
  //G4String volumeID = physVol->GetName();
  
  
  G4int detID;
  detID=GetDetID(aStep);
  
  G4Track* aTrack = aStep->GetTrack();
  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  G4int detectorID;
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition())
    {
      if(detID == 1311) detectorID = 0; //T1        
      if(detID == 1312) detectorID = 1; //T2
      if(detID == 1313) detectorID = 2; //T3
      if(detID == 1321) detectorID = 3; //T4
      if(detID == 1322) detectorID = 4; //T5
      if(detID == 1323) detectorID = 5; //T6

      if(detID == 1216) detectorID = 6; //P1
      if(detID == 1215) detectorID = 7; //P2
      if(detID == 1214) detectorID = 8; //P3
      if(detID == 1213) detectorID = 9; //P4
      if(detID == 1212) detectorID = 10; //P5
      if(detID == 1211) detectorID = 11; //P6
      if(detID == 1210) detectorID = 12; //P7
      if(detID == 1209) detectorID = 13; //P8
      if(detID == 1208) detectorID = 14; //P9
      if(detID == 1207) detectorID = 15; //P10
      if(detID == 1206) detectorID = 16; //P11
      if(detID == 1205) detectorID = 17; //P12
      if(detID == 1204) detectorID = 18; //P13
      if(detID == 1203) detectorID = 19; //P14
      if(detID == 1202) detectorID = 20; //P15
      if(detID == 1201) detectorID = 21; //P16
      if(detID == 1411) detectorID = 22; //VE
      if(detID == 1412) detectorID = 23; //VW
      if(detID == 1421) detectorID = 24; //VN
      if(detID == 1422) detectorID = 25; //VS
      if(detID == 1430) detectorID = 26; //VB

      if(detID == 1111) detectorID = 27; //L1
      if(detID == 1112) detectorID = 28; //L4
      if(detID == 1113) detectorID = 29; //L7
      if(detID == 1121) detectorID = 30; //L2
      if(detID == 1122) detectorID = 31; //L5
      if(detID == 1123) detectorID = 32; //L8
      if(detID == 1131) detectorID = 33; //L3
      if(detID == 1132) detectorID = 34; //L6
      if(detID == 1133) detectorID = 35; //L9
    }


  
  
   //  if(verboseLevel>1) G4cout << "Calo step on Volume = " << volumeID << G4endl;
   if(verboseLevel>1) G4cout << "Calo step on Volume = "<< detID << G4endl;
  
   if(LayerID.find(detID)==LayerID.end() || LayerTrkID.find(detID)->second!=tkID){
    //    CaloHit* calHit = new CaloHit(volumeID);
     CaloHit* calHit = new CaloHit(partID,detID,theEntryPoint,theExitPoint,theKE);
    calHit->SetEdep(edep/MeV,tkID);
    calHit->SetGenPhot(genphot,tkID);
    calHit->SetStepPos(theExitPoint,tkID);
    G4int icell = CaloCollection->insert(calHit);
    LayerID[detID] = icell - 1;
    LayerTrkID[detID] = tkID;
    if(verboseLevel>1){ 
       G4cout << " New  Hit on Calo Layer " 
          << detID <<" with deposited energy = "<<edep/MeV<< G4endl; 
     }
     }else{ 
     (*CaloCollection)[LayerID[detID]]->AddEdep(edep/MeV,tkID);
     (*CaloCollection)[LayerID[detID]]->AddGenPhot(genphot,tkID);
     (*CaloCollection)[LayerID[detID]]->SetStepPos(theExitPoint,tkID);

    if(verboseLevel>1){ 
      G4cout << " Energy added to Calo Layer " 
	     << detID <<" adding this energy deposit = "<<edep/MeV<< G4endl; 
    }
 }
  return true;
}

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*HCE){
  static G4int HCID = -1;
  if(HCID<0)
    HCID = GetCollectionID(0);   
  HCE->AddHitsCollection( HCID, CaloCollection );
}

void CalorimeterSD::clear(){
} 

void CalorimeterSD::DrawAll(){
} 

void CalorimeterSD::PrintAll(){
} 
/*
G4double CalorimeterSD::BirksAttenuation(const G4Step* aStep){
  double weight = 1.;
  double charge = aStep->GetPreStepPoint()->GetCharge();
  if (charge != 0. && aStep->GetStepLength() > 0){
    G4Material* mat = aStep->GetPreStepPoint()->GetMaterial();
    double density = mat->GetDensity();
    double dedx    = aStep->GetTotalEnergyDeposit()/aStep->GetStepLength();
    if(mat->GetName()=="Scintillator"||mat->GetName()=="Polystyrene"){
      double rkb     = birk1scint/density;
      double c       = birk2scint*rkb*rkb;
      if (std::abs(charge) >= 2.) rkb /= birk3scint; // based on alpha particle data
      weight = 1./(1.+rkb*dedx+c*dedx*dedx);
    }else{
      double rkb     = birk1crystal/density;
      double c       = birk2crystal*rkb*rkb;
      if (std::abs(charge) >= 2.) rkb /= birk3crystal; // based on alpha particle data
      weight = 1./(1.+rkb*dedx+c*dedx*dedx);
    }
  }
  return weight;
}
*/
