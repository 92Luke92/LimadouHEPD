/*
 * MeV2ADCmethod.hh
 *
 *
 *
 */


#ifndef MEV2ADCMETHOD_HH
#define MEV2ADCMETHOD_HH

#include <calomev2adcmethod.hh>

class MeV2ADCMethod: public calomev2adcmethod {
  public:
    MeV2ADCMethod (std::string datacardname);
    virtual short adcFromMev (float mev, int sensor) = 0;
    virtual void setBeamEnergy(int energy) {beamEnergy=energy;};

  protected:
    void init();

    enum PMTenum {T1e, T2e, T3e, T4e, T5e, T6e,
                  P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                  VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                  T1w, T2w, T3w, T4w, T5w, T6w,
                  P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                  VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                 };




    const std::array<const PMTenum, NPMT> PMTiterator = {{T1e, T2e, T3e, T4e, T5e, T6e,
                                                         P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                                                         VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                                                         T1w, T2w, T3w, T4w, T5w, T6w,
                                                         P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                                                         VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                                                        }};

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
