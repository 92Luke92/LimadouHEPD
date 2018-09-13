#include "LEvRec0File.hh"
#include <stdio.h>
#include <iostream>



LEvRec0File::LEvRec0File(const char *inpFile) {
  inputCalib=TFile::Open(inpFile, "READ");

  // calibration run processing...

  treeCalib=(TTree*)inputCalib->Get("T");
  Tmd = (TTree*)inputCalib->Get("Tmd");
  TConf=(TTree*)inputCalib->Get("TConf");
  THVpmt=(TTree*)inputCalib->Get("THVpmt");


  unsigned short runid;
  Tmd->SetBranchAddress("run_id", &runid);

  int entries = Tmd->GetEntries();
  RunId = new unsigned short[entries];
  
  for (int i = 0; i < entries; i++)
   {
      Tmd->GetEntry(i);
      RunId[i] = runid;
   }
  unsigned short run_type; 
  Tmd->SetBranchAddress("run_type",&run_type);
  Tmd->GetEntry(0);
  runType = run_type;

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
   
   if(IsZeroSuppressed())
   {
      treeCalib->SetBranchAddress("clust_nr", &ev.clust_nr);
      treeCalib->SetBranchAddress("cluster", &ev.cluster);
   }
   else 
      treeCalib->SetBranchAddress("strip[4608]", &ev.strip);
   
   // treeCalib->SetBranchStatus("*",kFALSE);
   
  return 0;
}

int LEvRec0File::SetTmdPointer(LEvRec0Md &metaData) {
   Tmd->SetBranchAddress("boot_nr", &metaData.boot_nr);
   Tmd->SetBranchAddress("run_id", &metaData.run_id);
   Tmd->SetBranchAddress("run_type", &metaData.run_type);
   Tmd->SetBranchAddress("run_duration", &metaData.run_duration);
   Tmd->SetBranchAddress("orbitZone", &metaData.orbit_Zone);
   
   Tmd->SetBranchAddress("silConfiguration", &metaData.silConfig.ladder_on);

   Tmd->SetBranchAddress("trigger_mask[2]", &metaData.trigger_mask[0]);
   Tmd->SetBranchAddress("easiroc_conf[60]", &metaData.easiroc_config[0]);
   Tmd->SetBranchAddress("PMT_mask[64]", &metaData.PMT_mask[0]);
   Tmd->SetBranchAddress("HV_mask[12]", &metaData.HV_mask[0]);
   Tmd->SetBranchAddress("HV_value[10]", &metaData.HV_value[0]);
   Tmd->SetBranchAddress("gen_trig_mask[18]", &metaData.gen_trig_mask[0]);

   // broadcasta data
   Tmd->SetBranchAddress("OBDH_info", &metaData.broadcast.OBDH.sec);
   Tmd->SetBranchAddress("GPS_info", &metaData.broadcast.GPS.sec);
   Tmd->SetBranchAddress("AOCC_info", &metaData.broadcast.AOCC.sec);
   Tmd->SetBranchAddress("star_sensor_info", &metaData.broadcast.star_sensor.sec_ss11);

   // scientific packet
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

int LEvRec0File::SetTConfPointer(LEvRec0Conf &dummyPKT) {

  TConf->SetBranchAddress("boot_nr", &dummyPKT.dummy_pkt.boot_nr);
  TConf->SetBranchAddress("run_id", &dummyPKT.dummy_pkt.run_id);
  
  // orbit zone 1 configuration
   TConf->SetBranchAddress("orbit_conf_1", &dummyPKT.dummy_pkt.orbit_conf[0].ladder_mask);
   

   // orbit zone 2 configuration
   TConf->SetBranchAddress("orbit_conf_2", &dummyPKT.dummy_pkt.orbit_conf[1].ladder_mask);


   // orbit zone 3 configuration
   TConf->SetBranchAddress("orbit_conf_3", &dummyPKT.dummy_pkt.orbit_conf[2].ladder_mask);


   // orbit zone 4 configuration
   TConf->SetBranchAddress("orbit_conf_4", &dummyPKT.dummy_pkt.orbit_conf[3].ladder_mask);

  
   // orbit zone 5 configuration
   TConf->SetBranchAddress("orbit_conf_5", &dummyPKT.dummy_pkt.orbit_conf[4].ladder_mask);


   // Other Configurations
   TConf->SetBranchAddress("user_orbital_settings", &dummyPKT.dummy_pkt.user_orbital_settings);
   TConf->SetBranchAddress("WO_config_ID", &dummyPKT.dummy_pkt.WO_config_ID);
   TConf->SetBranchAddress("calib_period", &dummyPKT.dummy_pkt.calib_period);
   TConf->SetBranchAddress("safe_mode", &dummyPKT.dummy_pkt.safe_mode);
   
   return 0;
}

int LEvRec0File::SetTHVpmtPointer(LEvRec0HVpmt &HVpkt) {

  THVpmt->SetBranchAddress("boot_nr", &HVpkt.boot_nr);
  THVpmt->SetBranchAddress("run_id", &HVpkt.run_id);

  THVpmt->SetBranchAddress("HV_pmt_mon[10]", &HVpkt.HV_pmt_mon[0]);
  THVpmt->SetBranchAddress("HV_sil_mon[2]", &HVpkt.HV_sil_mon[0]);

  return 0;

 } 


int LEvRec0File::GetEntry(int iEntry) {
  treeCalib->GetEntry(iEntry);
  return 0;
}

int LEvRec0File::GetTmdEntry(int iEntry) {
  Tmd->GetEntry(iEntry);
  return 0;
}

int LEvRec0File::GetTConfEntry(int iEntry) {
  TConf->GetEntry(iEntry);
  return 0;
}

int LEvRec0File::GetTHVpmtEntry(int iEntry) {
  THVpmt->GetEntry(iEntry);
  return 0;
}


int LEvRec0File::GetEntries() {
  return treeCalib->GetEntries();
}


int LEvRec0File::GetTmdEntries() {
  return Tmd->GetEntries();
}

int LEvRec0File::GetTConfEntries() {
  return TConf->GetEntries();
}

int LEvRec0File::GetTHVpmtEntries() {
  return THVpmt->GetEntries();
}


int LEvRec0File::RunIDtoEntry(unsigned short runid) {
   int ret = -1;
   
   for (int i = 0; i < Tmd->GetEntries(); i++)
      if (RunId[i] == runid)
	 return i;
   return ret;
}


//////////////////////////

void LEvRec0File::Close() {
  if(inputCalib) {
    treeCalib = 0;
    Tmd = 0;
    TConf = 0;
    THVpmt = 0;
    inputCalib->Close();

    inputCalib = 0;

  }
  return;
}

LEvRec0File::~LEvRec0File() {
  if(inputCalib) {
     treeCalib = 0;
     Tmd = 0;
     TConf = 0;
     THVpmt = 0;
    inputCalib->Close();

  }
}
