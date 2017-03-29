#include "LEvRec1File.hh"
#include <string>
#include <iostream>


LEvRec1File::LEvRec1File(const char *inpFile, const char *openMode="READ") {
  WritableFLAG=false; // safety
  evptr=0;
  
  std::string omstr(openMode);
  if((omstr.compare("READ"))==0) {                // READ MODE
    fFile=TFile::Open(inpFile, "READ");
    fTree=(TTree*)fFile->Get("L1");
    InitializeBranches();
    /*fTreeMd = (TTree*)fFile->Get("1L1md");
    unsigned short run_id;
    Tmd->SetBranchAddress("run_id",&run_id);
    Tmd->GetEntry(0);
    RunId = static_cast<int>(run_id);*/
    WritableFLAG=false;
  } else if((omstr.compare("WRITE"))==0) {        // WRITE MODE
    fFile = TFile::Open(inpFile, "NEW");
    WritableFLAG=true;
    CreateTrees();
  } else {                                        // UNKNOWN MODE
    std::cerr << "Open mode \"" << openMode << "\" unknwon. No file opened." << std::endl;
  }
}

void LEvRec1File::CreateTrees() {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to create trees for a file not open or open in Read MODE." << std::endl;
    return;
  }

  fFile->cd();
  //fTreeMd = new TTree("L1md", "Limadou Level 1 meta data"); // What do we put inside it???
  fTree = new TTree("L1", "Limadou Level 1 events");
  // Tracker
  tracker_nClBR = fTree->Branch("tracker_nCl", 0, "tracker_nCl/I");
  tracker_seedBR = fTree->Branch("tracker_seed[tracker_nCl]",0, "tracker_seed[tracker_nCl]/I");
  tracker_count0BR = fTree->Branch("tracker_count0[tracker_nCl]", 0, "tracker_count0[tracker_nCl]/D");
  tracker_count1BR = fTree->Branch("tracker_count1[tracker_nCl]",0, "tracker_count1[tracker_nCl]/D");
  tracker_count2BR = fTree->Branch("tracker_count2[tracker_nCl]",0, "tracker_count2[tracker_nCl]/D");
  tracker_count3BR = fTree->Branch("tracker_count3[tracker_nCl]",0, "tracker_count3[tracker_nCl]/D");
  tracker_count4BR = fTree->Branch("tracker_count4[tracker_nCl]",0, "tracker_count4[tracker_nCl]/D");
  tracker_sn0BR = fTree->Branch("tracker_sn0[tracker_nCl]",0, "tracker_sn0[tracker_nCl]/D");
  tracker_sn1BR = fTree->Branch("tracker_sn1[tracker_nCl]",0, "tracker_sn1[tracker_nCl]/D");
  tracker_sn2BR = fTree->Branch("tracker_sn2[tracker_nCl]",0, "tracker_sn2[tracker_nCl]/D");
  tracker_sn3BR = fTree->Branch("tracker_sn3[tracker_nCl]",0, "tracker_sn3[tracker_nCl]/D");
  tracker_sn4BR = fTree->Branch("tracker_sn4[tracker_nCl]",0, "tracker_sn4[tracker_nCl]/D");
  trigger_countHGBR = fTree->Branch("trigger_countHG[12]", 0, "trigger_countHG[12]/D");
  trigger_snHGBR = fTree->Branch("trigger_snHG[12]", 0, "trigger_snHG[12]/D");
  trigger_countLGBR = fTree->Branch("trigger_countLG[12]", 0, "trigger_countLG[12]/D");
  trigger_snLGBR = fTree->Branch("trigger_snLG[12]", 0, "trigger_snLG[12]/D");
  scint_countHGBR = fTree->Branch("scint_countHG[32]", 0, "scint_countHG[32]/D");
  scint_snHGBR = fTree->Branch("scint_snHG[32]", 0, "scint_snHG[32]/D");
  scint_countLGBR = fTree->Branch("scint_countLG[32]", 0, "scint_countLG[32]/D");
  scint_snLGBR = fTree->Branch("scint_snLG[32]", 0, "scint_snLG[32]/D");
  veto_countHGBR = fTree->Branch("veto_countHG[10]", 0, "veto_countHG[10]/D");
  veto_snHGBR = fTree->Branch("veto_snHG[10]", 0, "veto_snHG[10]/D");
  veto_countLGBR = fTree->Branch("veto_countLG[10]", 0, "veto_countLG[10]/D");
  veto_snLGBR = fTree->Branch("veto_snLG[10]", 0, "veto_snLG[10]/D");
  lyso_countHGBR = fTree->Branch("lyso_countHG[9]", 0, "lyso_countHG[9]/D");
  lyso_snHGBR = fTree->Branch("lyso_snHG[9]", 0, "lyso_snHG[9]/D");
  lyso_countLGBR = fTree->Branch("lyso_countLG[9]", 0, "lyso_countLG[9]/D");
  lyso_snLGBR = fTree->Branch("lyso_snLG[9]", 0, "lyso_snLG[9]/D");

  SetAddresses();

  return;
}

void LEvRec1File::InitializeBranches(void) {
  tracker_nClBR = fTree->GetBranch("tracker_nCl");
  tracker_seedBR = fTree->GetBranch("tracker_seed[tracker_nCl]");
  tracker_count0BR = fTree->GetBranch("tracker_count0[tracker_nCl]");
  tracker_count1BR = fTree->GetBranch("tracker_count1[tracker_nCl]");
  tracker_count2BR = fTree->GetBranch("tracker_count2[tracker_nCl]");
  tracker_count3BR = fTree->GetBranch("tracker_count3[tracker_nCl]");
  tracker_count4BR = fTree->GetBranch("tracker_count4[tracker_nCl]");
  tracker_sn0BR = fTree->GetBranch("tracker_sn0[tracker_nCl]");
  tracker_sn1BR = fTree->GetBranch("tracker_sn1[tracker_nCl]");
  tracker_sn2BR = fTree->GetBranch("tracker_sn2[tracker_nCl]");
  tracker_sn3BR = fTree->GetBranch("tracker_sn3[tracker_nCl]");
  tracker_sn4BR = fTree->GetBranch("tracker_sn4[tracker_nCl]");
  trigger_countHGBR = fTree->GetBranch("trigger_countHG[12]");
  trigger_snHGBR = fTree->GetBranch("trigger_snHG[12]");
  trigger_countLGBR = fTree->GetBranch("trigger_countLG[12]");
  trigger_snLGBR = fTree->GetBranch("trigger_snLG[12]");
  scint_countHGBR = fTree->GetBranch("scint_countHG[32]");
  scint_snHGBR = fTree->GetBranch("scint_snHG[32]");
  scint_countLGBR = fTree->GetBranch("scint_countLG[32]");
  scint_snLGBR = fTree->GetBranch("scint_snLG[32]");
  veto_countHGBR = fTree->GetBranch("veto_countHG[10]");
  veto_snHGBR = fTree->GetBranch("veto_snHG[10]");
  veto_countLGBR = fTree->GetBranch("veto_countLG[10]");
  veto_snLGBR = fTree->GetBranch("veto_snLG[10]");
  lyso_countHGBR = fTree->GetBranch("lyso_countHG[9]");
  lyso_snHGBR = fTree->GetBranch("lyso_snHG[9]");
  lyso_countLGBR = fTree->GetBranch("lyso_countLG[9]");
  lyso_snLGBR = fTree->GetBranch("lyso_snLG[9]");

  SetAddresses();  

  return;
}



void LEvRec1File::SetAddresses() {
  if(!fTree) {
    std::cerr << "LEvRec1File::SetAddresses error! fTree=0! " << std::endl;
    return;
  }

  tracker_nClBR->SetAddress(&(evstr.nClusters));
  tracker_seedBR->SetAddress(&(evstr.seed[0]));
  tracker_count0BR->SetAddress(&(evstr.count0[0]));
  tracker_count1BR->SetAddress(&(evstr.count1[0]));
  tracker_count2BR->SetAddress(&(evstr.count2[0]));
  tracker_count3BR->SetAddress(&(evstr.count3[0]));
  tracker_count4BR->SetAddress(&(evstr.count4[0]));
  tracker_sn0BR->SetAddress(&(evstr.sn0[0]));
  tracker_sn1BR->SetAddress(&(evstr.sn1[0]));
  tracker_sn2BR->SetAddress(&(evstr.sn2[0]));
  tracker_sn3BR->SetAddress(&(evstr.sn3[0]));
  tracker_sn4BR->SetAddress(&(evstr.sn4[0]));
  trigger_countHGBR->SetAddress(&(evstr.trigger_countHG[0]));
  trigger_snHGBR->SetAddress(&(evstr.trigger_snHG[0]));
  trigger_countLGBR->SetAddress(&(evstr.trigger_countLG[0]));
  trigger_snLGBR->SetAddress(&(evstr.trigger_snLG[0]));
  scint_countHGBR->SetAddress(&(evstr.scint_countHG[0]));
  scint_snHGBR->SetAddress(&(evstr.scint_snHG[0]));
  scint_countLGBR->SetAddress(&(evstr.scint_countLG[0]));
  scint_snLGBR->SetAddress(&(evstr.scint_snLG[0]));
  veto_countHGBR->SetAddress(&(evstr.veto_countHG[0]));
  veto_snHGBR->SetAddress(&(evstr.veto_snHG[0]));
  veto_countLGBR->SetAddress(&(evstr.veto_countLG[0]));
  veto_snLGBR->SetAddress(&(evstr.veto_snLG[0]));
  lyso_countHGBR->SetAddress(&(evstr.lyso_countHG[0]));
  lyso_snHGBR->SetAddress(&(evstr.lyso_snHG[0]));
  lyso_countLGBR->SetAddress(&(evstr.lyso_countLG[0]));
  lyso_snLGBR->SetAddress(&(evstr.lyso_snLG[0]));

  return;  
}

void LEvRec1File::Fill(const LEvRec1 event) {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to fill a file not open or open in Read MODE." << std::endl;
    return;
  }

  // Stream the LEvRec1 class
  evstr.CopyFromLEvRec1(event);
  fTree->Fill();
    
  return;
}

void LEvRec1File::FillRandom(const int nEvents) {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to fill a file not open or open in Read MODE." << std::endl;
    return;
  }
  for(int i=0; i<nEvents; ++i) {
    LEvRec1 ev;
    ev.FillRandom();
    Fill(ev);
  }
  
  return;
}


int LEvRec1File::SetTheEventPointer(LEvRec1 &ev) {
  if(WritableFLAG) {
    std::cerr << "Trying to address a file open in Write MODE." << std::endl;
    return -999;
  }
  
  evptr=&ev;
  return 0;
}



int LEvRec1File::GetEntry(int iEntry) {
  if(WritableFLAG) {
    std::cerr << "Trying to get the entry of a file open in Write MODE." << std::endl;
    return -999;
  }
  fTree->GetEntry(iEntry);
  evptr->CopyFromLEvRec1Stream(evstr);
  return 0;
}



int LEvRec1File::GetEntries() {
  return fTree->GetEntries();
}

void LEvRec1File::Write() {
  if(fFile && WritableFLAG) fFile->Write();
  return;
}

void LEvRec1File::Close() {
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


LEvRec1File::~LEvRec1File() {
  if(fFile) {
    if(fTree) {
      if(WritableFLAG==false) fTree=0;
      else delete fTree;
    }
    fFile->Close();
  }
}
