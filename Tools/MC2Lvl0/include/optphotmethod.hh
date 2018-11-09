/*
 * optphotmethod.hh
 *
 *
 *
 */

#ifndef OPTPHOTMETHOD_HH
#define OPTPHOTMETHOD_HH

#include <calomev2adcmethod.hh>
#include "TRandom3.h"


class OptPhotMethod : public calomev2adcmethod
{
  public:
    OptPhotMethod(std::string datacardname);
  ~OptPhotMethod();
    float adcFromMevNoPed(float mev, int sensor);

  private:
    void init();

    struct PMTnumbersOptPhot  {
      float p0;
      float p1;
      float p2;
      float Ped;
      float Sigma;
    };

  TRandom3 *Rand;
  std::array<PMTnumbersOptPhot, NPMT>  pmtParameters;

};


#endif
