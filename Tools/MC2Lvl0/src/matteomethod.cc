/*
 * francescomethod.cc
 *
 *
 *
 */


#include "matteomethod.hh"
#include <cmath>


/*
    datacard "PmtSlopeIntercErr_MeanMatteo.csv" :
    - it contains the pmt, slope, intercept, err_slope, err_intercept as columns
    - the values are referred to the plot 'DeltaE_data/DeltaE_MC vs DeltaE_MC' for each calo scintillator plane,
      where DeltaE_data(MC) is the mean of the gaussian that fits the peak of the energy loss distributions of the data(MC)
    - assumption: linear relation of the type y = slope * x + intercept, where x = DeltaE_MC and y = DeltaE_data/DeltaE_MC
*/

/*
    datacard "PmtSlopeIntercErr_SigmaMatteo.csv" :
    - it contains the pmt, slope, intercept, err_slope, err_intercept as columns
    - the values are referred to the plot 'sigma_data/sigma_MC vs DeltaE_MC' for each calo scintillator plane,
      where sigma_data(MC) is the standard deviation of the gaussian that fits the peak of the energy loss distributions of the data(MC)
    - assumption: linear relation of the type y = slope * x + intercept, where x = sigma_MC and y = sigma_data/sigma_MC
*/


MatteoMethod::MatteoMethod(std::string datacardname) : MeV2ADCMethod(datacardname)
{
    convertParameterDatacard();
	UpdateMyPMTs();
}

void MatteoMethod::UpdateMyPMTs(){
	int i = 0;
	for (auto pmt : PMTs) {
		PMTnumbersMatteo tmp(PMTs[i]);
		MyPMTs[i] = tmp;
		i++;
	}
}

void MatteoMethod::convertParameterDatacard(){
for (auto line : datacard) {
    int idx = static_cast<int> (line[0]);
    MyPMTs[idx].Slope = line[1];
    MyPMTs[idx].Interc = line[2];
    MyPMTs[idx].ErrSlope = line[3];
    MyPMTs[idx].ErrInterc = line[4];
  }
}

short MatteoMethod::adcFromMev(float mev, int sensor) {
	PMTnumbersMatteo thisPMT = MyPMTs[sensor];
    float adc_tmp = mev * thisPMT.Slope + thisPMT.Interc;
    short adc = static_cast<short> (adc_tmp);
    return adc;
}

short MatteoMethod::Err_adcFromMev(float mev, int sensor) {
	PMTnumbersMatteo thisPMT = MyPMTs[sensor];
    float Err_adc_tmp = std::sqrt(mev * mev * thisPMT.Slope * thisPMT.Slope + thisPMT.Interc * thisPMT.Interc);
    short Err_adc = static_cast<short> (Err_adc_tmp);
    return Err_adc;
}
