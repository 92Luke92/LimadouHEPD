#include "LEvRec1File.hh"
#include "LEvRec1.hh"
#include "LOfflineTrigger.hh"
#include <iostream>

int main(int argc, char **argv) {
  if(argc!=2) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./OTT <inputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }
  LOfflineTrigger mytrig;
  //mytrig.LoadConfiguration("(T2.OR.T3).AND.VB");
  mytrig.LoadConfiguration("central_muon_tight");
  LOfflineTrigger veto;
  veto.LoadConfiguration("VL");
  LEvRec1File inputFile(argv[1],"READ");
  LEvRec1 cev;
  inputFile.SetTheEventPointer(cev);
  int nentries=inputFile.GetEntries();
  int ottcounter = 0;
  int scint_counters[16];
  int veto_counters=0;
  double threshold = 5.; 
  for(int iscint=0; iscint<NSCINTPLANES; ++iscint) scint_counters[iscint]=0;
  for(int ie=0; ie<nentries; ++ie) {
    inputFile.GetEntry(ie);
    //if(ie%1000==0)cev.Dump();
    if(mytrig.Trigger(cev)&&(!veto.Trigger(cev))) {
      ++ottcounter;
      for(int iscint=0; iscint<NSCINTPLANES; ++iscint){
        if((cev.scint.sn_hg[iscint][0]+cev.scint.sn_hg[iscint][1])>threshold) ++scint_counters[iscint];
      }
      if((cev.veto.sn_hg[4][0]+cev.veto.sn_hg[4][1])>threshold) ++veto_counters;
    }
  }

  inputFile.Close();
  std::cout<<" Tot events "<<nentries<<" , OTT fraction "<<static_cast<double> (ottcounter)/nentries<< std::endl;
  for(int iscint=0; iscint<NSCINTPLANES; ++iscint){
    std::cout<<" The efficiency of scintillator plane "<< iscint <<" is "<<static_cast<double> (scint_counters[iscint])/(ottcounter) << std::endl;
  }
  std::cout<<" The efficiency of bottom veto plane is "<<static_cast<double> (veto_counters)/(ottcounter) << std::endl;
  return 0;
}
