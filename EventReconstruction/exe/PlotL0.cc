// here to start documentation
// author: vincenzo vitale, vincenzo.vitale@roma2.infn.it
//
// class purpose: an example of L0 data I/O 


#include "LEvRec0File.hh"
#include <iostream>
#include "TH1.h"

int main(int argc, char **argv) {

// input argument

  if(argc!=2) {

    std::cout << "Usage:" << " user> PlotL0 <inputFile>" << std::endl;
	     
   return -999;
 
  }
  
// output histo

  TH1D *h1 = new TH1D("Test Histo","",333,0,4000);

// data interfacing 

  LEvRec0File File(argv[1]);

  LEvRec0 cev;

  File.SetTheEventPointer(cev);

  int nEntries=File.GetEntries();

// event loop

  for(int ie=0; ie<nEntries; ++ie) {

    File.GetEntry(ie);

    h1->Fill(cev.pmt_high[0]);

  }// event loop

  File.Close();

// histo output

  TFile f("histos.root", "new");

  h1->Write();

  return 0;
}
