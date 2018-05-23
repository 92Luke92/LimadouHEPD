/*
 * examplemethod.hh
 *
 *
 *
 */


#ifndef EXAMPLEMETHOD_HH
#define EXAMPLEMETHOD_HH

#include <calomev2adcmethod.hh>

class ExampleMethod: public calomev2adcmethod
{
   public:
      ExampleMethod(std::string datacardname);
      short adcFromMevNoPed(float mev, int sensor);

   private:
      void init();
      /* add your private declarations */
};

#endif /* EXAMPLEMETHOD_HH */
