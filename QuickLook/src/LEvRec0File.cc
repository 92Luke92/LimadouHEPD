#include "LEvRec0File.hh"


LEvRec0File::LEvRec0File(const char *inpFile) {
  inputCalib=TFile::Open(inpFile, "READ");
  // calibration run processing...
  treeCalib=(TTree*)inputCalib->Get("T");
  Tmd = (TTree*)inputCalib->Get("Tmd");
}


int LEvRec0File::SetTheEventPointer(LEvRec0 &ev) {
   treeCalib->SetBranchAddress("run_type", &ev.runType);
   treeCalib->SetBranchAddress("ev_boot_nr", &ev.boot_nr);
   treeCalib->SetBranchAddress("ev_run_id", &ev.run_id);
   treeCalib->SetBranchAddress("event_index", &ev.event_index);
   treeCalib->SetBranchAddress("event_length", &ev.event_length);
   treeCalib->SetBranchAddress("trigger_index", &ev.trigger_index);
   treeCalib->SetBranchAddress("hepd_time", &ev.hepd_time);
   treeCalib->SetBranchAddress("pmt_high[64]", &ev.pmt_high);
   treeCalib->SetBranchAddress("pmt_low[64]", &ev.pmt_low);
   treeCalib->SetBranchAddress("PMTBoard_trigger_counter",&ev.PMTBoard_trigger_counter);
   treeCalib->SetBranchAddress("lost_trigger", &ev.lost_trigger);
   treeCalib->SetBranchAddress("rate_meter[9]", &ev.rate_meter);
   treeCalib->SetBranchAddress("trigger_flag[64]", &ev.trigger_flag);
   treeCalib->SetBranchAddress("alive_time", &ev.alive_time);
   treeCalib->SetBranchAddress("dead_time", &ev.dead_time);
   treeCalib->SetBranchAddress("strip[4608]",&ev.strip);

  // treeCalib->SetBranchStatus("*",kFALSE);
  // treeCalib->SetBranchStatus("strip[4608]",kTRUE);
  // treeCalib->SetBranchStatus("event_index",kTRUE);

  return 0;
}

int LEvRec0File::SetTmdPointer(LEvRec0Md &metaData) {
   Tmd->SetBranchAddress("boot_nr", &metaData.boot_nr);
   Tmd->SetBranchAddress("run_id", &metaData.run_id);
   Tmd->SetBranchAddress("run_type", &metaData.run_type);
   Tmd->SetBranchAddress("run_duration", &metaData.run_duration);
   Tmd->SetBranchAddress("orbitZone", &metaData.orbit_Zone);
   
   Tmd->SetBranchAddress("silConfiguration", &metaData.silConfig.ladder_on);

   Tmd->SetBranchAddress("trigger_mask[2]", &metaData.trigger_mask[2]);
   Tmd->SetBranchAddress("easiroc[118]", &metaData.easiroc_config[0]);
   Tmd->SetBranchAddress("PMT_mask[64]", &metaData.PMT_mask[0]);
   Tmd->SetBranchAddress("HV_mask[12]", &metaData.HV_mask[12]);
   Tmd->SetBranchAddress("HV_value[10]", &metaData.HV_value[0]);
   Tmd->SetBranchAddress("gen_trig_mask[4]", &metaData.gen_trig_mask[0]);

   // broadcasta data
   Tmd->SetBranchAddress("OBDH_info", &metaData.broadcast.OBDH.sec);
   Tmd->SetBranchAddress("GPS_info", &metaData.broadcast.GPS.sec);
   Tmd->SetBranchAddress("AOCC_info", &metaData.broadcast.AOCC.sec);
   Tmd->SetBranchAddress("star_sensor_info", &metaData.broadcast.star_sensor.sec_ss11);

   // scinentific packet
   Tmd->SetBranchAddress("PMT_rate_meter[65]", &metaData.PMT_rate_meter[0]);
   Tmd->SetBranchAddress("CPU_temp_start_stop_Run[2]", &metaData.CPU_temp[0]);
   Tmd->SetBranchAddress("PMT_temp_start_stop_Run[2]", &metaData.PMT_temp[0]);
   Tmd->SetBranchAddress("CPU_time_start_stop_Run[2]", &metaData.CPU_time[0]);

   // timestamp CPU 
    Tmd->SetBranchAddress("CPU_timestamp", &metaData.timestamp.OBDH);

   // board status 
   Tmd->SetBranchAddress("status_register", &metaData.status_reg.statusDAQ);

   // Tmd->SetBranchStatus("*",kFALSE);
   // Tmd->SetBranchStatus("boot_nr", &metaData.boot_nr);
   // Tmd->SetBranchStatus("run_id", &metaData.run_id);

   return 0;
}


int LEvRec0File::GetEntry(int iEntry) {
  treeCalib->GetEntry(iEntry);
  Tmd->GetEntry(iEntry);
  return 0;
}

int LEvRec0File::GetEntries() {
  return treeCalib->GetEntries();
}


int LEvRec0File::GetTmdEntries() {
  return Tmd->GetEntries();
}


unsigned short LEvRec0File::GetRunID(int iEntry) {
   unsigned short runID;
   Tmd->SetBranchAddress("run_id", &runID);
   Tmd->GetEntry(iEntry);
   return runID;
}

unsigned short LEvRec0File::GetBootNr(int iEntry) {
   unsigned short  bootNr;
   Tmd->SetBranchAddress("boot_nr", &bootNr);
   Tmd->GetEntry(iEntry);
   return bootNr;
}

void LEvRec0File::Close() {
  if(inputCalib) {
    treeCalib = 0;
    Tmd = 0;
    inputCalib->Close();
    inputCalib = 0;
  }
  return;
}

LEvRec0File::~LEvRec0File() {
  if(inputCalib) {
     treeCalib = 0;
     Tmd = 0;
    inputCalib->Close();
  }
}

