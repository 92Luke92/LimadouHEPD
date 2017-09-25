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
}





void calomev2adcmethod::dumpDatacard() {
  for (auto line : datacard) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }
}


