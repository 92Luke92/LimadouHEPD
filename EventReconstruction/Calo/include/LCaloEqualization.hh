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
  void Write(std::string fileOut) const;
  inline void addFactors(double mpv, double sigma) {
    eqFactors.push_back(mpv);
    sigmaEqFactors.push_back(sigma);
  };

 private:
  bool isHG;
  // std::vector<unsigned short> pedestalChecks;
  std::vector<double> eqFactors;
  std::vector<double> sigmaEqFactors;
};

#endif
