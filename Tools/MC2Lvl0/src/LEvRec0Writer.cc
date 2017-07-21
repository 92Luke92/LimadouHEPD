#include "LEvRec0Writer.hh"
#include "LEvRec0.hh"



LEvRec0Writer::LEvRec0Writer(std::string filename) {
  Init(filename);
}


void LEvRec0Writer::Init(std::string filename) {
  fFile=TFile::Open(filename.c_str(), "RECREATE");
  fTree=new TTree("T", "T");
  fTmdTree=new TTree("Tmd","Tmd");
  SetAllEventPointers();
  return;
}

void LEvRec0Writer::Close() {
  fFile->Close();

}


void LEvRec0Writer::SetAllEventPointers() {
  SetTEventPointer();
  SetTmdEventPointer();
}


void LEvRec0Writer::SetTEventPointer() {
  fTree->Branch("run_type", &evT.runType);
  fTree->Branch("evT_boot_nr", &evT.boot_nr);
  fTree->Branch("evT_run_id", &evT.run_id);
  fTree->Branch("evTent_index", &evT.event_index);
  fTree->Branch("evTent_length", &evT.event_length);
  fTree->Branch("trigger_index", &evT.trigger_index);
  fTree->Branch("hepd_time", &evT.hepd_time);
  fTree->Branch("pmt_high[64]", evT.pmt_high);
  fTree->Branch("pmt_low[64]", evT.pmt_low);
  fTree->Branch("PMTBoard_trigger_counter", &evT.PMTBoard_trigger_counter);
  fTree->Branch("lost_trigger", &evT.lost_trigger);
  fTree->Branch("rate_meter[9]", evT.rate_meter);
  fTree->Branch("trigger_flag[64]", evT.trigger_flag);
  fTree->Branch("alive_time", &evT.alive_time);
  fTree->Branch("dead_time", &evT.dead_time);
  fTree->Branch("strip[4608]",evT.strip);
  fTree->Branch("boot_nr",&evT.boot_nr);
  fTree->Branch("run_id",&evT.run_id);
  fTree->Branch("run_type",&evT.runType);
  fTree->Branch("run_duration",&evT.run_duration);
  fTree->Branch("orbitZone",&evT.orbitZone);
}


void LEvRec0Writer::SetTmdEventPointer() {

  fTmdTree->Branch("boot_nr",&evT.boot_nr);
  fTmdTree->Branch("run_id",&evT.run_id);
  fTmdTree->Branch("run_type",&evT.runType);
  fTmdTree->Branch("run_duration",&evT.run_duration);
  fTmdTree->Branch("orbitZone",&evT.orbitZone);
  fTmdTree->Branch("silConfiguration",&evTmd.sil_conf.ladder_on[0],
	       "ladder_on[3]/O:ladder_mask[3]/O:plane_HV[2]/s:adj_strip/s"
               ":zero_supp_thrd/s:thrd_CN_high/b:thrd_CN_low/b:calib_event_CN/s"
               ":calib_event_ped/s:calib_event_RMS/s:calib_event_gauss/s"
               ":guass_check/s");
  fTmdTree->Branch("trigger_mask[2]",evTmd.trigger_mask);
  fTmdTree->Branch("easiroc_conf[60]",evTmd.easiroc_conf);
  fTmdTree->Branch("PMT_mask[64]",evTmd.PMT_mask);
  fTmdTree->Branch("HV_mask[12]",evTmd.HV_mask);
  fTmdTree->Branch("HV_value[10]",evTmd.HV_value);
  fTmdTree->Branch("gen_trig_mask[18]",evTmd.gen_trig_mask);
  fTmdTree->Branch("OBDH_info", &evTmd.OBDH_info.sec_OBDH,
	       "sec_OBDH/i:ms_OBDH/s:NU[2]/b" );
  fTmdTree->Branch("GPS_info", &evTmd.GPS_info.sec_GPS,
	       "sec_GPS/i:axis_pos[3]/I:axis_vel[3]/I:dataID/b"
	       ":lon/I:lat/I:NU[3]/b");
  fTmdTree->Branch("AOCC_info", &evTmd.AOCC_info.sec_AOCC,
	       "sec_AOCC/i:us_AOCC/i:x_coord[3]/b:y_coord[3]/b"
	       ":z_coord[3]/b:roll_angle[3]/b:pitch_angle[3]/b"
	       ":yaw_angle[3]/b:roll_angl_vel/s:pitch_angl_vel/s"
	       ":yaw_angl_vel/s:attitude_det/b:magn_torquer/b"
	       ":magn_torquer_pulse[3]/s:q1/i:q2/i:q3/i");
  fTmdTree->Branch("star_sensor_info", &evTmd.star_sensor_info.sec_ss11,
	       "sec_ss11/i:us_ss11/i:att_quat_ss11[4]/i:sec_ss21/i:us_ss21/i"
	       ":att_quat_ss21[4]/i:sec_ss31/i:us_ss31/i:att_quat_ss31[4]/i"
	       ":quat_valID_x1/b:sec_ss12/i:us_ss12/i:att_quat_ss12[4]/i"
	       ":sec_ss22/i:us_ss22/i:att_quat_ss22[4]/i:sec_ss32/i:us_ss32/i"
	       ":att_quat_ss32[4]/i:quat_valID_x2/b:attitude_det/b:NU/b");
  fTmdTree->Branch("PMT_rate_meter[65]",evTmd.PMT_rate_meter);
  fTmdTree->Branch("CPU_temp_start_stop_Run[2]",evTmd.CPU_temp_start_stop_Run);
  fTmdTree->Branch("PMT_temp_start_stop_Run[2]",evTmd.PMT_temp_start_stop_Run);
  fTmdTree->Branch("CPU_time_start_stop_Run[2]",evTmd.CPU_time_start_stop_Run);
  fTmdTree->Branch("CPU_timestamp", &evTmd.CPU_timestamp.OBDH,
	       "OBDH/i:GPS/i:AOCC/i:star_sensor/i");
  fTmdTree->Branch("status_register", &evTmd.status_register.statusDAQ,
	       "statusDAQ/s:statusPMT/s:statusTM_TC/s:statusHV_PS/s"
	       ":CPU_board_boot/s:statusCPU/s");

}


LEvRec0Writer::~LEvRec0Writer() {
}
