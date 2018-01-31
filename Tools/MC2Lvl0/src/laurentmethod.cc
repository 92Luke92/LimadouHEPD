/*
 * laurentmethod.cc
 *

 *
 *
 */


#include "laurentmethod.hh"


LaurentMethod::LaurentMethod(std::string datacardname):MeV2ADCMethod(datacardname){
  UpdateMyPMTs();
}

void LaurentMethod::UpdateMyPMTs(){
	int i=0;
	for (auto pmt : PMTs) {
		PMTnumbersLaurent tmp(PMTs[i]);
		MyPMTs[i]=tmp;
		i++;
	}
}

float LaurentMethod::adcFromMevNoPed(float mev, int sensor) {
   PMTnumbersLaurent thisPMT=MyPMTs[sensor];
   float adcShift = mev * thisPMT.mev2adc;
   float adcShape = thisPMT.a * adcShift + thisPMT.b;
   if (adcShape<=0) adcShape=adcShift; // could be switched to 0
   int unclippedPMT = static_cast<int> (adcShape + thisPMT.pedMean);
   return clipADC(unclippedPMT);
}


void LaurentMethod::setBeamEnergy(int energy) {
   beamEnergy=energy;
  csv2fvec abfile;
  std::vector<std::vector<float>> abvalues=abfile.fromDatacard("laurent_pmt_e_a_b.csv");
  if (abvalues.empty()) std::cerr << "LaurentMethod: SetBeamEnergy failed (AB file not found)" << std::endl;
  for (auto line : abvalues) {
    if (line[1] != energy) continue;
    int idx = static_cast<int> ( line[0]);
    MyPMTs[idx].a=line[2];
    MyPMTs[idx].b=line[3];
  }
}
