/*
 * optphotmethod.hh
 *
 *
 *
 */

#ifndef OPTPHOTMETHOD_HH
#define OPTPHOTMETHOD_HH

#include <calomev2adcmethod.hh>

class OptPhotMethod : public calomev2adcmethod
{
  public:
    OptPhotMethod(std::string datacardname);
    float adcFromMevNoPed(float mev, int sensor);

  private:
    void init();

    struct PMTnumbersOptPhot  {
      float Conv;
      float Ped;
      float Sigma;
    };

    std::array<PMTnumbersOptPhot, NPMT>  pmtParameters;

};


#endif
