/*
 * examplemethod.cc
 *

 *
 *
 */


#include "examplemethod.hh"


LaurentMethod::LaurentMethod(std::string datacardname)
   : calomev2adcmethod(datacardname)
{
 init();
}



void LaurentMethod::init() {

}



float LaurentMethod::adcFromMev(float mev, int sensor) {
   sensor++; beamEnergy++;
   return mev;
}
