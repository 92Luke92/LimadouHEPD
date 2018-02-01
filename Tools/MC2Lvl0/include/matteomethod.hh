/*
 * matteomethod.hh
 *
 *
 *
 */

#ifndef MATTEOMETHOD_HH
#define MATTEOMETHOD_HH

#include <calomev2adcmethod.hh>

class MatteoMethod : public calomev2adcmethod
{
  public:
    MatteoMethod(std::string datacardname);
    float adcFromMevNoPed(float mev, int sensor);
    short Err_adcFromMev(float mev, int sensor);

  private:
    void init();

    struct PMTnumbersMatteo  {
      float Slope, ErrSlope;
      float Interc, ErrInterc;
    };

    std::array<PMTnumbersMatteo, NPMT>  pmtParameters;

};

#endif
