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



short ExampleMethod::adcFromMevNoPed(float mev, int sensor) {
   sensor++;
   return static_cast<short> (mev);
}
