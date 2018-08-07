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
//   wjb add PmtSD for optical photon simulation

#include "PmtSD.hh"
#include "PmtHits.hh"
#include "G4SDManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "G4OpticalPhoton.hh" //OP

PmtSD::PmtSD(G4String name):G4VSensitiveDetector(name){
  collectionName.insert("pmtCollection");
  fMessenger = new PmtSDMessenger(this);
  
}

PmtSD::~PmtSD()
{}

void PmtSD::Initialize(G4HCofThisEvent*){
  PmtCollection = new PmtHitsCollection(SensitiveDetectorName,collectionName[0]); 
  
  verboseLevel = 0;
  for (int i=0; i<nPmt; i++) totalPhotEvent[i]=0;
  //G4cout << " init PmtSD " << G4endl;
}


G4int PmtSD::GetDetID(G4Step*aStep){

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
  if(!volumeID.compare("ActiveLayerScint"))
    detID= 1E3 + 2*1E2 + (layerUp+2); 
  if(!volumeID.compare("ActiveLastLayerScint"))
    detID= 1E3 + 2*1E2 + 1;
  if(!volumeID.compare("ActiveBlockCrystal"))
    detID= 1E3 + 1*1E2 + (layer2Up+1)*1E1 + (layerUp+1)*1E0;
  return detID;
}

G4bool PmtSD::ProcessHits(G4Step*aStep,G4TouchableHistory*){
//OP - begin

//   G4cout << "---> PmtSD process hits" << G4endl;

  G4Track* aTrack = aStep->GetTrack();
  G4StepPoint* thePrePoint = aStep->GetPreStepPoint();
  G4VPhysicalVolume* thePrePV = thePrePoint->GetPhysicalVolume();
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4VPhysicalVolume* theTrackPV = aTrack->GetVolume();
  
  G4ParticleDefinition* particleType = aTrack->GetDefinition();
  //  G4cout << " particle Type " << &particleType << " opt photon " << G4OpticalPhoton::OpticalPhotonDefinition() << G4endl;
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){
    G4double energy = aTrack->GetKineticEnergy() / eV;
    //    G4cout << "photon energy " << energy << G4endl;
    //    G4cout << " pre material " << thePrePV->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
    //    G4cout << " post material " << thePostPV->GetLogicalVolume()->GetMaterial()->GetName() << G4endl;
    //    G4cout << " track volume " << theTrackPV->GetName() << G4endl;
    if(thePrePV->GetLogicalVolume()->GetMaterial()->GetName()=="AluminiumOpt") {
      //     G4cout << "debut aluminium process steps" << G4endl;
      G4TouchableHistory* touchable
       = (G4TouchableHistory*)(aStep->GetPreStepPoint()->GetTouchable());
      G4int depth = touchable->GetHistoryDepth();
      //     G4cout << "depth " << depth << G4endl;
      char  detname[20];
      detname[0] = '\0';
      if (depth > 0) {
	G4int id_pmt = 0;
	G4int id_tscin = 0;
	G4int id_cal_layer = 0;
	G4int ncopy = 0;
	G4int nblockraw=0;
	G4int nblock=0;
	for (int d=0; d<depth; d++) {
	   G4VPhysicalVolume* dpvolume =  touchable->GetVolume(d);
	   //G4cout << " depth " << d << " copy number " << touchable->GetCopyNumber(d) << " vname " << dpvolume->GetName() << G4endl;
	}
        for (int d=0; d<1; d++) {
         G4VPhysicalVolume* dpvolume =  touchable->GetVolume(d);
	 //	 G4cout << " depth " << d << " copy number " << touchable->GetCopyNumber(d) << " vname " << dpvolume->GetName() << G4endl;
	 if (d == 0) {
	   G4String vname = dpvolume->GetName();
	   //	   G4cout << "vname " << vname << G4endl;
 	   if (strstr(vname,"TSP") != 0) {
	     if (strcmp(vname,"TSPmt1") == 0) id_pmt = 1;
	     if (strcmp(vname,"TSPmt2") == 0) id_pmt = 2;
	     vname = touchable->GetVolume(1)->GetName();
	     if (strcmp(vname,"S1ScintillatorM") == 0)
	       id_tscin = touchable->GetCopyNumber(1) + 1;
	     if (strcmp(vname,"S1ScintillatorP") == 0)
	       id_tscin = touchable->GetCopyNumber(1) + 4;
	     sprintf(detname,"T%dPmt%d",id_tscin,id_pmt);
	     //G4cout << " T1 PLANE " << detname << G4endl;
	   }
	   //	   else G4cout << " vname " << vname << G4endl;
 	   if (strstr(vname,"P1P") != 0) {
	     if (strcmp(vname,"P1Pmt1") == 0) id_pmt = 1;
	     if (strcmp(vname,"P1Pmt2") == 0) id_pmt = 2;
	     ncopy = touchable->GetCopyNumber(2);
	     id_cal_layer = 2*ncopy - 1;
	     //     G4cout << "ici" << " id_cal_layer " << id_cal_layer << " id_pmt " << id_pmt << " ncopy " << ncopy << G4endl;
	     sprintf(detname,"P%dPmt%d",id_cal_layer,id_pmt);
	     //	     printf("%s/n",detname);
	     //	     G4cout << " CAL PLANE ODD " << detname << G4endl;
	   }
	   //	   else G4cout << " vname " << vname << G4endl;
 	   if (strstr(vname,"P2P") != 0) {
	     //	     G4cout << "ok" << G4endl;
	     if (strcmp(vname,"P2Pmt1") == 0) id_pmt = 1;
	     if (strcmp(vname,"P2Pmt2") == 0) id_pmt = 2;
	     ncopy = touchable->GetCopyNumber(2);
	     id_cal_layer = 2*ncopy;
	     if(touchable->GetVolume(2)->GetName()=="LastScintLayer") id_cal_layer = 16;
	     //G4cout << "ici" << " id_cal_layer " << id_cal_layer << " id_pmt " << id_pmt << " ncopy " << ncopy << G4endl;
	     sprintf(detname,"P%dPmt%d",id_cal_layer,id_pmt);
	     //	     G4cout << " CAL PLANE EVEN " << detname << G4endl;
	   }
	   if (strstr(vname,"L") != 0) {
	     //	     G4cout << "I'm in LPmt" << G4endl;
	     nblock = touchable->GetCopyNumber(2);
	     nblockraw = touchable->GetCopyNumber(3);
	     //G4cout << "nblock " << nblock << " nblockraw " << nblockraw << G4endl;
	     sprintf(detname,"L%dPmt",3*nblockraw+nblock+1);
	     //G4cout << "detname " << detname << G4endl;
	   }
	   //	   else G4cout << " vname " << vname << G4endl;
	 }
      //      if (d == 0) anActiveLayerName = dpvolume->GetName();
        }
      }
      G4String pmtname(detname);
      //      G4cout << " detname " << detname << " pmtname " << pmtname << G4endl;
      PmtAnalysis(pmtname, energy);
      //      PMTAnalysis(pmtname, energy, number, ph_energy, pmt_qe);
      aTrack->SetTrackStatus(fStopAndKill);
      //      G4cout << "---> fin PmtSD process hits" << G4endl;
    }
  }
  return true;
}

void PmtSD::PmtAnalysis(G4String detname, G4double energy){
//----------Quantum efficiency of PMT R9880-210
  //NUMBERS from R9880-210 datasheet
  const G4int number = 121;
  G4double ph_energy[number] = {1.692, 1.695, 1.698, 1.701, 1.704,
		           1.710, 1.716, 1.725, 1.728, 1.732,
		           1.741, 1.744, 1.751, 1.757, 1.764,
		           1.767, 1.777, 1.780, 1.784, 1.790,
			   1.801, 1.807, 1.811, 1.821, 1.825,
			   1.828, 1.835, 1.839, 1.846, 1.850,
			   1.860, 1.883, 1.890, 1.901, 1.909,
			   1.921, 1.928, 1.936, 1.948, 1.956,
			   1.976, 1.993, 2.006, 2.018, 2.036,
			   2.053, 2.076, 2.094, 2.113, 2.127,
			   2.146, 2.180, 2.211, 2.232, 2.258,
			   2.280, 2.302, 2.324, 2.342, 2.359,
			   2.365, 2.383, 2.389, 2.401, 2.431,
			   2.450, 2.476, 2.508, 2.542, 2.562,
			   2.583, 2.612, 2.648, 2.678, 2.708,
			   2.740, 2.796, 2.846, 2.890, 2.943,
			   2.999, 3.067, 3.138, 3.223, 3.290,
			   3.383, 3.470, 3.547, 3.628, 3.743,
			   3.818, 3.912, 3.994, 4.045, 4.115,
			   4.188, 4.263, 4.321, 4.361, 4.442,
			   4.527, 4.571, 4.637, 4.683, 4.730,
			   4.801, 4.826, 4.900, 4.951, 4.977,
			   5.030, 5.057, 5.111, 5.139, 5.195,
			   5.223, 5.252, 5.281, 5.311, 5.371,
			   5.402};

  G4double pmt_qe[number] ={0.00, 0.00, 0.00, 0.00, 0.00,
		        0.00, 0.00, 0.00, 0.00, 0.00,
	       	        0.00, 0.00, 0.00, 0.00, 0.00,
	       	        0.01, 0.01, 0.01, 0.01, 0.01,
       		        0.01, 0.01, 0.01, 0.01, 0.01,
		        0.01, 0.02, 0.02, 0.02, 0.02,
	       	        0.02, 0.02, 0.02, 0.03, 0.03,
		        0.03, 0.04, 0.04, 0.04, 0.04,
	       	        0.05, 0.05, 0.05, 0.06, 0.06,
      		        0.07, 0.07, 0.08, 0.08, 0.09,
		        0.09, 0.10, 0.10, 0.11, 0.12,
		        0.13, 0.13, 0.15, 0.16, 0.17,
		        0.18, 0.19, 0.21, 0.22, 0.24,
		        0.25, 0.26, 0.27, 0.28, 0.29,
		        0.29, 0.30, 0.32, 0.33, 0.34,
		        0.36, 0.37, 0.37, 0.38, 0.39,
       		        0.40, 0.40, 0.41, 0.41, 0.41,
		        0.42, 0.42, 0.42, 0.42, 0.41,
		        0.40, 0.39, 0.37, 0.35, 0.34,
		        0.32, 0.30, 0.29, 0.27, 0.26,
		        0.24, 0.22, 0.21, 0.20, 0.18,
		        0.17, 0.16, 0.14, 0.13, 0.12,
		        0.11, 0.10, 0.09, 0.08, 0.07,
		        0.06, 0.06, 0.05, 0.05, 0.04,
		        0.03};


  G4int detectorID = 0;
  G4int detID = 0;

  //  G4cout << "PMTAnalysis detname " << detname << " energy " << energy << G4endl;
  //  if (strcmp(detname,"P11Pmt1") == 0) {
  //    G4cout << " ok " << G4endl;
  //    detectorID = 12061; detID = 32;
  //  }
  //  G4cout << " detname " << detname << G4endl;
  //  for (int i=0; i<number; i++){
    G4double prob = rand() % 101;  //random number between 0 and 100
    prob = prob/100.;
    //    if(energy > ph_energy[i] && energy < ph_energy[i+1]){
    //   if(prob < pmt_qe[i]+(pmt_qe[i+1]-pmt_qe[i])/(ph_energy[i+1]-ph_energy[i])*(energy-ph_energy[i])) {
      //      G4cout << " photon detecte " << G4endl;
	if(detname=="T1Pmt1") { detectorID = 13111; detID = 0;}
	if(detname=="T1Pmt2") { detectorID = 13112; detID = 1;}
	if(detname=="T2Pmt1") { detectorID = 13121; detID = 2;}
	if(detname=="T2Pmt2") { detectorID = 13122; detID = 3;}
	if(detname=="T3Pmt1") { detectorID = 13131; detID = 4;}
	if(detname=="T3Pmt2") { detectorID = 13132; detID = 5;}
	if(detname=="T4Pmt1") { detectorID = 13211; detID = 6;}
	if(detname=="T4Pmt2") { detectorID = 13212; detID = 7;}
	if(detname=="T5Pmt1") { detectorID = 13221; detID = 8;}
	if(detname=="T5Pmt2") { detectorID = 13222; detID = 9;}
	if(detname=="T6Pmt1") { detectorID = 13231; detID = 10;}
	if(detname=="T6Pmt2") { detectorID = 13232; detID = 11;}

	if(detname=="P1Pmt1") { detectorID = 12161; detID = 12;}
	if(detname=="P1Pmt2") { detectorID = 12162; detID = 13;}
	if(detname=="P2Pmt1") { detectorID = 12151; detID = 14;}
	if(detname=="P2Pmt2") { detectorID = 12152; detID = 15;}
	if(detname=="P3Pmt1") { detectorID = 12141; detID = 16;}
	if(detname=="P3Pmt2") { detectorID = 12142; detID = 17;}
	if(detname=="P4Pmt1") { detectorID = 12131; detID = 18;}
	if(detname=="P4Pmt2") { detectorID = 12132; detID = 19;}
	if(detname=="P5Pmt1") { detectorID = 12121; detID = 20;}
	if(detname=="P5Pmt2") { detectorID = 12122; detID = 21;}
	if(detname=="P6Pmt1") { detectorID = 12111; detID = 22;}
	if(detname=="P6Pmt2") { detectorID = 12112; detID = 23;}
	if(detname=="P7Pmt1") { detectorID = 12101; detID = 24;}
	if(detname=="P7Pmt2") { detectorID = 12102; detID = 25;}
	if(detname=="P8Pmt1") { detectorID = 12091; detID = 26;}
	if(detname=="P8Pmt2") { detectorID = 12092; detID = 27;}
	if(detname=="P9Pmt1") { detectorID = 12081; detID = 28;}
	if(detname=="P9Pmt2") { detectorID = 12082; detID = 29;}
	if(detname=="P10Pmt1") { detectorID = 12071; detID = 30;}
	if(detname=="P10Pmt2") { detectorID = 12072; detID = 31;}
        if (strcmp(detname,"P11Pmt1") == 0) { detectorID = 12061; detID = 32;}
	//	if(detname=="P11Pmt1") { detectorID = 12061; detID = 32;}
	if(detname=="P11Pmt2") { detectorID = 12062; detID = 33;}
	if(detname=="P12Pmt1") { detectorID = 12051; detID = 34;}
	if(detname=="P12Pmt2") { detectorID = 12052; detID = 35;}
	if(detname=="P13Pmt1") { detectorID = 12041; detID = 36;}
	if(detname=="P13Pmt2") { detectorID = 12042; detID = 37;}
	if(detname=="P14Pmt1") { detectorID = 12031; detID = 38;}
	if(detname=="P14Pmt2") { detectorID = 12032; detID = 39;}
	if(detname=="P15Pmt1") { detectorID = 12021; detID = 40;}
	if(detname=="P15Pmt2") { detectorID = 12022; detID = 41;}
	if(detname=="P16Pmt1") { detectorID = 12011; detID = 42;}
	if(detname=="P16Pmt2") { detectorID = 12012; detID = 43;}

	if(detname=="L1Pmt") { detectorID = 11111; detID = 44;}
	if(detname=="L2Pmt") { detectorID = 11211; detID = 45;}
	if(detname=="L3Pmt") { detectorID = 11311; detID = 46;}
	if(detname=="L4Pmt") { detectorID = 11121; detID = 47;}
	if(detname=="L5Pmt") { detectorID = 11221; detID = 48;}
	if(detname=="L6Pmt") { detectorID = 11321; detID = 49;}
	if(detname=="L7Pmt") { detectorID = 11131; detID = 50;}
	if(detname=="L8Pmt") { detectorID = 11231; detID = 51;}
	if(detname=="L9Pmt") { detectorID = 11331; detID = 52;}

	//	G4cout << "detectID " << detectorID << " detID " << detID << G4endl;
	totalPhotEvent[detID] += 1;
	//	G4cout << " detID " << detID << " totalPhotEvent " << totalPhotEvent[detID] << G4endl;
	//        G4cout << " ok " << G4endl;
	//   }
	//   }
	//  }
  
}

void PmtSD::EndOfEvent(G4HCofThisEvent*HCE){
  //  G4cout << " PmtSD eofe " << G4endl;
  //  for (int i=0; i<nPmt; i++) {
  //	G4cout << " pmt  " << i << " totalPhotEvent " << totalPhotEvent[i] << G4endl;
  //  }
  size_t tnPmt = ((size_t) nPmt);
  G4int aTotalPhot[tnPmt];
  for (int i=0; i<nPmt; i++) aTotalPhot[i] = totalPhotEvent[i];
  PmtHits* store_pmtHits = new PmtHits(aTotalPhot,tnPmt);
  PmtSD* sd = dynamic_cast<PmtSD*>(G4SDManager::GetSDMpointer()->FindSensitiveDetector("/hepd/pmt"));
  sd->GetPmtHitsCollection()->insert(store_pmtHits);
  G4int* TotalPhot_ptr = store_pmtHits->GetTotalPhotpt();
  //  G4cout << " TotalPhot_ptr " << TotalPhot_ptr << G4endl;
  //  for (int i=0; i<nPmt; i++) {
  //    G4cout << " pmt " << i << " totalPhotEven hits " << store_pmtHits->GetNPhot(i) << G4endl;
  //  }  
  static G4int HCID = -1;
  if(HCID<0)
    HCID = GetCollectionID(0);
  //  G4cout << "HCID " << HCID << G4endl;
  HCE->AddHitsCollection( HCID, PmtCollection );
  //  G4cout << " fin PmtSD eofe " << G4endl;
}

void PmtSD::clear(){
} 

void PmtSD::DrawAll(){
} 

void PmtSD::PrintAll(){
} 
