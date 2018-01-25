/*
 * matteomethod.hh
 *
 *
 *
 */

#ifndef MATTEOMETHOD_HH
#define MATTEOMETHOD_HH

#include <mev2adcmethod.hh>

class MatteoMethod : public calomev2adcmethod
{
  public:
    MatteoMethod(std::string datacardname);
    short adcFromMev(float mev, int sensor);
    short Err_adcFromMev(float mev, int sensor);

  protected:
    void init();
    void getFitParameters();
    void getPedestals();


    struct PMTnumbersMatteo  {
      float Slope, ErrSlope;
      float Interc, ErrInterc;
      float pedMean;
    };


   private:
    std::array<PMTnumbersMatteo, NPMT>  pmtParameters;

};

#endif
