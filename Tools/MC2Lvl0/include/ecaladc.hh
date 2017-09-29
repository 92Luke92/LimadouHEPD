/*
 * ecaladc.hh
 *
 * Ecal Edep to ADC
 *
 */


#ifndef ECALADC_HH
#define ECALADC_HH

#include <vector>
#include <map>
#include <array>
#include <string>

#include "Edep_Pos.h"
#include "detector_const.hh"
#include "MCcoorPhysicalFrame.hh"
#include "laurentmethod.hh"


class EcalADC {
  public:
    EcalADC();
    void SetPositions (std::vector<Edep_Pos> pmt_info );
    void NormalizePMThg ( ushort* pmt_high);
    void NormalizePMTlg ( ushort* pmt_low);
    void setMCEnergy (int mcE);



  private:

    int mcEnergy;


    enum PMTenum {T1e, T2e, T3e, T4e, T5e, T6e,
                  P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                  VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                  T1w, T2w, T3w, T4w, T5w, T6w,
                  P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                  VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                 };




    const std::array<const PMTenum, NPMT> PMTiterator = {T1e, T2e, T3e, T4e, T5e, T6e,
                                                         P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw, P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                                                         VNu, VEu, VSu, VWu, VBne, L9sw, L7nw, L1ne, L8w, L5c,
                                                         T1w, T2w, T3w, T4w, T5w, T6w,
                                                         P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne, P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne,
                                                         VNd, VEd, VSd, VWd, VBsw, L3se, L2e, L6s, L4n, NC
                                                        };

    const std::array <PMTenum, 32 > scintPMT = {P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw,
                                    P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                                    P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne,
                                    P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne
                                   };



    // Aggregate brace inits; please don't change this order

    struct LinearFitParam {
        PMTenum index;
        int energy;
        float a;
        float b;
    };



    void initMCpos();

    std::array<float, NPMT>   correctedPMTs;

    std::vector< LinearFitParam> linearFitParams;
    float PMTAttCorr (float dist);

    void NormalizePMT ( ushort* pmt_out, calomev2adcmethod* method);

    std::pair<float, float> getFitCoeff(PMTenum pmt) ;
    float applyMCshaping(float ADCval, PMTenum pmt);

    calomev2adcmethod* methodHg;
    calomev2adcmethod* methodLg;

    std::map<PMTenum, TVector2> PMTpos;

};

#endif /* ECALADC_HH */
