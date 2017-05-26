#include "LEvRec0File.hh"

LEvRec0File::LEvRec0File() {
  fFile=0;
  fTree=0;
  RunId=-999;
  BootNr=-999;
}

LEvRec0File::LEvRec0File(const char *inpFile) {
  Open(inpFile);
}


void LEvRec0File::Open(const char *inpFile) {
  fFile=TFile::Open(inpFile, "READ");
  //calibration run processing...
  fTree=(TTree*)fFile->Get("T");
  TTree *Tmd = (TTree*)fFile->Get("Tmd");
  unsigned short run_id;
  Tmd->SetBranchAddress("run_id",&run_id);
  unsigned short boot_nr;
  Tmd->SetBranchAddress("boot_nr",&boot_nr);
  Tmd->GetEntry(0);
  RunId = static_cast<int>(run_id);
  BootNr = static_cast<int>(boot_nr);

  return;
}

void LEvRec0File::Reset() {
  if(fFile) {
    fTree=0;
    fFile->Close();
  }
  RunId=-999;
  
  return;
}

int LEvRec0File::SetTheEventPointer(LEvRec0 &ev) {
  fTree->SetBranchAddress("run_type", &ev.runType);
  fTree->SetBranchAddress("ev_boot_nr", &ev.boot_nr);
  fTree->SetBranchAddress("ev_run_id", &ev.run_id);
  fTree->SetBranchAddress("event_index", &ev.event_index);
  fTree->SetBranchAddress("event_length", &ev.event_length);
  fTree->SetBranchAddress("trigger_index", &ev.trigger_index);
  fTree->SetBranchAddress("hepd_time", &ev.hepd_time);
  fTree->SetBranchAddress("pmt_high[64]", &ev.pmt_high);
  fTree->SetBranchAddress("pmt_low[64]", &ev.pmt_low);
  fTree->SetBranchAddress("PMTBoard_trigger_counter", &ev.PMTBoard_trigger_counter);
  fTree->SetBranchAddress("lost_trigger", &ev.lost_trigger);
  fTree->SetBranchAddress("rate_meter[9]", &ev.rate_meter);
  fTree->SetBranchAddress("trigger_flag[64]", &ev.trigger_flag);
  fTree->SetBranchAddress("alive_time", &ev.alive_time);
  fTree->SetBranchAddress("dead_time", &ev.dead_time);
  fTree->SetBranchAddress("strip[4608]",&ev.strip);
  
  fTree->SetBranchStatus("*",kTRUE);
  
  return 0;
}



int LEvRec0File::GetEntry(int iEntry) {
  fTree->GetEntry(iEntry);
  return 0;
}



int LEvRec0File::GetEntries() {
  return fTree->GetEntries();
}

void LEvRec0File::Close() {
  Reset();
  return;
}



LEvRec0File::~LEvRec0File() {
  Reset();
}
