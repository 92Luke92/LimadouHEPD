// Local
#include "detectorID.h"
#include "PMTID.h"
#include "LEvRec0Writer.hh"

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



int main(int argc, char** argv) {

   const std::string mcfilename=getMCfilename(argc, argv);
   const std::string lvl0filename=getLvl0filename(mcfilename);

   TFile* filemc= TFile::Open(mcfilename.c_str(), "READ");
   TTree* Tmc = (TTree*)filemc->Get("HEPD/EventTree");
   std::cout << Tmc->GetEntries() << std::endl;

   LEvRec0Writer lvl0writer(lvl0filename);
   LEvRec0 evtstruc;
   lvl0writer.SetTheEventPointer(evtstruc);

   for (int i=0; i<100; i++) {
      evtstruc.Reset();
      evtstruc.pmt_high[2]=i*i;
      lvl0writer.Fill();
   }

   lvl0writer.Write();

   RootTrackerHit hit;
   std::cout << " >>> ELOSS 3" << hit.GetELoss() << std::endl;
   return 0;

   TTree T("T", "T");
   //TTree outtree=GetTTreeT();

}






