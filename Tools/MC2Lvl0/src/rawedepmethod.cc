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
   return;
}


float RawEdepMethod::adcFromMevNoPed(float mev, int sensor) {
   return mev;
}

