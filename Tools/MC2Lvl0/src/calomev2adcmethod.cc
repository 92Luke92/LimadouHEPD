/*
 * calomev2adcmethod.cc
 *
 *
 *
 */


#include "calomev2adcmethod.hh"


calomev2adcmethod::calomev2adcmethod(std::string datacardname)
{
   csv2fvec datagetter;
   datacard=datagetter.fromDatacard(datacardname);
   if (datacard.empty()) std::cerr << "CaloM2A: init failed (datacard file not found)" << std::endl;
}





void calomev2adcmethod::dumpDatacard() {
  for (auto line : datacard) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }
}


short calomev2adcmethod::clipADC(int unclippedADC) {
     if (unclippedADC <0 )    unclippedADC = 0;
     if (unclippedADC > NADC) unclippedADC = NADC - 1;
     return static_cast<short> (unclippedADC);
}

short calomev2adcmethod::clipADC(float unclippedADC) {
     return clipADC (static_cast<int> (unclippedADC));
}

