/*
 * optphotmethod.cc
 *
 *
 *
 */

/*
The structure of OptPhot_HG.csv and OptPhot_LG.csv is as follows:
PMTid,    pedestal mean,    pedestal sigma,    p0,    p1,    p2 of quadratic fit
*/

#include "optphotmethod.hh"
#include <cmath>
#include <random>


OptPhotMethod::OptPhotMethod (std::string datacardname) : calomev2adcmethod (datacardname)
{
    init();
    Rand = new TRandom3(559);
}

OptPhotMethod::~OptPhotMethod(){
  delete Rand;
}


void OptPhotMethod::init()
{
    setPedFromDatacard("nullped.csv");
    for (auto line : datacard) {
        const int iPMT = static_cast<int> (line[0]);
        PMTnumbersOptPhot pmt;
        pmt.Ped = line[1];
	pmt.Sigma = line[2];
	pmt.p0 = line[3];
	pmt.p1 = line[4];
	pmt.p2 = line[5];
	pmtParameters[iPMT] = pmt;
    }	     
    return;
}


float OptPhotMethod::adcFromMevNoPed (float mev, int sensor)
{
    PMTnumbersOptPhot thisPMT = pmtParameters[sensor];

    float fadc = Rand->Gaus(thisPMT.Ped,thisPMT.Sigma) + thisPMT.p0 + thisPMT.p1*mev + thisPMT.p2*mev*mev;

    double thesigma = 0.;
    float gaus_fadc = fadc + Rand->Gaus(0.,thesigma);

    return gaus_fadc;
}
