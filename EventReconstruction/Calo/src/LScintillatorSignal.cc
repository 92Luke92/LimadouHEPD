#include "LScintillatorSignal.hh"
#include "detector_const.hh"

#include <iostream>
#include <math.h>

LScintillatorSignal::LScintillatorSignal() {
    nunits=SCINTPLANES;
    npmts=2;
    CreateContainers();
}

LScintillatorSignal::~LScintillatorSignal() {

}

void LScintillatorSignal::Dump() const {
  std::cout << "############# Scintillator signal dumping." << std::endl;
  DumpAll();
  return;
}


double LScintillatorSignal::GetX(const int iu, const int ipmt) const {
  double result=0.;
  if((iu+ipmt)%2==0) result= 0.5*SCINTPLANESIDE;
  else result = -0.5*SCINTPLANESIDE;
  return result;
}

double LScintillatorSignal::GetY(const int iu, const int ipmt) const {
  double result=0.;
  if(ipmt%2==0) result = 0.5*SCINTPLANESIDE;
  else result = -0.5*SCINTPLANESIDE;
  return result;
}

double LScintillatorSignal::GetZ(const int iu, const int ipmt) const {
  double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP
    +TRIGBARTHICKNESS+TRIG_SCINT_GAP;
  double result = OFFSET+(iu+0.5)*SCINTPLANETHICKNESS + iu*SCINT_SCINT_GAP;
  return result;
}

double LScintillatorSignal::GetSNOfUnit(const int unit, const bool isHG) const {
  double result=0;
  if(unit==4) return sqrt(2.)*sn_hg[unit][0]; // PMT 1 broken
  else return LCaloSignal::GetSNOfUnit(unit,isHG);
}

double LScintillatorSignal::GetCountsOfUnit(const int unit, const bool isHG) const {
  double result=0.;
  if(unit==4) return 2.*cont_hg[unit][0]; // PMT 1 broken
  else return LCaloSignal::GetCountsOfUnit(unit,isHG);
}
