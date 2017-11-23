#include "LEvRec0File.hh"
#include <iostream>
#include <sstream>

LEvRec0File::LEvRec0File() {
  fFile=0;
  fTree=0;
  Tmd=0;
  RunId=-999;
  BootNr=-999;
  runType=0;
}

LEvRec0File::LEvRec0File(const char *inpFile) {
  Open(inpFile);
}

LEvRec0File::LEvRec0File(const char *outFile, LEvRec0 &event, LEvRec0Md &metadata) {

   std::cout << __LEVREC0FILE__ << "Opening Level 0 file in writing mode"  << std::endl;

   fFile = new TFile(outFile, "recreate");
   fTree = new TTree("T","Data");
   Tmd   = new TTree("Tmd","Config");

   // Event Tree
   // event header
   fTree->Branch("run_type", &event.runType, "run_type/s");
   fTree->Branch("ev_boot_nr", &event.boot_nr, "ev_boot_nr/s");
   fTree->Branch("ev_run_id", &event.run_id, "ev_run_id/s");
   fTree->Branch("event_index", &event.event_index, "event_index/i");     
   fTree->Branch("event_length", &event.event_length, "event_length/s");
   fTree->Branch("trigger_index", &event.trigger_index, "trigger_index/i");
   fTree->Branch("hepd_time", &event.hepd_time, "hepd_time/i");

   // trigger board data
   fTree->Branch("pmt_high[64]", &event.pmt_high[0], "pmt_high[64]/s"); 
   fTree->Branch("pmt_low[64]", &event.pmt_low[0], "pmt_low[64]/s");
   fTree->Branch("PMTBoard_trigger_counter", &event.PMTBoard_trigger_counter,
	     "PMTBoard_trigger_counter/i");
   fTree->Branch("lost_trigger", &event.lost_trigger, "lost_trigger/s");
   fTree->Branch("rate_meter[9]", &event.rate_meter[0], "rate_meter[9]/s");  
   fTree->Branch("trigger_flag[64]", &event.trigger_flag[0], "trigger_flag[64]/O");
   fTree->Branch("alive_time", &event.alive_time, "alive_time/i");
   fTree->Branch("dead_time", &event.dead_time, "dead_time/i");

   // silicon data
   fTree->Branch("strip[4608]", &event.strip[0], "strip[4608]/s");


   // meta data Tree
   Tmd->Branch("boot_nr", &metadata.boot_nr, "boot_nr/s");
   Tmd->Branch("run_id", &metadata.run_id);
   Tmd->Branch("run_type", &metadata.run_type);
   Tmd->Branch("run_duration", &metadata.run_duration);
   Tmd->Branch("orbitZone", &metadata.orbit_Zone);

   Tmd->Branch("silConfiguration", &metadata.silConfig.ladder_on[0],
   	       "ladder_on[3]/O:ladder_mask[3]/O:plane_HV[2]/s:adj_strip/s"
   	       ":zero_supp_thrd/s:thrd_CN_HIGH/b:thrd_CN_LOW/b:calib_event_CN/s"
   	       ":calib_event_ped/s:calib_event_RMS/s:calib_event_gauss/s:gauss_check/s");
   
   Tmd->Branch("trigger_mask[2]", &metadata.trigger_mask[0], "trigger_mask[2]/b");
   Tmd->Branch("easiroc_conf[60]", &metadata.easiroc_config[0]); 
   Tmd->Branch("PMT_mask[64]", &metadata.PMT_mask[0]);
   Tmd->Branch("HV_mask[12]", &metadata.HV_mask[0]);
   Tmd->Branch("HV_value[10]", &metadata.HV_value[0]);
   Tmd->Branch("gen_trig_mask[18]", &metadata.gen_trig_mask[0]);

   // broadcasta data
   Tmd->Branch("OBDH_info", &metadata.broadcast.OBDH.sec,
   	       "sec_OBDH/i:ms_OBDH/s:NU[2]/b" );
   Tmd->Branch("GPS_info", &metadata.broadcast.GPS.sec,
   	       "sec_GPS/i:axis_pos[3]/I:axis_vel[3]/I:dataID/b"
   	       ":lon/I:lat/I:NU[3]/b");
   Tmd->Branch("AOCC_info", &metadata.broadcast.AOCC.sec,
   	       "sec_AOCC/i:us_AOCC/i:x_coord[3]/b:y_coord[3]/b"
   	       ":z_coord[3]/b:roll_angle[3]/b:pitch_angle[3]/b"
   	       ":yaw_angle[3]/b:roll_angl_vel/s:pitch_angl_vel/s"
   	       ":yaw_angl_vel/s:attitude_det/b:magn_torquer/b"
   	       ":magn_torquer_pulse[3]/s:q1/i:q2/i:q3/i");
   Tmd->Branch("star_sensor_info", &metadata.broadcast.star_sensor.sec_ss11,
   	       "sec_ss11/i:us_ss11/i:att_quat_ss11[4]/i:sec_ss21/i:us_ss21/i"
   	       ":att_quat_ss21[4]/i:sec_ss31/i:us_ss31/i:att_quat_ss31[4]/i"
   	       ":quat_valID_x1/b:sec_ss12/i:us_ss12/i:att_quat_ss12[4]/i"
   	       ":sec_ss22/i:us_ss22/i:att_quat_ss22[4]/i:sec_ss32/i:us_ss32/i"
   	       ":att_quat_ss32[4]/i:quat_valID_x2/b:attitude_det/b:NU/b");
   
   // scinentific packet
   Tmd->Branch("PMT_rate_meter[65]", &metadata.PMT_rate_meter[0]);
   Tmd->Branch("CPU_temp_start_stop_Run[2]", &metadata.CPU_temp[0]);
   Tmd->Branch("PMT_temp_start_stop_Run[2]", &metadata.PMT_temp[0]);
   Tmd->Branch("CPU_time_start_stop_Run[2]", &metadata.CPU_time[0]);
   
   // timestamp CPU
   Tmd->Branch("CPU_timestamp", &metadata.timestamp.OBDH,
   	       "OBDH/i:GPS/i:AOCC/i:star_sensor/i");
   
   // board status
   Tmd->Branch("status_register", &metadata.status_reg.statusDAQ,
   	       "statusDAQ/s:statusPMT/s:statusTM_TC/s:statusHV_PS/s"
   	       ":CPU_board_boot/s:statusCPU/s");
}


void LEvRec0File::Open(const char *inpFile) {
  fFile=TFile::Open(inpFile, "READ");
  //calibration run processing...
  fTree=(TTree*)fFile->Get("T");
  Tmd = (TTree*)fFile->Get("Tmd");
  unsigned short run_id;
  Tmd->SetBranchAddress("run_id",&run_id);
  unsigned short boot_nr;
  Tmd->SetBranchAddress("boot_nr",&boot_nr);
  unsigned short run_type; 
  Tmd->SetBranchAddress("run_type",&run_type);
  Tmd->GetEntry(0);
  RunId = static_cast<int>(run_id);
  BootNr = static_cast<int>(boot_nr);
  runType = run_type;
  
  return;
}

void LEvRec0File::Reset() {
  if(fFile) {
    fTree=0;
    fFile->Close();
  }
  RunId=-999;
  BootNr=-999;
  runType=0;

  return;
}


void LEvRec0File::Fill() {

   fTree->Fill();
   Tmd->Fill();
   
   return;
}

void LEvRec0File::Write() {

   fFile->cd();
   fFile->Write();

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

int LEvRec0File::SetMdPointer(LEvRec0Md &metaData) {
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

   return 0;
}

int LEvRec0File::GetEntry(int iEntry) {
  fTree->GetEntry(iEntry);
  return 0;
}


int LEvRec0File::GetMDEntry(int iEntry) {
   Tmd->GetEntry(iEntry);
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
