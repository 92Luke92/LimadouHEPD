#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvRec0.hh"
#include <iostream>

#pragma pack(push)
#pragma pack(1)

LEvRec0::LEvRec0(){
   for(int i=0; i<NCHAN; ++i)
      strip[i]=0;
   
   runType = 0x0;
   trigger_index=0;
   hepd_time=0;
   event_index=0;
   event_length=0;
   for(int i=0; i<NPMT; ++i) {
      pmt_high[i]=0;
      pmt_low[i]=0;
      trigger_flag[i]=false;
   }
   for(int i=0; i<NRATEMETER; ++i)
      rate_meter[i]=0;
   alive_time=0;
   dead_time=0;

}



void LEvRec0::DumpStrip(void) {
  std::cout << "strip" << std::endl;
  for(int i=0; i<NCHAN;++i) std::cout << strip[i] << " ";
  std::cout << std::endl;
  return;
}


void LEvRec0::DumpEventIndex() {
  std::cout << "event_index " << event_index << std::endl;
  return;
}

bool  LEvRec0::IsComp(void) {
   int ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x36 || runType == 0x6336)
      ret = true;
   
   return ret;
   
}

bool  LEvRec0::IsVirgin(void) {
   bool ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x4e ||runType == 0x634e )
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

LEvRec0Conf::LEvRec0Conf(){
  
  for(int i=0; i<3; ++i)
    dummy_pkt.orbit_conf[0].ladder_mask[i]=false;
  for(int i=0; i<2; ++i)
    dummy_pkt.orbit_conf[0].trigger_mask[i]=0;
  
  dummy_pkt.orbit_conf[0].run_duration=0;
  dummy_pkt.orbit_conf[0].lat1=0;
  dummy_pkt.orbit_conf[0].lat2=0;
  dummy_pkt.orbit_conf[0].long1=0;
  dummy_pkt.orbit_conf[0].long2=0;

  for(int i=0; i<3; ++i)
    dummy_pkt.orbit_conf[1].ladder_mask[i]=false;
  for(int i=0; i<2; ++i)
    dummy_pkt.orbit_conf[1].trigger_mask[i]=0;
  
  dummy_pkt.orbit_conf[1].run_duration=0;
  dummy_pkt.orbit_conf[1].lat1=0;
  dummy_pkt.orbit_conf[1].lat2=0;
  dummy_pkt.orbit_conf[1].long1=0;
  dummy_pkt.orbit_conf[1].long2=0;

  for(int i=0; i<3; ++i)
    dummy_pkt.orbit_conf[2].ladder_mask[i]=false;
  for(int i=0; i<2; ++i)
    dummy_pkt.orbit_conf[2].trigger_mask[i]=0;
  dummy_pkt.orbit_conf[2].run_duration=0;
  dummy_pkt.orbit_conf[2].lat1=0;
  dummy_pkt.orbit_conf[2].lat2=0;
  dummy_pkt.orbit_conf[2].long1=0;
  dummy_pkt.orbit_conf[2].long2=0;

  for(int i=0; i<3; ++i)
    dummy_pkt.orbit_conf[3].ladder_mask[i]=false;
  for(int i=0; i<2; ++i)
    dummy_pkt.orbit_conf[3].trigger_mask[i]=0;
  
  dummy_pkt.orbit_conf[3].run_duration=0;
  dummy_pkt.orbit_conf[3].lat1=0;
  dummy_pkt.orbit_conf[3].lat2=0;
  dummy_pkt.orbit_conf[3].long1=0;
  dummy_pkt.orbit_conf[3].long2=0;

  for(int i=0; i<3; ++i)
    dummy_pkt.orbit_conf[4].ladder_mask[i]=false;
  for(int i=0; i<2; ++i)
    dummy_pkt.orbit_conf[4].trigger_mask[i]=0;
  
  dummy_pkt.orbit_conf[4].run_duration=0;
  dummy_pkt.orbit_conf[4].lat1=0;
  dummy_pkt.orbit_conf[4].lat2=0;
  dummy_pkt.orbit_conf[4].long1=0;
  dummy_pkt.orbit_conf[4].long2=0;

  dummy_pkt.boot_nr = 0;
  dummy_pkt.run_id = 0;
  dummy_pkt.user_orbital_settings=0;
  dummy_pkt.WO_config_ID=0;
  dummy_pkt.calib_period=0;
  dummy_pkt.safe_mode=0;

  }

LEvRec0HVpmt::LEvRec0HVpmt(){

  boot_nr = 0;
  run_id = 0;

  for(int i=0; i<10; ++i)
    HV_pmt_mon[i] = 0;

  for(int l=0; l<2; ++l)
    HV_sil_mon[l] = 0;


}
 
  

  

  
    



