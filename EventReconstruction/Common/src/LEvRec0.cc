#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvRec0.hh"
#include <iostream>

LEvRec0::LEvRec0(){
  for(int i=0; i<NCHAN; ++i) strip[i]=0;
  
  runType = 0x0;
  boot_nr=0;
  run_id=0;
  event_index=0;
  event_length=0;
  trigger_index=0;
  hepd_time=0;
  lost_trigger=0;
  
  for(int i=0; i<NPMT; ++i) {
    pmt_high[i]=0;
    pmt_low[i]=0;
    trigger_flag[i]=false;
  }
  
  PMTBoard_trigger_counter = 0;
  
  for(int i=0; i<NRATEMETER; ++i) rate_meter[i]=0;
  alive_time=0;
  dead_time=0;
}



void LEvRec0::DumpStrip(void) const {
  std::cout << "strip" << std::endl;
  for(int i=0; i<NCHAN;++i) std::cout << strip[i] << " ";
  std::cout << std::endl;
  return;
}


void LEvRec0::DumpEventIndex() const{
  std::cout << "event_index " << event_index << std::endl;
  return;
}


bool  LEvRec0::IsZeroSuppressed(void) const {
   bool ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x6336 )
      ret = true;
   
   return ret;   
}

bool  LEvRec0::IsVirgin(void) const {
   bool ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x634e)
      ret = true;
   
   return ret;
}


LEvRec0Md::LEvRec0Md(){
   boot_nr = 0;
   run_id = 0;
   run_type = 0;
   run_duration = 0;
   orbit_Zone = 0;
   for(int i=0; i<3; ++i)
      silConfig.ladder_on[i] = false;
   for(int i=0; i<3; ++i)
      silConfig.ladder_mask[i] = false; 
   silConfig.plane_HV[0] = 0;
   silConfig.plane_HV[1] = 0;
   silConfig.adj_strip = 0;
   silConfig.zero_supp_thrd = 0; 
   silConfig.thrd_CN_HIGH = 0;
   silConfig.thrd_CN_LOW = 0;
   silConfig.calib_event_CN = 0;
   silConfig.calib_event_ped = 0;
   silConfig.calib_event_RMS = 0;
   silConfig.calib_event_gauss = 0;
   silConfig.gauss_check = 0;
   for(int i=0; i<2; ++i)
      trigger_mask[i] = 0; 
   for(int i=0; i<120; ++i)
      easiroc_config[i] = 0;
   for(int i=0; i<2; ++i)
      PMT_mask[i] = 0;    
   for(int i=0; i<12; ++i)
      HV_mask[i] = false;
   for(int i=0; i<10; ++i)
      HV_value[i] = 0; 
   for(int i=0; i<4; ++i)
      gen_trig_mask[i] = 0;

   broadcast.GPS.sec = 0;
   for(int i=0; i<3; ++i)
      broadcast.GPS.axis_pos[i] = 0;
   for(int i=0; i<3; ++i)
      broadcast.GPS.axis_vel[i] = 0;
   broadcast.GPS.dataID = 0;
   broadcast.GPS.lon = 0;
   broadcast.GPS.lat = 0;
   for(int i=0; i<3; ++i)
      broadcast.GPS.NU[i] = 0; 
      
}
