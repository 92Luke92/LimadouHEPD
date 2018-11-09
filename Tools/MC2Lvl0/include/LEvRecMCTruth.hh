#include "detector_const.hh"
#include "analysis_const.hh"

#ifndef __LEVRECMCTRUTH__
#define __LEVRECMCTRUTH__ 1




class LEvRecMCTruth {

public:
  LEvRecMCTruth();

  unsigned int particleid;
  unsigned short energy;
  unsigned short x;
  unsigned short y;
  unsigned short theta;
  unsigned short phi;

  void Dump(void) const;
  
};


#endif
