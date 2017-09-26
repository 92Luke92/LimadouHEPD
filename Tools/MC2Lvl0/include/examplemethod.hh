/*
 * examplemethod.hh
 *
 *
 *
 */


#ifndef LAURENTMETHOD_HH
#define LAURENTMETHOD_HH

#include <calomev2adcmethod.hh>

class LaurentMethod: public calomev2adcmethod
{
   public:
      LaurentMethod(std::string datacardname);
      float adcFromMev(float mev, int sensor);

   private:
      void init();
      /* add your private declarations */
};

#endif /* LAURENTMETHOD_HH */
