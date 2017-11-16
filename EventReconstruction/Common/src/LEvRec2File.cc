#include "LEvRec2File.hh"
#include <string>
#include <iostream>

LEvRec2File::LEvRec2File(const char *inpFile, const char *openMode="READ") {
  WritableFLAG=false; // safety
  evptr=0;
  
  std::string omstr(openMode);
  if((omstr.compare("READ"))==0) {                // READ MODE
    fFile=TFile::Open(inpFile, "READ");
    fTree=(TTree*)fFile->Get("L2");
    InitializeBranches();
    WritableFLAG=false;
  } else if((omstr.compare("WRITE"))==0) {        // WRITE MODE
    fFile = TFile::Open(inpFile, "NEW");
    WritableFLAG=true;
    CreateTrees();
  } else {                                        // UNKNOWN MODE
    std::cerr << "Open mode \"" << openMode << "\" unknwon. No file opened." << std::endl;
  }
}


void LEvRec2File::CreateTrees() {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to create trees for a file not open or open in Read MODE." << std::endl;
    return;
  }

  fFile->cd();

  fTreeMd = new TTree("L2md", "Limadou Level 1 meta data");
  fTree = new TTree("L2", "Limadou Level 1 events");
  
  //Braches
  Francesco = fTree->Branch("Francesco_Energy", 0, "FrancescoEnergy/F");
  Ester = fTree->Branch("Ester_dE_dx",0, "EsterdE/dx/F");
  Roberto = fTree->Branch("Roberto_Id", 0, "Roberto_Id/I");

  SetAddresses();

  return;
}

void LEvRec2File::InitializeBranches(void) {

  SetAddresses();  
  return;
}



void LEvRec2File::SetAddresses() {
  if(!fTree) {
    std::cerr << "LEvRec2File::SetAddresses error! fTree=0! " << std::endl;
    return;
  }

  return;  
}

void LEvRec2File::Fill(const LEvRec2 event) {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to fill a file not open or open in Read MODE." << std::endl;
    return;
  }
  // Stream the LEvRec2 class
  evstr.CopyFromLEvRec2(event);
  fTree->Fill();
    
  return;
}

int LEvRec2File::GetEntry(int iEntry) {
  if(WritableFLAG) {
    std::cerr << "Trying to get the entry of a file open in Write MODE." << std::endl;
    return -999;
  }
  fTree->GetEntry(iEntry);
  evptr->CopyFromLEvRec2Stream(evstr);
  return 0;
}


int LEvRec2File::GetEntries() {
  return fTree->GetEntries();
}

void LEvRec2File::Write() {
  if(fFile && WritableFLAG) fFile->Write();
  return;
}

void LEvRec2File::Close() {
  if(fFile) {
    if(fTree) {
      if(WritableFLAG==false) fTree=0;
      else delete fTree;
    }
    fFile->Close();
    fFile=0;
  }
  return;
}

LEvRec2File::~LEvRec2File() {
  if(fFile) {
    if(fTree) {
      if(WritableFLAG==false) fTree=0;
      else delete fTree;
    }
    fFile->Close();
  }
}




