#include "LCaloEqualization.hh"
#include "detector_const.hh"
#include <math.h>
#include <algorithm>
#include <iostream>

const double __WARNINGMPVADCTRHESHOLD__ = 20.;
const double __WARNINGSIGMAADCTRHESHOLD__ = 3.;


const double LCaloEqualization::GetMPVFactor(const int index) const {
  if(eqFactors.size() > index) return -999.;
  else return eqFactors.at(index);
}

const double LCaloEqualization::GetSigmaFactor(const int index) const {
  if(sigmaEqFactors.size() > index) return -999.;
  else return sigmaEqFactors.at(index);
}


LCaloEqualization::LCaloEqualization() {
  //	pedestalCheks.clear();
  sigmaEqFactors.clear();
  eqFactors.clear();
}

void LCaloEqualization::Write(const std::string fileOut) const {
  std::ofstream out(fileOut.c_str());
  if (out.good()) {
    for (int i = 0; i < NPMT-1; i++) {
      out << eqFactors.at(i) << "\t" << sigmaEqFactors.at(i) << std::endl;
    }
  } else {
    std::cerr << "outputfile not defined..." << std::endl;
  }
  out.close();
}


LCaloEqualization* LCaloEqualization::Read(const std::string fileIn) {
  return Read(fileIn.c_str());
}

LCaloEqualization* LCaloEqualization::Read(const char *fileIn) {
  std::ifstream fin(fileIn, std::ifstream::in);
  LCaloEqualization *result = 0;
  if(fin.good()) {
    result = new LCaloEqualization();
    double eqMPV, eqSigma;
    for (int i = 0; i < NPMT; i++) {
      fin >> eqMPV >> eqSigma; 
      result->addFactors(eqMPV, eqSigma); 
    }
  } else {
    std::cerr << __LCALOEQUALIZATION__ << " read error! Input file not good." << std::endl;
  }
  fin.close();
  return result;
}

void LCaloEqualization::Reset(void) {
  eqFactors.resize(0);
  sigmaEqFactors.resize(0);
  return;
}

const bool LCaloEqualization::CheckStatus(void) const {
  if(eqFactors.size()==0) return false;
  if(sigmaEqFactors.size()==0) return false;
  if(eqFactors.at(12) < __WARNINGMPVADCTRHESHOLD__ && eqFactors.at(12) != -1.) return false; // check on the first pmt of first plane. No equalization case excluded.
  if(sigmaEqFactors.at(12) < __WARNINGSIGMAADCTRHESHOLD__&& sigmaEqFactors.at(12) != -1.) return false; // check on the first pmt of first plane. No equalization case excluded.
  return true;
}