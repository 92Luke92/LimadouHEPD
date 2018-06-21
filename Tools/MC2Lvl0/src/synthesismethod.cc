/*
 * synthesismethod.cc
 *
 *
 *
 */


#include "synthesismethod.hh"
#include <cmath>
#include <random> // getting rid of ROOT whenever possible.





SynthesisMethod::SynthesisMethod (std::string datacardname) : calomev2adcmethod (datacardname)
{
    init();
}



void SynthesisMethod::init()
{
    for (auto line : datacard) {
        const int iPMT = static_cast<int> (line[0]);
        PMTnumbers pmt;
        pmt.meanA = line[1];
        pmt.meanB = line[2];
        pmt.meanC = line[3];
        pmt.irqA  = line[4];
        pmt.irqB  = line[5];
        pmt.irqC  = line[6];
        pmtParameters[iPMT] = pmt;
    }

    return;
}





float SynthesisMethod::adcFromMevNoPed (float mev, int sensor)
{
    PMTnumbers pmt = pmtParameters[sensor];
    float mean= pmt.meanA * mev * mev + pmt.meanB * mev; // no Ped -> no meanC
    float irq = pmt.irqA * mev * mev  + pmt.irqB * mev + pmt.irqC;
    std::normal_distribution<float> Gaussian(mean, iqr2std(irq));
    std::default_random_engine seed(time(0));
    float fadc=Gaussian(seed);
    return clipADC (fadc);
}


