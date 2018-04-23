#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvRec0.hh"
#include <iostream>

LEvRec0::LEvRec0(){

  strip = 0;
  clust_nr = 0;
  for (int icl =0 ; icl< __MAXCLUSTERNR__; ++icl) cluster[icl] = 0;
  __adj_strip = 0;

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

void LEvRec0::SetVirginMode(void) {
  if(cluster[0] != 0) {
    for (int icl =0 ; icl< __MAXCLUSTERNR__; ++icl) delete[] cluster[icl];
    for (int icl =0 ; icl< __MAXCLUSTERNR__; ++icl) cluster[icl] = 0;
    clust_nr = 0;
    __adj_strip = 0;
  }
  if(!strip) {
    strip = new short[NCHAN];
    for(int i=0; i<NCHAN; ++i) strip[i]=0;
  }
  return;
}

void LEvRec0::SetZeroSuppressedMode(const unsigned short adj_strip) {
  if(strip != 0) {
    delete[] strip;
  }
  if(!cluster[0]) {
    clust_nr = 0;
    __adj_strip = adj_strip;
    for(int icl=0; icl< __MAXCLUSTERNR__; ++icl) {
      cluster[icl] = new short[2*adj_strip+2];
      for(int ich=0; ich<2*adj_strip+2; ++ich) cluster[icl][ich] = 0;
    }
  }
  return;
}

const int LEvRec0::trigger(const int i, const int j) const {
  if(i<0 || i>=NTRIGSCINT) return -999;
  if(j<0 || j>=2) return -999;
  return pmt_high[i+j*NPMT/2];
}

const int LEvRec0::plane(const int i, const int j) const {
  if(i<0 || i>=NSCINTPLANES) return -999;
  if(j<0 || j>=2) return -999;
  return pmt_high[NTRIGSCINT+i+j*NPMT/2];
}

const int LEvRec0::lyso(const int i) const {  //***## LOW GAIN FOR LYSO ##***
  if(i<0 || i>=NLYSOCRYSTALS) return -999;
  if(i==0) return pmt_low[28]; // North West 
  if(i==1) return pmt_low[62]; // North
  if(i==2) return pmt_low[29]; // North East
  if(i==3) return pmt_low[30]; // West
  if(i==4) return pmt_low[31]; // Center
  if(i==5) return pmt_low[60]; // East
  if(i==6) return pmt_low[27]; // South West
  if(i==7) return pmt_low[61]; // South
//  if(i==2) return pmt_low[59]; // South East
  if(i==8) return pmt_low[59]; // South East

}

const int LEvRec0::veto(const int i, const int j) const {
  if(i<0 || i>=NVETOSCINT) return -999;
  if(j<0 || j>=2) return -999;
  return pmt_high[NTRIGSCINT+NSCINTPLANES +i+j*NPMT/2];

}

void LEvRec0::DumpStrip(void) const {
  std::cout << "strip" << std::endl;
  if(IsVirgin()) {
    for(int i=0; i<NCHAN;++i) std::cout << strip[i] << " ";
    std::cout << std::endl;
  } else if(IsZeroSuppressed()){
    for(int i=0; i<clust_nr; ++i) {
      std::cout << cluster[i][0] << "      ";
      for(int ich=1; ich<=2*__adj_strip+1; ++ich) std::cout << cluster[i][ich] << " ";
    }
    std::cout << std::endl;
  }
  return;
}


void LEvRec0::DumpEventIndex() const{
  std::cout << "event_index " << event_index << std::endl;
  return;
}

//0x63 RUN_MIXED (VR+ZS-CN)
//0x36 RUN_ZERO-SUPPRESSED
//0x55 RUN_ZERO-SUPPRESSED COMMON NOISE
bool  LEvRec0::IsZeroSuppressed(void) const {
   bool ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x0055|| runType == 0x0036|| runType == 0x6336 || runType == 0x6355)
      ret = true;
   return ret;   
}

//0x4E RUN_VIRGIN_RAW
//0x63(4E) RUN_MIXED (VR+ZS-CN)
//0x78 RUN_GENERIC (HV and Gain PMT Calibration)->VR+ packet HVPS mon
bool  LEvRec0::IsVirgin(void) const {
   bool ret = false;
   //std::cout << std::hex << "runType = 0x" << runType << std::dec << std::endl;
   if (runType == 0x634e ||runType ==  0x004E|| runType ==  0x0078)
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

void LEvRec0Md::Dump(void) const {

   std::cout << "boot_nr = " << boot_nr << std::endl;
   std::cout << "run_id = " << run_id  << std::endl;
   std::cout << "run_type = " << run_type << std::endl;
   std::cout << "run_duration = " << run_duration << std::endl;
   std::cout << "orbit_Zone = " << orbit_Zone << std::endl;
   for(int i=0; i<3; ++i)
      std::cout << "silConfig.ladder_on["<< i << "] = " << silConfig.ladder_on[i] << std::endl;
   for(int i=0; i<3; ++i)
      std::cout << "silConfig.ladder_mask["<< i << "] = " << silConfig.ladder_mask[i] << std::endl;
   std::cout << "silConfig.plane_HV[0] = "  << silConfig.plane_HV[0] << std::endl;
   std::cout << "silConfig.plane_HV[1] = " << silConfig.plane_HV[1] << std::endl;
   std::cout << "silConfig.adj_strip =  " << silConfig.adj_strip << std::endl;
   std::cout << "silConfig.zero_supp_thrd = " << silConfig.zero_supp_thrd << std::endl; 
   std::cout << "silConfig.thrd_CN_HIGH = " << (int)silConfig.thrd_CN_HIGH << std::endl;
   std::cout << "silConfig.thrd_CN_LOW = " << (int)silConfig.thrd_CN_LOW << std::endl;
   std::cout << "silConfig.calib_event_CN = " << silConfig.calib_event_CN << std::endl;
   std::cout << "silConfig.calib_event_ped = " << silConfig.calib_event_ped << std::endl;
   std::cout << "silConfig.calib_event_RMS = " << silConfig.calib_event_RMS << std::endl;
   std::cout << "silConfig.calib_event_gauss = " << silConfig.calib_event_gauss << std::endl;
   std::cout << "silConfig.gauss_check = " << silConfig.gauss_check << std::endl;
   for(int i=0; i<2; ++i)
      std::cout << "trigger_mask[" << i << "] = " << (int)trigger_mask[i] << std::endl; 
   for(int i=0; i<120; ++i)
      std::cout << "easiroc_config[" << i << "] = "  << easiroc_config[i] << std::endl;
   for(int i=0; i<2; ++i)
      std::cout << "PMT_mask[" << i << "] = " << PMT_mask[i] << std::endl;    
   for(int i=0; i<12; ++i)
      std::cout << "HV_mask[" << i << "] = " << HV_mask[i] << std::endl;
   for(int i=0; i<10; ++i)
      std::cout << "HV_value["<< i << "] = " << HV_value[i] << std::endl; 
   for(int i=0; i<4; ++i)
      std::cout << "gen_trig_mask[" << i << "] = " << gen_trig_mask[i] << std::endl;

    std::cout << "broadcast.GPS.sec = " << broadcast.GPS.sec << std::endl;
   for(int i=0; i<3; ++i)
      std::cout << "broadcast.GPS.axis_pos[" << i<< "] = " << broadcast.GPS.axis_pos[i] << std::endl;
   for(int i=0; i<3; ++i)
      std::cout << "broadcast.GPS.axis_vel["<< i<< "] = " << broadcast.GPS.axis_vel[i] << std::endl;
   std::cout << "broadcast.GPS.dataID = " << broadcast.GPS.dataID << std::endl;
   std::cout << "broadcast.GPS.lon = " << broadcast.GPS.lon << std::endl;
   std::cout << "broadcast.GPS.lat = " << broadcast.GPS.lat << std::endl;
   for(int i=0; i<3; ++i)
      std::cout << "broadcast.GPS.NU[" << i << "] = "
		<< (int)broadcast.GPS.NU[i] << std::endl; 
 return;
}

// Added according to QuickLook
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

LEvRec0::~LEvRec0(){
  if(strip) delete[] strip;
  for (int icl =0 ; icl< __MAXCLUSTERNR__; ++icl) delete[] cluster[icl];

}
