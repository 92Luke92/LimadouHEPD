#include "LVetoSignal.hh"
#include "detector_const.hh"
#include <iostream>

LVetoSignal::LVetoSignal() {
    nunits=VETOPLANES;
    npmts=2;
    CreateContainers();
}

LVetoSignal::~LVetoSignal() {

}

void LVetoSignal::Dump() const {
  std::cout << "############# Veto signal dumping." << std::endl;
  DumpAll();
  return;
}

double LVetoSignal::GetX(const int iu, const int ipmt) const {
  double result=0.;
  if(iu==0 && ipmt==0) result=0.5*XLVETOWIDTH;
  else if(iu==0 && ipmt==1) result=-0.5*XLVETOWIDTH;
  else if(iu==1) {
    double OFFSET=0.5*SCINTPLANESIDE+SCINT_YLVETO_GAP;
    result=OFFSET+0.5*VETOTHICKNESS;
  } else if(iu==2 && ipmt==0) result=-0.5*XLVETOWIDTH;
  else if(iu==2 && ipmt==1) result=+0.5*XLVETOWIDTH;
  else if(iu==3) {
    double OFFSET=0.5*SCINTPLANESIDE+SCINT_YLVETO_GAP;
    result=-(OFFSET+0.5*VETOTHICKNESS);
  } else if(iu==4 && ipmt==0) result=0.5*BVETOSIDE;
  else if(iu==4 && ipmt==1) result=-0.5*BVETOSIDE;

  return result;
}

double LVetoSignal::GetY(const int iu, const int ipmt) const {
  double result=0.;
  if(iu==1 && ipmt==0) result=-0.5*YLVETOWIDTH;
  else if(iu==1 && ipmt==1) result=0.5*YLVETOWIDTH;
  else if(iu==0) {
    double OFFSET=0.5*SCINTPLANESIDE+SCINT_XLVETO_GAP;
    result=-(OFFSET+0.5*VETOTHICKNESS);
  } else if(iu==3 && ipmt==0) result=+0.5*XLVETOWIDTH;
  else if(iu==3 && ipmt==1) result=-0.5*XLVETOWIDTH;
  else if(iu==2) {
    double OFFSET=0.5*SCINTPLANESIDE+SCINT_XLVETO_GAP;
    result=-(OFFSET+0.5*VETOTHICKNESS);
  } else if(iu==4 && ipmt==0) result=-0.5*BVETOSIDE;
  else if(iu==4 && ipmt==1) result=+0.5*BVETOSIDE;

  return result;
}

double LVetoSignal::GetZ(const int iu, const int ipmt) const {
  double result=0.;
  if(iu==4) { // bottom veto
    double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP
      +TRIGBARTHICKNESS+TRIG_SCINT_GAP
      +NSCINTPLANES*SCINTPLANETHICKNESS+(NSCINTPLANES-1)*SCINT_SCINT_GAP+SCINT_LYSO_GAP
      +LYSOTHICKNESS+LYSO_BVETO_GAP;
    result = OFFSET+0.5*VETOTHICKNESS;
  } else if(ipmt%2==0) { // lateral veto up
    double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP
      +TRIGBARTHICKNESS+TRIG_LVETO_ZGAP;
    result = OFFSET;
  } else {
    double OFFSET = SILICONTHICKNESS+SILOUT_SILIN_GAP+SILICONTHICKNESS+SILIN_TRIGGER_GAP
      +TRIGBARTHICKNESS+TRIG_LVETO_ZGAP;
    result = OFFSET+LVETOLENGTH;
  }

  return result;
}
