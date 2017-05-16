#include "LCaloEqualization.hh"
#include <math.h>
#include <algorithm>
#include <iostream>

LCaloEqualization::LCaloEqualization() {
  //	pedestalCheks.clear();
  sigmaEqFactors.clear();
  eqFactors.clear();
}

void LCaloEqualization::Write(std::string fileOut) const {
  std::ofstream out(fileOut.c_str());
  if (out.good()) {
    int nCh = eqFactors.size();
    for (int i = 0; i < nCh; i++) {
      out << eqFactors.at(i) << "\t" << sigmaEqFactors.at(i) << std::endl;
    }
  } else {
    std::cerr << "outputfile not defined..." << std::endl;
  }
  out.close();
}
