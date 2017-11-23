#include "LLysoMSCrystal.hh"
#include "LLysoSignal.hh"

#include <iostream>
#include <stdlib.h>
#include "detector_const.hh"

LLysoMSCrystal& LLysoMSCrystal::operator=(const LLysoMSCrystal& otherlyso) {
  // check for self-assignment
  if(&otherlyso == this)
    return *this;

  // Copy operations
  this->isHG = otherlyso.isHG;
  this->count = otherlyso.count;
  this->sigma = otherlyso.sigma;
  this->sn = otherlyso.sn;

  return *this;
}

LLysoMSCrystal::LLysoMSCrystal(){
  SetDefault();
}

void LLysoMSCrystal::SetDefault() {
  mscindex = -999;
	isHG=true;  // default value
  count= -999.;
  sigma= -999.;
  sn= -999.;
  posX = -999.;
  posY = -999.;
  posZ =-999.;
  return;
}

LLysoMSCrystal::LLysoMSCrystal(const LLysoSignal *lysoSignal, const bool isHG_INP, const double threshold) {
  LoadLysoSignal(lysoSignal, isHG_INP, threshold);
}

void LLysoMSCrystal::LoadLysoSignal(const LLysoSignal *lysoSignal, const bool isHG_INP, const double threshold) {
    if(lysoSignal==0) {
    std::cerr << __LLYSOMSCRYSTAL__ << "LLysoMSCrystal::LoadLysoSignal(const LLysoMSCrystal *lysoSignal, const bool isHG_INP, const double threshold)   error:"
        << " lysoSignal=0" << std::endl;
    SetDefault();
    return;
  }
  int nsigcrys=lysoSignal->GetNOfSignificantCrystal(isHG_INP,threshold);
  if(nsigcrys==0 || nsigcrys>NLYSOCRYSTALS) {
    //std::cout << __LLYSOMSCRYSTAL__ << "Warning! Found " << nsigcrys << " lyso crystals ("
    //    << (isHG_INP ? "high" : "low") << " gain) having SN higher than " << threshold << std::endl;
    if(nsigcrys==0) {
    //  std::cout << __LLYSOMSCRYSTAL__ << "Impossible to continue." << std::endl;
      return;
    } else {
      std::cout << __LLYSOMSCRYSTAL__ << "Only the most significant kept. Possible signal loss" << std::endl;
    }
  }

  isHG = isHG_INP;
  mscindex = lysoSignal->GetTheMostSignificantCrystal(isHG_INP,threshold);
  posX = lysoSignal->GetX(mscindex);
  posY = lysoSignal->GetY(mscindex);
  posZ = lysoSignal->GetZ(mscindex);
  count = (isHG ? lysoSignal->cont_hg[mscindex][0] : lysoSignal->cont_lg[mscindex][0]);
  sn = (isHG ? lysoSignal->sn_hg[mscindex][0] : lysoSignal->sn_lg[mscindex][0]);
  sigma = count/sn;

  return;
}

void LLysoMSCrystal::Dump() const {
  std::cout << "***************" << std::endl
	    << (isHG ? "\t High Gain" : "\t Low Gain") << std::endl
        <<"\t bar index ";

  std::cout << std::endl<< "\t counts ";
  std::cout << count;
  std::cout << std::endl << "\t sigma  ";
  std::cout << sigma;
  std::cout << std::endl << "\t sn     ";
  std::cout << sn;
  std::cout << std::endl << "***************" << std::endl;
  return;
}
