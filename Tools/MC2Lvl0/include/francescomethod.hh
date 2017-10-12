/*
 * examplemethod.hh
 *
 *
 *
 */


#ifndef FRANCESCOMETHOD_HH
#define FRANCESCOMETHOD_HH

#include <calomev2adcmethod.hh>

class ExampleMethod: public calomev2adcmethod
{
   public:
      ExampleMethod(std::string datacardname);
      short adcFromMev(float mev, int sensor);

   private:
      void init();
      /* add your private declarations */
};

#endif 
