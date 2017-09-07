#include "detector_const.hh"
#include "analysis_const.hh"
#include "TFile.h"
#include "TTree.h"

#ifndef __LEVTEMP__
#define __LEVTEMP__ 1

class LEvTemp {

public:
  LEvTemp();
  int   trig_mult=-1;
  int   bars_status[6];
  int   calo_mult=-1;
  int  lyso_mult=-1;
  float trig_signal_sum=0.;
  float calo_signal_sum=0.;
  float lyso_signal_sum=0.;   
  float plane_signal[16];
  float trig_ener_dep;
  float calo_ener_dep;
  float lyso_ener_dep;

};

#endif
