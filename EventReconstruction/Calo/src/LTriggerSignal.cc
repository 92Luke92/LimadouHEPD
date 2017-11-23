#include "LTriggerSignal.hh"
#include "detector_const.hh"

#include <iostream>

LTriggerSignal::LTriggerSignal() {
    nunits=TRIGBARS;
    npmts=2;
    CreateContainers();
}

LTriggerSignal::~LTriggerSignal() {

}

void LTriggerSignal::Dump() const {
  std::cout << "############# Trigger signal dumping." << std::endl;
  DumpAll();
  return;
}

double LTriggerSignal::GetX(const int iu, const int ipmt) const {
  double result= 0.5*TRIGBARLENGTH;
  if(ipmt%2==0) return result;
  else return -result;
}

double LTriggerSignal::GetY(const int iu, const int ipmt) const {
  double result= (0.5+iu)*TRIGBARWIDTH;
  return result;
}

double LTriggerSignal::GetZ(const int iu, const int ipmt) const {
  double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP;
  double result = OFFSET+0.5*TRIGBARTHICKNESS;
  return result;
}


bool LTriggerSignal::SignalSharedAmongBars(const bool isHG, const double threshold) const {
  int maxbar = GetTheMostSignificantUnit(isHG,threshold);
  if(maxbar==-999) return false;
  int max2bar = GetThe2ndMostSignificantUnit(isHG,threshold);
  if(max2bar==-999) return false;
  return (maxbar-max2bar==1 || maxbar-max2bar==-1);
}

