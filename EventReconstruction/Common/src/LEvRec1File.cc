#include "LEvRec1File.hh"
#include "TObject.h"
#include <string>
#include <iostream>


LEvRec1File::LEvRec1File(const char *inpFile, const char *openMode="READ") {
  WritableFLAG=false; // safety
  metaDataFlag=true;
  
  evptr= 0;
  
  std::string omstr(openMode);
  if((omstr.compare("READ"))==0) {                // READ MODE
    fFile=TFile::Open(inpFile, "READ");
    fTree=(TTree*)fFile->Get("L1");
    if(fFile->GetListOfKeys()->Contains("L1md")) // for backward compatibility
       fTreeMD=(TTree*)fFile->Get("L1md");
    else
    {
       std::cout << "Old L1 File without metaData Tree "<< std::endl;
       metaDataFlag=false;
    }
    
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

void LEvRec1File::CreateTrees() {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to create trees for a file not open or open in Read MODE." << std::endl;
    return;
  }

  fFile->cd();
  fTreeMD = new TTree("L1md", "Limadou Level 1 meta data"); 
  fTreeMD->SetDirectory(fFile);
  fTree = new TTree("L1", "Limadou Level 1 events");
  fTree->SetDirectory(fFile);
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
  trigger_trigger_flagBR = fTree->Branch("trigger_trigger_flag[12]", 0, "trigger_trigger_flag[12]/O");
  trigger_is_saturatedBR = fTree->Branch("trigger_is_saturated[12]", 0, "trigger_is_saturated[12]/O");
  scint_countHGBR = fTree->Branch("scint_countHG[32]", 0, "scint_countHG[32]/D");
  scint_snHGBR = fTree->Branch("scint_snHG[32]", 0, "scint_snHG[32]/D");
  scint_countLGBR = fTree->Branch("scint_countLG[32]", 0, "scint_countLG[32]/D");
  scint_snLGBR = fTree->Branch("scint_snLG[32]", 0, "scint_snLG[32]/D");
  scint_trigger_flagBR = fTree->Branch("scint_trigger_flag[32]", 0, "scint_trigger_flag[32]/O");
  scint_is_saturatedBR = fTree->Branch("scint_is_saturated[32]", 0, "scint_is_saturated[32]/O");
  veto_countHGBR = fTree->Branch("veto_countHG[10]", 0, "veto_countHG[10]/D");
  veto_snHGBR = fTree->Branch("veto_snHG[10]", 0, "veto_snHG[10]/D");
  veto_countLGBR = fTree->Branch("veto_countLG[10]", 0, "veto_countLG[10]/D");
  veto_snLGBR = fTree->Branch("veto_snLG[10]", 0, "veto_snLG[10]/D");
  veto_trigger_flagBR = fTree->Branch("veto_trigger_flag[10]", 0, "veto_trigger_flag[10]/O");
  veto_is_saturatedBR = fTree->Branch("veto_is_saturated[10]", 0, "veto_is_saturated[10]/O");
  lyso_countHGBR = fTree->Branch("lyso_countHG[9]", 0, "lyso_countHG[9]/D");
  lyso_snHGBR = fTree->Branch("lyso_snHG[9]", 0, "lyso_snHG[9]/D");
  lyso_countLGBR = fTree->Branch("lyso_countLG[9]", 0, "lyso_countLG[9]/D");
  lyso_snLGBR = fTree->Branch("lyso_snLG[9]", 0, "lyso_snLG[9]/D");
  lyso_trigger_flagBR = fTree->Branch("lyso_trigger_flag[9]", 0, "lyso_trigger_flag[9]/O");
  lyso_is_saturatedBR = fTree->Branch("lyso_is_saturated[9]", 0, "lyso_is_saturated[9]/O");

  
  runTypeBR = fTree->Branch("runType", 0, "runType/s");
  boot_nrBR = fTree->Branch("boot_nr", 0, "boot_nr/s");
  run_idBR = fTree->Branch("run_id", 0, "run_id/s");
  event_indexBR = fTree->Branch("event_index", 0, "event_index/i");
  event_lengthBR = fTree->Branch("event_length", 0, "event_length/s");
  trigger_indexBR = fTree->Branch("trigger_index", 0, "trigger_index/i");
  hepd_timeBR = fTree->Branch("hepd_time", 0, "hepd_time/i");
  PMTBoard_trigger_counterBR = fTree->Branch("PMTBoard_trigger_counter", 0, "PMTBoard_trigger_counter/i");
  lost_triggerBR = fTree->Branch("lost_trigger", 0, "lost_trigger/s");
  rate_meterBR = fTree->Branch("rate_meter[9]", 0, "rate_meter[9]/s");
  alive_timeBR = fTree->Branch("alive_time", 0, "alive_time/i");
  dead_timeBR = fTree->Branch("dead_time", 0, "dead_time/i");

  // orbital info
  // if(ORBITAL_INFO)
  // {
  //    rigBR = fTree->Branch("rig", 0, "rig/D");
  //    abstimeBR = fTree->Branch("abstime", 0, "abstime/i");
  //    BBR = fTree->Branch("B", 0, "B/D");
  //    latBR = fTree->Branch("lat", 0, "lat/D");  
  //    lonBR = fTree->Branch("lon", 0, "lon/D");
  //    altBR = fTree->Branch("alt", 0, "alt/D");
  //    LBR = fTree->Branch("L", 0, "L/D");
  //    B_xBR = fTree->Branch("B_x", 0, "B_x/D");
  //    B_yBR = fTree->Branch("B_y", 0, "B_y/D");
  //    B_zBR = fTree->Branch("B_z", 0, "B_z/D");
  // }
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
  trigger_trigger_flagBR = fTree->GetBranch("trigger_trigger_flag[12]");
  trigger_is_saturatedBR = fTree->GetBranch("trigger_is_saturated[12]");
  scint_countHGBR = fTree->GetBranch("scint_countHG[32]");
  scint_snHGBR = fTree->GetBranch("scint_snHG[32]");
  scint_countLGBR = fTree->GetBranch("scint_countLG[32]");
  scint_snLGBR = fTree->GetBranch("scint_snLG[32]");
  scint_trigger_flagBR = fTree->GetBranch("scint_trigger_flag[32]");
  scint_is_saturatedBR = fTree->GetBranch("scint_is_saturated[32]");
  veto_countHGBR = fTree->GetBranch("veto_countHG[10]");
  veto_snHGBR = fTree->GetBranch("veto_snHG[10]");
  veto_countLGBR = fTree->GetBranch("veto_countLG[10]");
  veto_snLGBR = fTree->GetBranch("veto_snLG[10]");
  veto_trigger_flagBR = fTree->GetBranch("veto_trigger_flag[10]");
  veto_is_saturatedBR = fTree->GetBranch("veto_is_saturated[10]");
  lyso_countHGBR = fTree->GetBranch("lyso_countHG[9]");
  lyso_snHGBR = fTree->GetBranch("lyso_snHG[9]");
  lyso_countLGBR = fTree->GetBranch("lyso_countLG[9]");
  lyso_snLGBR = fTree->GetBranch("lyso_snLG[9]");
  lyso_trigger_flagBR = fTree->GetBranch("lyso_trigger_flag[9]");
  lyso_is_saturatedBR = fTree->GetBranch("lyso_is_saturated[9]");

  runTypeBR = fTree->GetBranch("runType");
  boot_nrBR = fTree->GetBranch("boot_nr");
  run_idBR = fTree->GetBranch("run_id");
  event_indexBR = fTree->GetBranch("event_index");
  event_lengthBR = fTree->GetBranch("event_length");
  trigger_indexBR = fTree->GetBranch("trigger_index");
  hepd_timeBR = fTree->GetBranch("hepd_time");
  PMTBoard_trigger_counterBR = fTree->GetBranch("PMTBoard_trigger_counter");
  lost_triggerBR = fTree->GetBranch("lost_trigger");
  rate_meterBR = fTree->GetBranch("rate_meter[9]");
  alive_timeBR = fTree->GetBranch("alive_time");
  dead_timeBR = fTree->GetBranch("dead_time");

  // orbital info
  // if(ORBITAL_INFO)
  // {
  //    rigBR = fTree->GetBranch("rig");
  //    abstimeBR = fTree->GetBranch("abstime");
  //    BBR = fTree->GetBranch("B");
  //    latBR = fTree->GetBranch("lat");  
  //    lonBR = fTree->GetBranch("lon");
  //    altBR = fTree->GetBranch("alt");
  //    LBR = fTree->GetBranch("L");
  //    B_xBR = fTree->GetBranch("B_x");
  //    B_yBR = fTree->GetBranch("B_y");
  //    B_zBR = fTree->GetBranch("B_z");
  // }
  // MetaData Tree
  if (metaDataFlag)
  {
     run_idMDBR = fTreeMD->GetBranch("run_id");
     boot_nrMDBR = fTreeMD->GetBranch("boot_nr");
     run_typeMDBR = fTreeMD->GetBranch("run_type");
     run_durationBR = fTreeMD->GetBranch("run_duration");
     orbitZoneBR = fTreeMD->GetBranch("orbitZone");
     silConfigurationBR = fTreeMD->GetBranch("silConfiguration");
     trigger_maskBR = fTreeMD->GetBranch("trigger_mask[2]");
     easiroc_confBR = fTreeMD->GetBranch("easiroc_conf[60]");
     PMT_maskBR = fTreeMD->GetBranch("PMT_mask[64]");
     HV_maskBR = fTreeMD->GetBranch("HV_mask[12]");
     HV_valueBR = fTreeMD->GetBranch("HV_value[10]");
     gen_trig_maskBR = fTreeMD->GetBranch("gen_trig_mask[18]");
     OBDH_infoBR = fTreeMD->GetBranch("OBDH_info");
     GPS_infoBR = fTreeMD->GetBranch("GPS_info");
     AOCC_infoBR = fTreeMD->GetBranch("AOCC_info");
     star_sensor_infoBR = fTreeMD->GetBranch("star_sensor_info");
     PMT_rate_meterBR = fTreeMD->GetBranch("PMT_rate_meter[65]");
     CPU_tempBR = fTreeMD->GetBranch("CPU_temp_start_stop_Run[2]");
     PMT_tempBR = fTreeMD->GetBranch("PMT_temp_start_stop_Run[2]");
     CPU_timeBR = fTreeMD->GetBranch("CPU_time_start_stop_Run[2]");
     CPU_timestampBR = fTreeMD->GetBranch("CPU_timestamp");
     status_registerBR = fTreeMD->GetBranch("status_register");
  }

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
  trigger_trigger_flagBR->SetAddress(&(evstr.trigger_trigger_flag[0]));
  trigger_is_saturatedBR->SetAddress(&(evstr.trigger_is_saturated[0]));
  scint_countHGBR->SetAddress(&(evstr.scint_countHG[0]));
  scint_snHGBR->SetAddress(&(evstr.scint_snHG[0]));
  scint_countLGBR->SetAddress(&(evstr.scint_countLG[0]));
  scint_snLGBR->SetAddress(&(evstr.scint_snLG[0]));
  scint_trigger_flagBR->SetAddress(&(evstr.scint_trigger_flag[0]));
  scint_is_saturatedBR->SetAddress(&(evstr.scint_is_saturated[0]));
  veto_countHGBR->SetAddress(&(evstr.veto_countHG[0]));
  veto_snHGBR->SetAddress(&(evstr.veto_snHG[0]));
  veto_countLGBR->SetAddress(&(evstr.veto_countLG[0]));
  veto_snLGBR->SetAddress(&(evstr.veto_snLG[0]));
  veto_trigger_flagBR->SetAddress(&(evstr.veto_trigger_flag[0]));
  veto_is_saturatedBR->SetAddress(&(evstr.veto_is_saturated[0]));
  lyso_countHGBR->SetAddress(&(evstr.lyso_countHG[0]));
  lyso_snHGBR->SetAddress(&(evstr.lyso_snHG[0]));
  lyso_countLGBR->SetAddress(&(evstr.lyso_countLG[0]));
  lyso_snLGBR->SetAddress(&(evstr.lyso_snLG[0]));
  lyso_trigger_flagBR->SetAddress(&(evstr.lyso_trigger_flag[0]));
  lyso_is_saturatedBR->SetAddress(&(evstr.lyso_is_saturated[0]));

  runTypeBR->SetAddress(&(evstr.runType));
  boot_nrBR->SetAddress(&(evstr.boot_nr));
  run_idBR->SetAddress(&(evstr.run_id));
  event_indexBR->SetAddress(&(evstr.event_index));
  event_lengthBR->SetAddress(&(evstr.event_length));
  trigger_indexBR->SetAddress(&(evstr.trigger_index));
  hepd_timeBR->SetAddress(&(evstr.hepd_time));
  PMTBoard_trigger_counterBR->SetAddress(&(evstr.PMTBoard_trigger_counter));
  lost_triggerBR->SetAddress(&(evstr.lost_trigger));
  rate_meterBR->SetAddress(&(evstr.rate_meter[0]));
  alive_timeBR->SetAddress(&(evstr.alive_time));
  dead_timeBR->SetAddress(&(evstr.dead_time));

  // if (ORBITAL_INFO)
  // {
  //    rigBR->SetAddress(&(evstr.rig));
  //    abstimeBR->SetAddress(&(evstr.abstime));
  //    BBR->SetAddress(&(evstr.B));
  //    latBR->SetAddress(&(evstr.lat));
  //    lonBR->SetAddress(&(evstr.lon));
  //    altBR->SetAddress(&(evstr.alt));
  //    LBR->SetAddress(&(evstr.L));
  //    B_xBR->SetAddress(&(evstr.B_x));
  //    B_yBR->SetAddress(&(evstr.B_y));
  //    B_zBR->SetAddress(&(evstr.B_z));
  // }

  return;  
}

void LEvRec1File::SetMDTreeAddress(LEvRec1 &l1ev) {
   
   fTreeMD->Branch("boot_nr", &l1ev.lev0MD.boot_nr, "boot_nr/s");
   fTreeMD->Branch("run_id", &l1ev.lev0MD.run_id);
   fTreeMD->Branch("run_type", &l1ev.lev0MD.run_type);
   fTreeMD->Branch("run_duration", &l1ev.lev0MD.run_duration);
   fTreeMD->Branch("orbitZone", &l1ev.lev0MD.orbit_Zone);

   fTreeMD->Branch("silConfiguration", &l1ev.lev0MD.silConfig.ladder_on[0],
   		"ladder_on[3]/O:ladder_mask[3]/O:plane_HV[2]/s:adj_strip/s"
   		":zero_supp_thrd/s:thrd_CN_HIGH/b:thrd_CN_LOW/b:calib_event_CN/s"
   		":calib_event_ped/s:calib_event_RMS/s:calib_event_gauss/s:gauss_check/s");
   
   fTreeMD->Branch("trigger_mask[2]", &l1ev.lev0MD.trigger_mask[0], "trigger_mask[2]/b");
   fTreeMD->Branch("easiroc_conf[60]", &l1ev.lev0MD.easiroc_config[0]); 
   fTreeMD->Branch("PMT_mask[64]", &l1ev.lev0MD.PMT_mask[0]);
   fTreeMD->Branch("HV_mask[12]", &l1ev.lev0MD.HV_mask[0]);
   fTreeMD->Branch("HV_value[10]", &l1ev.lev0MD.HV_value[0]);
   fTreeMD->Branch("gen_trig_mask[18]", &l1ev.lev0MD.gen_trig_mask[0]);

   // broadcasta data
   fTreeMD->Branch("OBDH_info", &l1ev.lev0MD.broadcast.OBDH.sec,
   		   "sec_OBDH/i:ms_OBDH/s:NU[2]/b" );
   fTreeMD->Branch("GPS_info", &l1ev.lev0MD.broadcast.GPS.sec,
   		   "sec_GPS/i:axis_pos[3]/I:axis_vel[3]/I:dataID/b"
   		   ":lon/I:lat/I:NU[3]/b");
   fTreeMD->Branch("AOCC_info", &l1ev.lev0MD.broadcast.AOCC.sec,
   		   "sec_AOCC/i:us_AOCC/i:x_coord[3]/b:y_coord[3]/b"
   		   ":z_coord[3]/b:roll_angle[3]/b:pitch_angle[3]/b"
   		   ":yaw_angle[3]/b:roll_angl_vel/s:pitch_angl_vel/s"
   		   ":yaw_angl_vel/s:attitude_det/b:magn_torquer/b"
   		   ":magn_torquer_pulse[3]/s:q1/i:q2/i:q3/i");
   fTreeMD->Branch("star_sensor_info", &l1ev.lev0MD.broadcast.star_sensor.sec_ss11,
   		   "sec_ss11/i:us_ss11/i:att_quat_ss11[4]/i:sec_ss21/i:us_ss21/i"
   		   ":att_quat_ss21[4]/i:sec_ss31/i:us_ss31/i:att_quat_ss31[4]/i"
   		   ":quat_valID_x1/b:sec_ss12/i:us_ss12/i:att_quat_ss12[4]/i"
   		   ":sec_ss22/i:us_ss22/i:att_quat_ss22[4]/i:sec_ss32/i:us_ss32/i"
   		   ":att_quat_ss32[4]/i:quat_valID_x2/b:attitude_det/b:NU/b");
   
   // scinentific packet
   fTreeMD->Branch("PMT_rate_meter[65]", &l1ev.lev0MD.PMT_rate_meter[0]);
   fTreeMD->Branch("CPU_temp_start_stop_Run[2]", &l1ev.lev0MD.CPU_temp[0]);
   fTreeMD->Branch("PMT_temp_start_stop_Run[2]", &l1ev.lev0MD.PMT_temp[0]);
   fTreeMD->Branch("CPU_time_start_stop_Run[2]", &l1ev.lev0MD.CPU_time[0]);
   
   // timestamp CPU
   fTreeMD->Branch("CPU_timestamp", &l1ev.lev0MD.timestamp.OBDH,
   		   "OBDH/i:GPS/i:AOCC/i:star_sensor/i");
   
   // board status
   fTreeMD->Branch("status_register", &l1ev.lev0MD.status_reg.statusDAQ,
   		   "statusDAQ/s:statusPMT/s:statusTM_TC/s:statusHV_PS/s"
   		   ":CPU_board_boot/s:statusCPU/s");
   
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

void LEvRec1File::FillMD() {
  if(!(WritableFLAG && fFile)) {
    std::cerr << "Trying to fill a file not open or open in Read MODE." << std::endl;
    return;
  }
  
  fTreeMD->Fill();
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

  if(metaDataFlag)
     SetMDPointer(ev);
     
  return 0;
}



int LEvRec1File::SetMDPointer(LEvRec1 &ev) {
  if(WritableFLAG) {
    std::cerr << "Trying to address a file open in Write MODE." << std::endl;
    return -999;
  }

  fTreeMD->SetBranchAddress("boot_nr", &ev.lev0MD.boot_nr);
  fTreeMD->SetBranchAddress("run_id", &ev.lev0MD.run_id);

  fTreeMD->SetBranchAddress("run_type", &ev.lev0MD.run_type);
  fTreeMD->SetBranchAddress("run_duration", &ev.lev0MD.run_duration);
  fTreeMD->SetBranchAddress("orbitZone", &ev.lev0MD.orbit_Zone);
   
  fTreeMD->SetBranchAddress("silConfiguration", &ev.lev0MD.silConfig.ladder_on);

  fTreeMD->SetBranchAddress("trigger_mask[2]", &ev.lev0MD.trigger_mask[0]);
  fTreeMD->SetBranchAddress("easiroc_conf[60]", &ev.lev0MD.easiroc_config[0]);
  fTreeMD->SetBranchAddress("PMT_mask[64]", &ev.lev0MD.PMT_mask[0]);
  fTreeMD->SetBranchAddress("HV_mask[12]", &ev.lev0MD.HV_mask[0]);
  fTreeMD->SetBranchAddress("HV_value[10]", &ev.lev0MD.HV_value[0]);
  fTreeMD->SetBranchAddress("gen_trig_mask[18]", &ev.lev0MD.gen_trig_mask[0]);

  // broadcasta data
  fTreeMD->SetBranchAddress("OBDH_info", &ev.lev0MD.broadcast.OBDH.sec);
  fTreeMD->SetBranchAddress("GPS_info", &ev.lev0MD.broadcast.GPS.sec);
  fTreeMD->SetBranchAddress("AOCC_info", &ev.lev0MD.broadcast.AOCC.sec);
  fTreeMD->SetBranchAddress("star_sensor_info", &ev.lev0MD.broadcast.star_sensor.sec_ss11);

  // scientific packet
  fTreeMD->SetBranchAddress("PMT_rate_meter[65]", &ev.lev0MD.PMT_rate_meter[0]);
  fTreeMD->SetBranchAddress("CPU_temp_start_stop_Run[2]", &ev.lev0MD.CPU_temp[0]);
  fTreeMD->SetBranchAddress("PMT_temp_start_stop_Run[2]", &ev.lev0MD.PMT_temp[0]);
  fTreeMD->SetBranchAddress("CPU_time_start_stop_Run[2]", &ev.lev0MD.CPU_time[0]);

  // timestamp CPU 
  fTreeMD->SetBranchAddress("CPU_timestamp", &ev.lev0MD.timestamp.OBDH);

  // board status 
  fTreeMD->SetBranchAddress("status_register", &ev.lev0MD.status_reg.statusDAQ);

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


int LEvRec1File::GetMDEntry(int iEntry) {
   if(WritableFLAG) {
      std::cerr << "Trying to get the entry of a file open in Write MODE." << std::endl;
      return -999;
   }

   fTreeMD->GetEntry(iEntry);

   return 0;
}


int LEvRec1File::GetMDEntries() {
  return fTreeMD->GetEntries();
}

void LEvRec1File::Write() {
  if(fFile && WritableFLAG) fFile->Write(0, TObject::kOverwrite);
  return;
}

void LEvRec1File::Close() {
  if(fFile) {
    if(fTree) {
      if(WritableFLAG==false) fTree=0;
      else delete fTree;
    }
    if(fTreeMD) {
       if(WritableFLAG==false) fTreeMD=0;
       else delete fTreeMD;
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
    if(fTreeMD) {
       if(WritableFLAG==false) fTreeMD=0;
       else delete fTreeMD;
    }
    fFile->Close();
  }
}
