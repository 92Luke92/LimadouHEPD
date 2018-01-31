/*
 * MeV2ADCmethod.hh
 *
 *
 *
 */


#ifndef MEV2ADCMETHOD_HH
#define MEV2ADCMETHOD_HH

#include <calomev2adcmethod.hh>
#include "pmtenum.hh"

class MeV2ADCMethod: public calomev2adcmethod {
  public:
    MeV2ADCMethod (std::string datacardname);
    virtual float adcFromMevNoPed (float mev, int sensor) = 0;
    virtual void setBeamEnergy(int energy) {beamEnergy=energy;};

  protected:
    void init();



    struct PMTnumbers {
        PMTenum index;
        float pedMean;
        float pedSigma;
        float maxPeak;
        float mevPeak;
        bool  isScint;
        float mev2adc;
   	PMTnumbers(){};
   };

    void convertDatacard();
    void addMevPeak();
    void computeMev2ADCratio();

    std::array<PMTnumbers, NPMT>  PMTs;

};

#endif
