#include "LEvRec1File.hh"
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
  mytrig.LoadConfiguration("francesco");
  LEvRec1File inputFile(argv[1],"READ");
  LEvRec1 cev;
  inputFile.SetTheEventPointer(cev);
  int nentries=inputFile.GetEntries();
  int ottcounter = 0;
  for(int ie=0; ie<nentries; ++ie) {
    inputFile.GetEntry(ie);
    if(ie%1000==0)cev.Dump();
    if(mytrig.Trigger(cev)) ++ottcounter;
  }

  inputFile.Close();
  std::cout<<static_cast<double> (ottcounter)/nentries << std::endl;
  return 0;
}
