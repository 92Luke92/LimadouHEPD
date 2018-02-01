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


MatteoMethod::MatteoMethod (std::string datacardname) : calomev2adcmethod (datacardname)
{
    init();
}



void MatteoMethod::init()
{
    getFitParameters();
    getPedestals();
}

void MatteoMethod::getFitParameters()
{
    for (auto line : datacard) {
        const int iPMT = static_cast<int> (line[0]);
        PMTnumbersMatteo pmt;
        pmt.Slope = line[1];
        pmt.Interc = line[2];
        pmt.ErrSlope = line[3];
        pmt.ErrInterc = line[4];
        pmtParameters[iPMT] = pmt;
    }
    return;
}


void MatteoMethod::getPedestals()
{
    csv2fvec dataPed;
    std::vector<std::vector<float>> datacardPedestals = dataPed.fromDatacard ("laurentHGpeakshift.csv");
    if (datacardPedestals.empty() ) std::cerr << "MatteoM: init failed (pedestals datacard file not found)" << std::endl;
    int iPMT = 0;
    for (auto line : datacardPedestals) {
        pmtParameters[iPMT].pedMean = line[0];
        iPMT++;
    }
    return;
}



float MatteoMethod::adcFromMevNoPed (float mev, int sensor)
{
    PMTnumbersMatteo thisPMT = pmtParameters[sensor];
    // See comment at top: slope, intercept defined such that E(data)/E(MC) = slope * E(MC) + intercept
    float fadc = mev * mev * thisPMT.Slope + mev * thisPMT.Interc;
    fadc += thisPMT.pedMean;
    return clipADC (fadc);
}

short MatteoMethod::Err_adcFromMev (float mev, int sensor)
{
    PMTnumbersMatteo thisPMT = pmtParameters[sensor];
    float fErr_adc = mev * std::sqrt (mev * mev * thisPMT.ErrSlope * thisPMT.ErrSlope + thisPMT.ErrInterc * thisPMT.ErrInterc);
    short Err_adc = static_cast<short> (fErr_adc);
    return Err_adc;
}
