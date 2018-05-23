/*
 * RawEdepMethod.hh
 *
 *
 *
 */


#ifndef RAWEDEPMETHOD_HH
#define RAWEDEPMETHOD_HH

#include <calomev2adcmethod.hh>

class RawEdepMethod: public calomev2adcmethod
{
   public:
      RawEdepMethod(std::string datacardname);
      float adcFromMevNoPed(float mev, int sensor);

   private:
      void init();
      /* add your private declarations */
};

#endif /* RAWEDEPMETHOD_HH */
