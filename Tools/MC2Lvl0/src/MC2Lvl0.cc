// Local
#include "detectorID.h"
#include "PMTID.h"
#include "LEvRec0Writer.hh"
#include "MCcoorPhysicalFrame.hh"

// C++ std
#include <iostream>
#include <string>

// MCEventAnalyze
#include "RootEvent.hh"
#include "RootTrack.hh"
#include "RootVertex.hh"
#include "RootCaloHit.hh"
#include "RootTrackerHit.hh"

// ROOT libs
#include "TTree.h"
#include "TVector3.h"
#include "TTree.h"
#include "TFile.h"
#include "TBranch.h"

//Various
#include "LEvRec0.hh"



float Vector3Dist (TVector3 v1, TVector3 v2);
std::string  getMCfilename (int argc, char** argv);
std::string  getLvl0filename (const std::string mcfilename);
void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc);
void getPMThigh(std::vector<RootCaloHit>, ushort* pmt_high);
void getPMTlow (std::vector<RootCaloHit>, ushort* pmt_low);
void getStrips (std::vector<RootTrackerHit>, short* strips);



int main (int argc, char** argv)
{
    const std::string mcfilename = getMCfilename (argc, argv);
    const std::string lvl0filename = getLvl0filename (mcfilename);
    TFile* filemc = TFile::Open (mcfilename.c_str(), "READ");
    TTree* Tmc = (TTree*) filemc->Get ("HEPD/EventTree");
    std::cout << Tmc->GetEntries() << std::endl;
    LEvRec0Writer lvl0writer (lvl0filename);
    LoopOnEvents (&lvl0writer, Tmc);
    lvl0writer.Write();
    lvl0writer.Close();
    delete Tmc;
    filemc->Close();
    delete filemc;
    return 0;
}




void LoopOnEvents (LEvRec0Writer* lvl0writer, TTree* Tmc)
{
    int ne = Tmc->GetEntries();
    RootEvent* MCevt = new RootEvent;
    TBranch* b_Event = new TBranch;
    Tmc->SetBranchAddress ("Event", &MCevt, &b_Event);
    for (int ie = 0; ie < ne; ie++) {
        int nb = Tmc->GetEntry (ie);
        int eventid =  MCevt->EventID();
        std::vector<RootTrack> rootTracks =  MCevt->GetTracks();
        std::vector<RootCaloHit> caloHits =  MCevt->GetCaloHit();
        std::vector<RootTrackerHit>  trackerHits =  MCevt->GetTrackerHit();

        LEvRec0* ev = lvl0writer->pev();
        ev->Reset();
        getPMThigh(caloHits, ev->pmt_high);
        getPMTlow(caloHits, ev->pmt_low);
        getStrips(trackerHits, ev->strip);


        lvl0writer->Fill();
        std::cout << ie << " " << nb << "\r" << std::flush;
    }
    delete b_Event;
    delete MCevt;
    std::cout << "Done" << std::endl;
}


float Vector3Dist (TVector3 v1, TVector3 v2)
{
    TVector3 diff = v1 - v2;
    return static_cast<float> (diff.Mag() ); // get magnitude (=rho=Sqrt(x*x+y*y+z*z)))
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
    std::string lvl0filename = "test.root";
    return lvl0filename;
}



void getPMThigh(std::vector<RootCaloHit>, ushort* pmt_high) {
   for (uint ip=0; ip<NPMT; ip++) {
      pmt_high[ip]=ip;
   }
   return;
}

void getPMTlow(std::vector<RootCaloHit>, ushort* pmt_low) {
   for (uint ip=0; ip<NPMT; ip++) {
      pmt_low[ip]=ip;
   }
   return;
}


void getStrips (std::vector<RootTrackerHit>, short* strips) {
   for (uint ic=0; ic<NCHAN; ic++) {
      strips[ic]=ic%20;
   }
   return;
}
