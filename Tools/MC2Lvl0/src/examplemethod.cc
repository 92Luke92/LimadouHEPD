/*
 * examplemethod.cc
 *

 *
 *
 */


#include "examplemethod.hh"


ExampleMethod::ExampleMethod(std::string datacardname)
   : calomev2adcmethod(datacardname)
{
 init();
}



void ExampleMethod::init() {

}



float ExampleMethod::adcFromMev(float mev, int sensor) {
   sensor++; beamEnergy++;
   return mev;
}
