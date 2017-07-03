#include "detector_const.hh"
#include "analysis_const.hh"

#include "LEvRec0.hh"
#include <iostream>

LEvRec0::LEvRec0(){
  Reset();
}


void LEvRec0::Reset() {
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
