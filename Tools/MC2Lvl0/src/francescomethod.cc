/*
 * francescomethod.cc
 *

 *
 *
 */


#include "francescomethod.hh"


FrancescoMethod::FrancescoMethod(std::string datacardname)
   : calomev2adcmethod(datacardname)
{
 init();
}



void FrancescoMethod::init() {
  convertDatacard();
  addMevPeak();
  computeMev2ADCratio();
}

void FrancescoMethod::convertDatacard() {
  int iPMT=0;
  for (auto line : datacard) {
      MyPMTnumbers pmt;
      pmt.PMT.index=PMTiterator[iPMT];
      pmt.PMT.pedMean =line[0];
      pmt.PMT.pedSigma=line[1];
      pmt.PMT.maxPeak =line[2];
      pmt.PMT.isScint =  (line[3] != 0) ;

      MyPMTs[iPMT]=pmt;
      iPMT++;
  }
  return;
}

void FrancescoMethod::addMevPeak()
{
    const std::array<float, NSCINTPLANES> MeVPeakLayer = {
        5.19256, 5.31434, 5.43623, 5.56329, 5.71436, 5.89574, 6.07625, 6.29347,
        6.53174, 6.80163, 7.13162, 7.51907, 8.00189, 8.59306, 9.37135, 10.4922
    };
    const std::array <PMTenum, 32 > scintPMT = {P1se, P2sw, P3se, P4sw, P5se, P6sw, P7se, P8sw,
                                    P9se, P10sw, P11se, P12sw, P13se, P14sw, P15se, P16sw,
                                    P1nw, P2ne, P3nw, P4ne, P5nw, P6ne, P7nw, P8ne,
                                    P9nw, P10ne, P11nw, P12ne, P13nw, P14ne, P15nw, P16ne
                                   };
    for (uint iPMT=0; iPMT<scintPMT.size(); iPMT++) {
        uint layer= iPMT % ( scintPMT.size()/2 );
        float peak=MeVPeakLayer[layer];
        int idx=  static_cast<int> (scintPMT[iPMT]);
        MyPMTs[idx].PMT.mevPeak=peak;
    }
    return;
}


void FrancescoMethod::computeMev2ADCratio() {

  for (auto pmt : MyPMTs) {
      float maxMev = pmt.PMT.isScint? pmt.PMT.mevPeak:15;
      float maxADC = pmt.PMT.maxPeak - pmt.PMT.pedMean;
      MyPMTs[static_cast<int>(pmt.PMT.index)].PMT.mev2adc = maxADC / maxMev;
  }

}

short FrancescoMethod::adcFromMev(float mev, int sensor) {
   MyPMTnumbers thisPMT=MyPMTs[sensor];
   float adcShift = mev * thisPMT.PMT.mev2adc;
   int untrimmedPMT = static_cast<int> (thisPMT.PMT.pedMean);
   return trimADC(untrimmedPMT);
}


