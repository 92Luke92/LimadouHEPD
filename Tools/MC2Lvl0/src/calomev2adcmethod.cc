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
   std::cout<<datacardname<<std::endl;
}





void calomev2adcmethod::dumpDatacard() {
  for (auto line : datacard) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }
}


short calomev2adcmethod::trimADC(int untrimmedADC) {
     if (untrimmedADC <0 )    untrimmedADC = 0;
     if (untrimmedADC > NADC) untrimmedADC = NADC - 1;
     return static_cast<short> (untrimmedADC);
}
