/*
 * calomev2adcmethod.hh
 *
 *
 *
 */


#ifndef CALOM2AMETHOD_HH
#define CALOM2AMETHOD_HH

#include "csv2fvec.hh"

class calomev2adcmethod
{
   public:
      calomev2adcmethod(std::string datacardname);
      virtual float adcFromMev(float mev, int sensor) = 0;
      void dumpDatacard();

   protected:
      std::vector<std::vector<float>> datacard;
   private:
      virtual void init() = 0;
};

#endif /* CALOMEV2ADCMETHOD_HH */
