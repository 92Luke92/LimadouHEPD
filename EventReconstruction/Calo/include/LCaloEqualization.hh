#ifndef __LCALOEQUALIZATION__
#define __LCALOEQUALIZATION__ "LCaloEqualization  ######### "

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "LCaloCalibration.hh"
#include "detector_const.hh"

class LCaloEqualization {
 public:
  LCaloEqualization();
  ~LCaloEqualization(){Reset();};
  void Write(const std::string fileOut) const;
  static LCaloEqualization* Read(const std::string fileIn);
  static LCaloEqualization* Read(const char *fileIn);
  inline void addFactors(double mpv, double sigma) {
    eqFactors.push_back(mpv);
    sigmaEqFactors.push_back(sigma);
  };
  void Reset(void);
  const bool CheckStatus(void) const ;
  const double GetMPVFactor(const int index) const;
  const double GetSigmaFactor(const int index) const;
 
 private:
  bool isHG;
  // std::vector<unsigned short> pedestalChecks;
  std::vector<double> eqFactors;
  std::vector<double> sigmaEqFactors;
};

#endif
