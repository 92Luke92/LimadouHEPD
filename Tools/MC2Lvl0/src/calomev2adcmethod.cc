/*
 * calomev2adcmethod.cc
 *
 *
 *
 */


#include "calomev2adcmethod.hh"
#include "pmtenum.hh"


calomev2adcmethod::calomev2adcmethod(std::string datacardname)
{
   csv2fvec datagetter;
   datacard=datagetter.fromDatacard(datacardname);
   if (datacard.empty()) std::cerr << "CaloM2A: init failed (datacard file not found)" << std::endl;
}





void calomev2adcmethod::dumpDatacard() {
  for (auto line : datacard) {
     for(float elt:line) std::cout << elt << " ";
     std::cout << std::endl;
  }
}


short calomev2adcmethod::clipADC(int unclippedADC) {
     if (unclippedADC <0 )    unclippedADC = 0;
     if (unclippedADC > NADC) unclippedADC = NADC - 1;
     return static_cast<short> (unclippedADC);
}

short calomev2adcmethod::clipADC(float unclippedADC) {
     return clipADC (static_cast<int> (unclippedADC));
}

void calomev2adcmethod::SetPedFromCalib(LCaloCalibration calocalib) {
     const double* peds= calocalib.GetPedestal();
     for (PMTenum ipmt : PMTiterator) pedestals[ipmt] = peds[ipmt];
     return;
}


void calomev2adcmethod::SetPedFromDatacard(std::string datacardfile="laurentHGpeakshift.csv") {
    csv2fvec dataPed;
    std::vector<std::vector<float>> datacardPedestals = dataPed.fromDatacard (datacardfile);
    if (datacardPedestals.empty() ) std::cerr << "MEV2ADC: init failed (pedestals datacard file not found)" << std::endl;
    for (PMTenum ipmt : PMTiterator) pedestals[ipmt] = datacardPedestals[ipmt][0];
    return;
}
