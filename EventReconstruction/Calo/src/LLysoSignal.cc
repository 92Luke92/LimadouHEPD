#include "LLysoSignal.hh"
#include "detector_const.hh"

#include <iostream>

LLysoSignal::LLysoSignal() {
    nunits=LYSOCRYSTALS;
    npmts=1;
    CreateContainers();
}

LLysoSignal::~LLysoSignal() {

}

void LLysoSignal::Dump() const {
  std::cout << "############# Lyso signal dumping." << std::endl;
  DumpAll();
  return;
}

double LLysoSignal::GetX(const int iu, const int ipmt) const {
  double result=0.;
  if(iu==2||iu==5||iu==6) result=0.5*LYSOSIDE+LYSO_LYSO_GAP+0.5*LYSOSIDE;
  else if(iu==0||iu==1||iu==3) result=-(0.5*LYSOSIDE+LYSO_LYSO_GAP+0.5*LYSOSIDE);
  else if(iu==4||iu==7||iu==8) result=0.;
  
  return result;
}

double LLysoSignal::GetY(const int iu, const int ipmt) const {
  double result=0.;
  if(iu==0||iu==5||iu==7) result=0.5*LYSOSIDE+LYSO_LYSO_GAP+0.5*LYSOSIDE;
  else if(iu==1||iu==2||iu==8) result=-(0.5*LYSOSIDE+LYSO_LYSO_GAP+0.5*LYSOSIDE);
  else if(iu==3||iu==4||iu==6) result=0.;
  
  return result;
}

double LLysoSignal::GetZ(const int iu, const int ipmt) const {
  double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP
    +TRIGBARTHICKNESS+TRIG_SCINT_GAP
    +NSCINTPLANES*SCINTPLANETHICKNESS+(NSCINTPLANES-1)*SCINT_SCINT_GAP+SCINT_LYSO_GAP;
  double result=OFFSET+0.5*LYSOTHICKNESS;
  
  return result;
}

