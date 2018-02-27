#include "LEvRec1File.hh"
#include <iostream>

int main(int argc, char **argv) {
  if(argc!=2) {
    std::cerr << "************ Usage:" << std::endl
	      << "************ ~user> ./L1Reader <inputFile>" << std::endl
	      << "************ aborting."
	      << std::endl;
    return -999;
  }
  
  LEvRec1File inputFile(argv[1],"READ");
  LEvRec1 cev;
  inputFile.SetTheEventPointer(cev);

  int nentries=inputFile.GetEntries();
  for(int ie=0; ie<nentries; ++ie) {
    inputFile.GetEntry(ie);
    if(ie == 0 && inputFile.ContainsMD()) // to dump only header run (entry 0 of MD Tree)
       inputFile.GetMDEntry(ie);
    if(ie%1000==0)
       cev.Dump(ie);
    
  }

  inputFile.Close();

  return 0;
}
