/*
 * laurentmethod.cc
 *

 *
 *
 */


#include "laurentmethod.hh"


LaurentMethod::LaurentMethod(std::string datacardname)
   : calomev2adcmethod(datacardname)
{
 init();
}



void LaurentMethod::init() {
  convertDatacard();
  addMevPeak();
  computeMev2ADCratio();
}

short LaurentMethod::adcFromMev(float mev, int sensor) {
   PMTnumbers thisPMT=PMTs[sensor];
   float adcShift = mev * thisPMT.mev2adc;
   float adcShape = thisPMT.a * adcShift + thisPMT.b;
   if (adcShape<=0) adcShape=adcShift; // could be switched to 0
   int untrimmedPMT = static_cast<int> (adcShape + thisPMT.pedMean);
   return trimADC(untrimmedPMT);
}


void LaurentMethod::convertDatacard() {
  int iPMT=0;
  for (auto line : datacard) {
      PMTnumbers pmt;
      pmt.index=PMTiterator[iPMT];
      pmt.pedMean =line[0];
      pmt.pedSigma=line[1];
      pmt.maxPeak =line[2];
      pmt.isScint =  (line[3] != 0) ;

      PMTs[iPMT]=pmt;
      iPMT++;
  }
  return;
}



void LaurentMethod::addMevPeak()
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
        PMTs[idx].mevPeak=peak;
    }
    return;
}


void LaurentMethod::computeMev2ADCratio() {

  for (auto pmt : PMTs) {
      float maxMev = pmt.isScint? pmt.mevPeak:15;
      float maxADC = pmt.maxPeak - pmt.pedMean;
      PMTs[static_cast<int>(pmt.index)].mev2adc = maxADC / maxMev;
  }

}


void LaurentMethod::setBeamEnergy(int energy) {
  beamEnergy=energy;
  csv2fvec abfile;
  std::vector<std::vector<float>> abvalues=abfile.fromDatacard("laurent_pmt_e_a_b.csv");
  for (auto line : abvalues) {
    if (line[1] != energy) continue;
    int idx = static_cast<int> ( line[0]);
    PMTs[idx].a=line[2];
    PMTs[idx].b=line[3];
  }

  for (auto pmt : PMTs)  std::cout << pmt.index << " "<< pmt.a << " "<< pmt.b  << std::endl;

}
