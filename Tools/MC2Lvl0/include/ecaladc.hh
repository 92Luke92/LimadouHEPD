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


class EcalADC {
  public:
    EcalADC();
    void SetPositions (std::vector<Edep_Pos> edepPositions )
    {
        edepPos = edepPositions;
    }
    std::vector<short> GetStrips();
    void NormalizePMThg ( ushort* pmt_high);
    void NormalizePMTlg ( ushort* pmt_low);



  private:


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

    // Aggregate brace init; please don't change this order
    struct PMTnumbers {
        PMTenum index;
        std::string name;
        float pedMean;
        float pedSigma;
        float maxPeak;
        bool isScint;
        int layerScint;
        float scintMeVPeak;
        TVector3 physPosition;
    };

    using PMTarray = std::array<PMTnumbers, NPMT>;

    std::array<TVector3, NPMT> GetPMTphysPos();
    void initHGaggregate();
    void initLGaggregate();
    void initScint();
    void initMCpos();

    PMTarray hgPMT, lgPMT;
    std::vector<Edep_Pos> edepPos;
    std::vector<float> CorrectPMT (std::vector<Edep_Pos>);
    std::vector<int> GetPMTHGPeds();
    std::vector<int> GetPMTLGPeds();
    float PMTAttCorr (float dist);
    float EcalMev2ADCfactorHG (PMTenum PMT);
    float EcalMev2ADCfactorLG (PMTenum PMT);


};

#endif /* ECALADC_HH */
