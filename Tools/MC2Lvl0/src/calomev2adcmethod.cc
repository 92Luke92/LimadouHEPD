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
   setPedFromDatacard(); // consistency with previous numbers / methods, will be changed to datacards
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


void calomev2adcmethod::setPedFromCalib(LCaloCalibration calocalib) {
     const double* peds= calocalib.GetPedestal();
     for (PMTenum ipmt : PMTiterator) pedestals[ipmt] = peds[ipmt];
     return;
}


void calomev2adcmethod::setPedFromDatacard(std::string datacardfile) {
    csv2fvec dataPed;
    std::vector<std::vector<float>> datacardPedestals = dataPed.fromDatacard (datacardfile);
    if (datacardPedestals.empty() ) std::cerr << "CALOMEV2ADC: init failed (pedestals datacard file not found)" << std::endl;
    for (PMTenum ipmt : PMTiterator) pedestals[ipmt] = datacardPedestals[ipmt][0];
    return;
}


short calomev2adcmethod::adcFromMev (float mev, int sensor) {
     float rawadc=adcFromMevNoPed(mev, sensor);
     if (rawadc<0) {
          std::cerr << "CALOMEV2ADC: Negative raw MeV value for sensor "
                    << sensor << " : " << rawadc << std::endl;
          rawadc=0;
     }
     float pedadc= rawadc + pedestals[sensor];
     return clipADC(pedadc);
}
