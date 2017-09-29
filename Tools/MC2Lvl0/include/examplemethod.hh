/*
 * examplemethod.hh
 *
 *
 *
 */


#ifndef LAURENTMETHOD_HH
#define LAURENTMETHOD_HH

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

#endif /* LAURENTMETHOD_HH */
