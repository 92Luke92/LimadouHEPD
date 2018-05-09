#include "LEvRec1File.hh"
#include "LL1Selector.hh"
#include "detector_const.hh"
#include "LEvRec0.hh"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "TFile.h"
#include "TTree.h"

int main(int argc, char **argv) {
  if(argc!=3) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./L1Analysis_ForSummer2018 <inputFileList> <outputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }

  ///////////////////////
  LL1Selector cleaner;
  std::string selection("NoCorrTAny");
  cleaner.LoadConfiguration(selection);
  cleaner.SetThreshold(3.);

  LL1Selector veto;
  std::string veto_str("Veto");
  veto.LoadConfiguration(veto_str);
  veto.SetThreshold(3.);

  LL1Selector StrengthenTrigger;
  std::string mask3_str("DMask3");
  StrengthenTrigger.LoadConfiguration(mask3_str);
  StrengthenTrigger.SetThreshold(10.);

  const unsigned long IntegrationTime = 10; // unit: usec
  const double DISC_THRESH = 5.;
  const double LASTPLANEHIT_THRESH = 10.;
  const double PLANETOBAR_THRESH = 10.;
  const double SCINT_THRESH = 10.;
  const double BAR_THRESH = 10.;
  ///////////////////////


  LEvRec1 cev;
    
  TFile fout(argv[2],"new");
  TTree L2("L2","L2");
  L2.Branch("event_index", &cev.event_index, "event_index/i");
  L2.Branch("boot_nr", &cev.boot_nr, "boot_nr/s");
  L2.Branch("run_id", &cev.run_id, "run_id/s");
  
  bool discontinuity;
  L2.Branch("discontinuity", &discontinuity, "discontinuity/O");
  int LastPlaneHit;
  L2.Branch("LastPlaneHit", &LastPlaneHit, "LastPlaneHit/I");
  double PlaneToBar;
  L2.Branch("PlaneToBar", &PlaneToBar, "PlaneToBar/D");
  double ScintCounts;
  L2.Branch("ScintCounts", &ScintCounts, "ScintCounts/D");
  double TriggerCounts;
  L2.Branch("TriggerCounts", &TriggerCounts, "TriggerCounts/D");
  double TrackerMSSN;
  L2.Branch("TrackerMSSN", &TrackerMSSN, "TrackerMSSN/D");
  double TrackerMSCounts;
  L2.Branch("TrackerMSCounts", &TrackerMSCounts, "TrackerMSCounts/D");
  unsigned long EventTime;
  L2.Branch("EventTime", &EventTime, "EventTime/l");
  
  std::ifstream fin(argv[1],std::ifstream::in);
  std::string fname;
  while(1) {
    fin >> fname;
    if(fin.eof()) break;
    std::cout << "Working on " << fname << std::endl;
    
    LEvRec1File inputFile(fname.c_str(),"READ");
    inputFile.SetTheEventPointer(cev);

    inputFile.GetMDEntry(0);
    unsigned long  startOBDHTime = static_cast<unsigned long>(cev.lev0MD.broadcast.OBDH.sec)*1000000; //usec
    unsigned long  startOBDHTimeStamp = static_cast<unsigned long>(cev.lev0MD.timestamp.OBDH)*1000; //usec
    inputFile.GetMDEntry(1);
    unsigned long  CPUTimeStartRun = static_cast<unsigned long>(cev.lev0MD.CPU_time[0])*1000; //usec  // even if it is start, it is written only in the tail
    unsigned long  stopOBDHTime = static_cast<unsigned long>(cev.lev0MD.broadcast.OBDH.sec)*1000000; //usec
    unsigned long  stopOBDHTimeStamp = static_cast<unsigned long>(cev.lev0MD.timestamp.OBDH)*1000; //usec
    unsigned long  CPUTimeStopRun = static_cast<unsigned long>(cev.lev0MD.CPU_time[1])*1000; //usec  

    const unsigned long startRunAbsoluteTime = startOBDHTime-startOBDHTimeStamp+CPUTimeStartRun;
    const unsigned long stopRunAbsoluteTime = stopOBDHTime-stopOBDHTimeStamp+CPUTimeStopRun;
 
   /* std::cout << "startOBDHTime " << startOBDHTime << "   "
    <<	"startOBDHTimeStamp " << startOBDHTimeStamp << "   "
    << "CPUTimeStartRun " << CPUTimeStartRun << "   "
    << "startRunAbsoluteTime "  << startRunAbsoluteTime << std::endl <<std::flush; */

    int counter=0;
 	int nentries=inputFile.GetEntries();
    for(int ie=0; ie<nentries; ++ie) {
      inputFile.GetEntry(ie);
      if(veto.Select(cev)) continue;
      if(!cleaner.Select(cev)) continue;
      if(!StrengthenTrigger.Select(cev)) continue;
      
      if(counter<10) {
      	cev.scint.Dump();
      	++counter;
      }
      //std::cout << ie << " " << std::flush;

      discontinuity = cev.DiscontinousSignal(DISC_THRESH);
  	  LastPlaneHit = cev.GetLastPlaneHit(LASTPLANEHIT_THRESH);
      PlaneToBar = cev.GetMSPlaneToMSBarRatio(PLANETOBAR_THRESH);
      ScintCounts = cev.GetScintCounts(SCINT_THRESH);
      TriggerCounts = cev.GetTriggerCounts(BAR_THRESH);
      TrackerMSSN = cev.tracker.GetMSClusterSN();
  	  TrackerMSCounts = cev.tracker.GetMSClusterCounts();
  
	  EventTime = startRunAbsoluteTime+static_cast<unsigned long>(cev.hepd_time)*10;
      //if(newTime-oldTime>IntegrationTime) 
      L2.Fill();
      
    }// entries loop
    inputFile.Close();
  }//file loop

  fout.Write();
  fout.Close();

  return 0;
}
