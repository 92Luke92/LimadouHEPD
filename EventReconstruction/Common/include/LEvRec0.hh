#include "detector_const.hh"
#include "analysis_const.hh"

#ifndef __LEVREC0__
#define __LEVREC0__ 1

class LEvRec0 {

public:
  LEvRec0();
  
  unsigned short   runType;
  unsigned short   boot_nr;
  unsigned short   run_id;
  unsigned int     event_index;
  unsigned short   event_length;
  unsigned int     trigger_index;
  unsigned int     hepd_time;
  
  unsigned short   pmt_high[NPMT];
  unsigned short   pmt_low[NPMT];
  unsigned int     PMTBoard_trigger_counter;
  unsigned short   lost_trigger;
  unsigned short   rate_meter[NRATEMETER];
  bool             trigger_flag[NPMT];
  unsigned int     alive_time;
  unsigned int     dead_time;

  short            strip[NCHAN];
   
  void DumpStrip(void) const;
  void DumpEventIndex() const;
  bool IsCompressed() const;
  bool IsVirgin() const;
};


#endif
