/*
 * optphotmethod.cc
 *
 *
 *
 */

/*
The structure of OptPhot_HG.csv and OptPhot_LG.csv is as follows:
PMTid,    pedestal mean,    pedestal sigma,    conversion factor
*/

#include "optphotmethod.hh"
#include <cmath>
#include <random>


OptPhotMethod::OptPhotMethod (std::string datacardname) : calomev2adcmethod (datacardname)
{
    init();
}



void OptPhotMethod::init()
{
    setPedFromDatacard("nullped.csv");
    for (auto line : datacard) {
        const int iPMT = static_cast<int> (line[0]);
        PMTnumbersOptPhot pmt;
        pmt.Ped = line[1];
	pmt.Sigma = line[2];
	pmt.Conv = line[3];
	pmtParameters[iPMT] = pmt;
    }	     
    return;
}


float OptPhotMethod::adcFromMevNoPed (float mev, int sensor)
{
    PMTnumbersOptPhot thisPMT = pmtParameters[sensor];

    std::default_random_engine gen;
    std::normal_distribution<double> distr(thisPMT.Ped,thisPMT.Sigma);
    
    float fadc = distr(gen) + mev * thisPMT.Conv;
    return fadc;
}
