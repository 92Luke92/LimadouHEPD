/*
 * RawEdepMethod.cc
 *

 *
 *
 */


#include "rawedepmethod.hh"


RawEdepMethod::RawEdepMethod(std::string datacardname)
   : calomev2adcmethod(datacardname)
{
 init();
}



void RawEdepMethod::init() {
   setPedFromDatacard("nullped.csv"); // boy ain't that ugly
   return;
}


float RawEdepMethod::adcFromMevNoPed(float mev, int sensor) {
   return 100*mev;
}

