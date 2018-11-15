// Local
#include "ecaladc.hh"
#include "detectorID.h"
#include "Edep_Pos.h"
#include "LEvRec0Writer.hh"
#include "trackeradc.hh"
#include "LEvRec0.hh"
#include "hits2edeppos.hh"

// C++ std
#include <iostream>
#include <string>
#include <cmath>
#include <sstream>
#include <vector>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"
#include "RootPmtHits.hh"


// ROOT libs
#include "TTree.h"
#include "TVector3.h"
#include "TFile.h"
#include "TBranch.h"








std::string  getMCfilename (int argc, char** argv);
std::string  getLvl0filename (const std::string mcfilename);
void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc);
std::vector<float> CaloHitsToEdep (std::vector<RootCaloHit>);
void getPMTs (std::vector<RootCaloHit>, ushort * pmt_high, ushort * pmt_low, EcalADC ecaladc);
void getPMTs_OP (std::vector<RootPmtHits>, ushort * pmt_high, ushort * pmt_low, EcalADC ecaladc);
void getStrips (std::vector<RootTrackerHit>, short* strips, TrackerADC trkadc);
int getMCTrackHitsEnergy(std::vector<RootTrackerHit> trHits);

TTree* Tmct = new TTree("TMCTruth","TMCTruth");
int particleid;
unsigned short energy;
short gen[3];
short theta;
short phi;
short SIL1_point[3];
float SILEdep[2];
float TEdep[6];
float PEdep[16];
float LEdep[9];
float VEdep[5];
float TRIGEdep;
float TOWEREdep;
float TOTALEdep;
int pmt[53];


int main (int argc, char** argv) {
    const std::string mcfilename = getMCfilename (argc, argv);
    const std::string lvl0filename = getLvl0filename (mcfilename);
    TFile* filemc = TFile::Open (mcfilename.c_str(), "READ");
    TTree* Tmc =  static_cast<TTree*> (filemc->Get ("HEPD/EventTree"));
    LEvRec0Writer lvl0writer (lvl0filename);
    //mctruth tree
    Tmct->Branch("particleid", &particleid);
    Tmct->Branch("energy", &energy);
    Tmct->Branch("gen[3]", &gen[0]);
    Tmct->Branch("theta", &theta);
    Tmct->Branch("phi", &phi);
    Tmct->Branch("SIL1_point[3]", &SIL1_point[0]);
    Tmct->Branch("SILEdep[2]", &SILEdep[0]);
    Tmct->Branch("TEdep[6]", &TEdep[0]);
    Tmct->Branch("PEdep[16]", &PEdep[0]);
    Tmct->Branch("LEdep[9]", &LEdep[0]);
    Tmct->Branch("VEdep[5]", &VEdep[0]);
    Tmct->Branch("TRIGEdep", &TRIGEdep);
    Tmct->Branch("TOWEREdep", &TOWEREdep);
    Tmct->Branch("TOTALEdep", &TOTALEdep);
    Tmct->Branch("pmt[53]", &pmt[0]);
        
    
    LoopOnEvents (&lvl0writer, Tmc);
    Tmct->Write();
    lvl0writer.Write();
    lvl0writer.Close(); delete Tmc;
    filemc->Close();    delete filemc;
    return 0;
} // main conclusion




void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc)
{
    int ne = Tmc->GetEntries();
    RootEvent* MCevt = new RootEvent;
    TBranch* b_Event = new TBranch;
    Tmc->SetBranchAddress ("Event", &MCevt, &b_Event);
    EcalADC ecaladc(EcalADC::OptPhot);
    TrackerADC trkadc;

    std::cout << "Entries are: " << Tmc->GetEntries() << std::endl;
    //if (ne>100000) ne=100000;

    Int_t checkID=0;
    Int_t checkIDTrack=0;
    for (int ie = 0; ie < ne; ie++) {
        Tmc->GetEntry (ie);
	std::vector<RootTrack> trackHits = MCevt->GetTracks();
        std::vector<RootCaloHit> caloHits =  MCevt->GetCaloHit();
	std::vector<RootCaloHit> vetoHits =  MCevt->GetVetoHit();
        std::vector<RootTrackerHit>  trackerHits =  MCevt->GetTrackerHit();
	std::vector<RootPmtHits> pmtHits = MCevt->GetPmtHits();
	
	
        //if(ie==0) ecaladc.setMCEnergy(getMCTrackHitsEnergy(trackerHits));

        LEvRec0* ev = lvl0writer->pev();
        ev->Reset();

	if(ecaladc.OPmethod) getPMTs_OP (pmtHits, ev->pmt_high, ev->pmt_low, ecaladc);
        else getPMTs (caloHits, ev->pmt_high, ev->pmt_low, ecaladc);
        getStrips (trackerHits, ev->strip, trkadc);

	
	particleid = trackHits[0].GetPDG();
	energy = trackHits[0].GetKinEnergy();
	gen[0] = trackHits[0].GetPosition().X();
	gen[1] = trackHits[0].GetPosition().Y();
	gen[2] = trackHits[0].GetPosition().Z();
	theta = trackHits[0].GetDirection().Theta()*180/TMath::Pi();//vertical part. theta = 0
	if(theta>90) theta=180-theta;
	phi = trackHits[0].GetDirection().Phi()*180/TMath::Pi();
	
	for(size_t th=0; th<trackerHits.size(); th++){
	  Int_t layerTrack = trackerHits[th].GetDetectorId();
	  if(layerTrack == 2221){
	    SIL1_point[0]=trackerHits[th].GetEntryPoint().X();
	    SIL1_point[1]=trackerHits[th].GetEntryPoint().Y();
	    SIL1_point[2]=trackerHits[th].GetEntryPoint().Z();
	  }
	  //1st plane silicon: layerTrack = 2221
	  //2nd plane silicon: layerTrack = 2121
	  checkIDTrack = layerTrack / 100;
	  TOTALEdep+=trackerHits[th].GetELoss();
	  SILEdep[22-checkIDTrack] += trackerHits[th].GetELoss();
	}

	for(size_t ch=0; ch<caloHits.size(); ch++){
	  Int_t layer = caloHits[ch].GetVolume();
	  TOTALEdep+=caloHits[ch].GetTotalEdep();
	  checkID = layer / 100;
	  if(checkID == 12) TOWEREdep += caloHits[ch].GetTotalEdep();
	  if(checkID == 13) TRIGEdep += caloHits[ch].GetTotalEdep();

	  if(layer==1311) TEdep[0]+=caloHits[ch].GetTotalEdep();
	  if(layer==1312) TEdep[1]+=caloHits[ch].GetTotalEdep();
	  if(layer==1313) TEdep[2]+=caloHits[ch].GetTotalEdep();
	  if(layer==1321) TEdep[3]+=caloHits[ch].GetTotalEdep();
	  if(layer==1322) TEdep[4]+=caloHits[ch].GetTotalEdep();
	  if(layer==1323) TEdep[5]+=caloHits[ch].GetTotalEdep();

	  if(checkID==12) PEdep[1216-layer]+=caloHits[ch].GetTotalEdep();

	  if(layer == 1111) LEdep[0]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1112) LEdep[3]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1113) LEdep[6]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1121) LEdep[1]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1122) LEdep[4]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1123) LEdep[7]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1131) LEdep[2]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1132) LEdep[5]+=caloHits[ch].GetTotalEdep();
	  if(layer == 1133) LEdep[8]+=caloHits[ch].GetTotalEdep();
	}

	for(size_t vh=0; vh<vetoHits.size(); vh++){
	  TOTALEdep+=vetoHits[vh].GetTotalEdep();
	  Int_t layer = vetoHits[vh].GetVolume();

	  if(layer == 1411) VEdep[0]+=vetoHits[vh].GetTotalEdep();
          if(layer == 1412) VEdep[1]+=vetoHits[vh].GetTotalEdep();
	  if(layer == 1421) VEdep[2]+=vetoHits[vh].GetTotalEdep();
	  if(layer == 1422) VEdep[3]+=vetoHits[vh].GetTotalEdep();
	  if(layer == 1430) VEdep[4]+=vetoHits[vh].GetTotalEdep();
	}

	for(size_t ph=0; ph < pmtHits.size(); ph++){
	  for(int i=0; i<53; i++) pmt[i]=pmtHits[ph].GetNPhot(i);
	}
	

	Tmct->Fill();
	
	particleid = 0;
	energy = 0.;
	for (int i=0; i<3; i++) gen[i] = 0.;
	theta = 0.;
	phi = 0.;
	for (int i=0; i<2; i++) SILEdep[i]=0.;
	TRIGEdep=0.;
	TOWEREdep=0.;
	for (int i=0; i<6; i++) TEdep[i] = 0.;
	for (int i=0; i<16; i++) PEdep[i] = 0.;
	for (int i=0; i<9; i++) LEdep[i] = 0.;
	for (int i=0; i<5; i++) VEdep[i] = 0.;
	TOTALEdep = 0.;
	for (int i=0; i<53; i++) pmt[i] = 0;
	for (int i=0; i<3; i++) SIL1_point[i]=0.;

	
        lvl0writer->Fill();
	std::cout << ie << " out of " << ne << "\r" << std::flush;
    }
    delete b_Event;
    delete MCevt;
    std::cout << "Done     " << std::endl;
}




std::string  getMCfilename (int argc, char** argv)
{
    std::string filename = "../../../Simulation/run/Simulations_root/hepd5000_qmd_173MeV_proton_3C0.root"; // Supposing you run from Tools/MC2Lvl0/build/ ; I know, it's ugly :(
    if (argc > 1) filename = argv[1];
    std::cout << "MC2Lvl0: MC file name set to " << filename << std::endl;
    return filename;
}

std::string  getLvl0filename (const std::string mcfilename)
{

    std::vector<std::string> strings;
    std::istringstream f(mcfilename.c_str());
    std::string s;
    while (std::getline(f, s, '/')) {
	    strings.push_back(s);
    }

    std::string lvl0filename = ("MC2Lvl0_"+strings[strings.size()-1]).c_str();
    return lvl0filename;
}




void getPMTs (std::vector<RootCaloHit> CaloHits, ushort* pmt_high, ushort* pmt_low, EcalADC ecaladc )
{
    HitsToEdepPos h2ep;
    std::vector<Edep_Pos> pmt_info = h2ep.Calo2Edep_PosConverter (CaloHits);

    ecaladc.SetPositions(pmt_info);
    ecaladc.NormalizePMThg(pmt_high);
    ecaladc.NormalizePMTlg(pmt_low);
    return;
}


void getPMTs_OP (std::vector<RootPmtHits> pmtHits, ushort* pmt_high, ushort* pmt_low, EcalADC ecaladc )
{
  ecaladc.NormalizePMThg_OP(pmtHits, pmt_high);
  ecaladc.NormalizePMTlg_OP(pmtHits, pmt_low);
  
    return;
}



void getStrips (std::vector<RootTrackerHit> TrackerHits, short* strips, TrackerADC trkadc)
{
    HitsToEdepPos h2ep;
    std::vector<std::vector<Edep_Pos>> TrackerEdepPos = h2ep.Tracker2Edep_PosConverter (TrackerHits);
    std::array<short,NCHAN> trkstrips = trkadc.getStripsFromPos(TrackerEdepPos);
    for (uint ic = 0; ic < NCHAN; ic++) {
        strips[ic] = trkstrips[ic];
    }
    return;
}

int getMCTrackHitsEnergy(std::vector<RootTrackerHit> trHits) {
    int closestEnergy=0;
    int kinEmeas= static_cast <int> (trHits[0].GetKinEnergy());
    int dist=500;
    for (int iEnergy : {37, 51, 100, 125, 154, 174, 228}) {
        int thisdist= abs(kinEmeas - iEnergy);
        if (thisdist<dist)
         {
             dist=thisdist;
             closestEnergy=iEnergy;
         }
    }
    return closestEnergy;
}
