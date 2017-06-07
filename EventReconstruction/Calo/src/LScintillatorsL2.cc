#include "TH1I.h"
#include "LEvRec1File.hh"
#include "LScintillatorsL2.hh"
#include <iostream>



LScintillatorsL2::LScintillatorsL2() {
  
}


LScintillatorsL2 &LScintillatorsL2::GetInstance() {
  static LScintillatorsL2 instance;  // Guaranteed to be destroyed.
  return instance;
}


//---------------------------------------------------------------------------

int LScintillatorsL2::LoadRun(const std::string inputFileROOT) {
  


// Reset previous loaded runs
  if (RunFile) {
    if (RunFile->IsOpen()) RunFile->Close();
    RunFile = 0;
  }

  RunFile = new LEvRec1File(inputFileROOT.c_str(), "READ");
  if (!RunFile || !(RunFile->IsOpen())) {
    std::cerr << __LScintillatorsL2__
              << "Error! Attempt to load a data run, but the file "
                 "cannot be opened."
              << std::endl;
    RunFile = 0;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

//---------------------------------------------------------------------------


int LScintillatorsL2::Test() {
  

LEvRec1 cev;
TH1D *h1 = new TH1D("","",100,0,4000);
RunFile->SetTheEventPointer(cev);

int nEvents = RunFile->GetEntries();

for (int iEv = 0; iEv < nEvents; iEv++)  // Event loop
  {
    std::cout << "\b\b\b" << std::setprecision(2) << std::setw(2)
              << int(double(iEv) / double(nEvents - 1) * 100) << "%"
              << std::flush;
    RunFile->GetEntry(iEv);   
     

  }
cev.DumpTrigger();
//h1->Draw("");
return 0;
}



//---------------------------------------------------------------------------

LScintillatorsL2::~LScintillatorsL2() {
  // do not care about singleton destructor
}

