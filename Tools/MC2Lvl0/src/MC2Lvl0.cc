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


float Vector3Dist(TVector3 v1, TVector3 v2)
{
   TVector3 diff= v1-v2;
   return static_cast<float> (diff.Mag()); // get magnitude (=rho=Sqrt(x*x+y*y+z*z)))

}



std::string  getMCfilename(int argc, char** argv) {
   std::string filename="/storage/gpfs_data/limadou/mpuel/HEPDSW_had_v2/run/Simulations_root/hepd5000_qmd_173MeV_proton_3C0.root";
   if (argc>1) filename=argv[1];
   std::cout << "MC2Lvl0: MC file name set to " << filename << std::endl;
   return filename;

}

std::string  getLvl0filename(const std::string mcfilename) {
   std::string lvl0filename="test.root";
   return lvl0filename;
}



void LoopOnEvents(LEvRec0Writer* lvl0writer, TTree* Tmc);



int main(int argc, char** argv) {

   const std::string mcfilename=getMCfilename(argc, argv);
   const std::string lvl0filename=getLvl0filename(mcfilename);

   TFile* filemc= TFile::Open(mcfilename.c_str(), "READ");
   TTree* Tmc = (TTree*)filemc->Get("HEPD/EventTree");
   std::cout << Tmc->GetEntries() << std::endl;

   LEvRec0Writer lvl0writer(lvl0filename);
   LoopOnEvents(&lvl0writer, Tmc);

   lvl0writer.Write();
   lvl0writer.Close();
   delete Tmc;
   filemc->Close();
   delete filemc;

   return 0;


}




void LoopOnEvents(LEvRec0Writer* lvl0writer, TTree* Tmc) {
   int ne=Tmc->GetEntries();

   RootEvent* MCevt=0;
   TBranch* b_Event=0;
   Tmc->SetBranchAddress("Event", &MCevt, &b_Event);

   for (int ie=0; ie<ne; ie++) {


      int nb = Tmc->GetEntry(ie);
      std::cout << ie << " " << nb << "\r" << std::flush;

        int eventid =  MCevt->EventID();
        //myTracks =  MCevt->GetTracks();
        //myCaloHit =  MCevt->GetCaloHit();
        //myVetoHit =  MCevt->GetVetoHit();
        //myTrackerHit =  MCevt->GetTrackerHit();

      LEvRec0* ev=lvl0writer->pev();
      ev->Reset();
      //ev->pmt_high=Getpmt_high();
      //ev->pmt_low =Getpmt_low ();
      lvl0writer->Fill();
   }
   std::cout << "Done" << std::endl;
}
