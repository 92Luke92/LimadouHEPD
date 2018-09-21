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
#include "pmtenum.hh"

#include "laurentmethod.hh"
#include "francescomethod.hh"
#include "matteomethod.hh"
#include "rawedepmethod.hh"
#include "synthesismethod.hh"
#include "optphotmethod.hh"

#include "RootPmtHits.hh"


#include "TVector3.h"
#include "TVector2.h"


class EcalADC {
  public:
  enum method {Laurent, Francesco, Matteo, RawEdep, Synthesis, OptPhot};
  bool OPmethod;
    EcalADC(method ecalmethod);
    void SetPositions (std::vector<Edep_Pos> pmt_info );
    void NormalizePMThg ( ushort* pmt_high);
    void NormalizePMTlg ( ushort* pmt_low);
    void NormalizePMThg_OP (std::vector<RootPmtHits> pmtHits, ushort* pmt_high);
    void NormalizePMTlg_OP (std::vector<RootPmtHits> pmtHits, ushort* pmt_low);
    void setMCEnergy (int mcE);
  bool GetOPmethod() {return OPmethod;}




  private:

    int mcEnergy;
    void initMCpos();
    float VectorXYDist (TVector2 v1, TVector2 v2);
    std::array<float, NPMT>   correctedPMTs;
    float PMTAttCorr (float dist);
    void NormalizePMT ( ushort* pmt_out, calomev2adcmethod* method);
    void NormalizePMT_OP (std::vector<RootPmtHits> pmtHits, ushort* pmt_out, calomev2adcmethod* method);
    calomev2adcmethod* methodHg;
    calomev2adcmethod* methodLg;
    std::map<PMTenum, TVector2> PMTpos;

};

#endif /* ECALADC_HH */
