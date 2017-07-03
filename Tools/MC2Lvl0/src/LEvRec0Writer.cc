#include "LEvRec0Writer.hh"



LEvRec0Writer::LEvRec0Writer(std::string filename) {
  Init(filename);
}


void LEvRec0Writer::Init(std::string filename) {
  fFile=TFile::Open(filename.c_str(), "RECREATE");
  fTree=new TTree("T", "T");
  SetTheEventPointer();
  return;
}

void LEvRec0Writer::Close() {
  fFile->Close();

}


void LEvRec0Writer::SetTheEventPointer() {
  fTree->Branch("run_type", &ev.runType);
  fTree->Branch("ev_boot_nr", &ev.boot_nr);
  fTree->Branch("ev_run_id", &ev.run_id);
  fTree->Branch("event_index", &ev.event_index);
  fTree->Branch("event_length", &ev.event_length);
  fTree->Branch("trigger_index", &ev.trigger_index);
  fTree->Branch("hepd_time", &ev.hepd_time);
  fTree->Branch("pmt_high[64]", ev.pmt_high);
  fTree->Branch("pmt_low[64]", ev.pmt_low);
  fTree->Branch("PMTBoard_trigger_counter", &ev.PMTBoard_trigger_counter);
  fTree->Branch("lost_trigger", &ev.lost_trigger);
  fTree->Branch("rate_meter[9]", ev.rate_meter);
  fTree->Branch("trigger_flag[64]", ev.trigger_flag);
  fTree->Branch("alive_time", &ev.alive_time);
  fTree->Branch("dead_time", &ev.dead_time);
  fTree->Branch("strip[4608]",ev.strip);

}


LEvRec0Writer::~LEvRec0Writer() {
}
